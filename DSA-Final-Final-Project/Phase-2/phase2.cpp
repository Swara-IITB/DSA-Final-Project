#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "../Phase-1/graph.hpp"
#include "../Phase-1/loadGraph1.hpp"
#include "queryHandler2.hpp"
#include "preprocess.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <graph.json> <queries.json> <output.json>" << std::endl;
        return 1;
    }

    // Read graph from first file
    std::string filename = argv[1];
    Graph g = loadGraph_parse(filename); // will throw runtime error if unable to open
    ll L;
    if(g.V>10000) L = 64;
    else L = 32;
    auto t0 = std::chrono::high_resolution_clock::now();
    LandmarkOracle oracle = preprocessLandmarks(g, L);
    auto t1 = std::chrono::high_resolution_clock::now();
    double prep_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "preprocessing_time_ms:" << prep_ms << std::endl;
    // Read queries from second file
    std::ifstream queries_file(argv[2]);
    if (!queries_file.is_open()) {
        std::cerr << "Failed to open " << argv[2] << std::endl;
        return 1;
    }

    json queries_json;
    queries_file >> queries_json;
    queries_file.close();

    json meta = queries_json["meta"];
    std::vector<json> results;

    for (const auto& query : queries_json["events"]) {
        auto start_time = std::chrono::high_resolution_clock::now();
        // Answer each query replacing the function process_query using 
        // whatever function or class methods that you have implemented
        json result = process_query_phase2(g,query,start_time, oracle);

        auto end_time = std::chrono::high_resolution_clock::now();
        result["processing_time"] = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        results.push_back(result);
    }

    std::ofstream output_file(argv[3]);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output.json for writing" << std::endl;
        return 1;
    }

    json output;
    output["meta"] = meta;
    output["results"] = results;
    output_file << output.dump(4) << std::endl;

    output_file.close();
    return 0;
}