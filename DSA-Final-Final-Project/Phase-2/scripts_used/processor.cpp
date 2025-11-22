#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "../preprocess.hpp"
#include "algo4.hpp" // contains odAstar()

using json = nlohmann::json;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " graph.json queries.json\n";
        return 1;
    }

    Graph g = loadGraph_parse(argv[1]);

    int L = 16; // try 32; raise to 64 if needed
    auto t0 = high_resolution_clock::now();
    LandmarkOracle oracle = preprocessLandmarks(g, L);
    auto t1 = high_resolution_clock::now();
    double prep_ms = duration_cast<milliseconds>(t1 - t0).count();

    // load queries
    std::ifstream fin(argv[2]);
    json qjson;
    fin >> qjson;

    json results = json::array();
    for (auto &q : qjson["events"][1]["queries"]) {
        long long u = q["source"];
        long long v = q["target"];

        auto ta = high_resolution_clock::now();
        double trueDist = ogAstar(g, u, v);
        auto tb = high_resolution_clock::now();

        auto tc = high_resolution_clock::now();
        double approxDist = landmarkApproxDist(oracle, u, v);
        auto td = high_resolution_clock::now();

        double astar_us = duration_cast<microseconds>(tb - ta).count();
        double approx_us = duration_cast<microseconds>(td - tc).count();

        double absErr = std::fabs(trueDist - approxDist);
        double relErr = (trueDist > 0.0 ? absErr / trueDist * 100.0 : 0.0);

        results.push_back({
            {"u", u},
            {"v", v},
            {"trueDist", trueDist},
            {"approxDist", approxDist},
            {"absError", absErr},
            {"relError%", relErr},
            {"timeAstar_us", astar_us},
            {"timeApprox_us", approx_us}
        });
    }

    json out;
    out["preprocessing_ms"] = prep_ms;
    out["Landmarks"] = L;
    out["results"] = results;

    std::cout << out.dump(2) << std::endl;
    return 0;
}
