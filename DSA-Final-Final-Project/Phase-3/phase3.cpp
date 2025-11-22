#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include "graph3.hpp"
#include "loadGraph3.hpp"
#include "queryHandler3.hpp"
#include "minimum_penalty_next_scheduler.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <graph.json> <queries.json> <output.json>" << std::endl;
        return 1;
    }

    // Read graph from first file
    std::string filename = argv[1];
    Graph g = loadGraph_parse3(filename); // will throw runtime error if unable to open
                        // for precomputation we can use floyd warshall for small graphs, hopefully

    // Read queries from second file
    std::ifstream queries_file(argv[2]);
    if (!queries_file.is_open()) {
        std::cerr << "Failed to open " << argv[2] << std::endl;
        return 1;
    }

    json queries_json;
    queries_file >> queries_json;
    queries_file.close();

    // json meta = queries_json["meta"];
    // std::vector<json> results;

        auto start_time = std::chrono::high_resolution_clock::now();

        /*
            Add your query processing code here
            Each query should return a json object which should be printed to sample.json
        */

        // Answer each query replacing the function process_query using 
        // whatever function or class methods that you have implemented
        // std::cout << queries_json.dump(2) << std::endl;

        json result = process_query_phase3(g,queries_json);

        auto end_time = std::chrono::high_resolution_clock::now();
        result["processing_time"] = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    
    

    std::ofstream output_file(argv[3]);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output.json for writing" << std::endl;
        return 1;
    }

    json output;
    output = result;
    output_file << output.dump(4) << std::endl;

    output_file.close();
    return 0;
}