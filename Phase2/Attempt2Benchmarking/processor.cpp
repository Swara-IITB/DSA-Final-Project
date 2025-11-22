#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>
#include "../../Phase1/graph.hpp"
#include "../../Phase1/loadGraph1.hpp"
#include "algo4.hpp"

typedef long long ll;
using json = nlohmann::json;

double eps = 1e-9;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <graph.json>" << std::endl;
        return 1;
    }

    Graph g = loadGraph_parse(argv[1]);
    ll V = g.V;
    // Number of random test queries
    int NUM_QUERIES = 30;
    std::mt19937 rng(42);
    std::uniform_int_distribution<ll> dist(0, V-1);

    json output;
    output["results"] = json::array();

    for (int i = 0; i < NUM_QUERIES; i++) {

        ll src = dist(rng);
        ll dst = dist(rng);
        if (src == dst) dst = (dst + 1) % V;

        json one;
        one["src"] = src;
        one["dst"] = dst;

        // Measure time + get true output for OG A*
        double ogDist;
        auto start = std::chrono::high_resolution_clock::now();
        ogDist = ogAstar(g, src, dst);
        auto end = std::chrono::high_resolution_clock::now();
        double og_ms = std::chrono::duration<double, std::milli>(end - start).count();

        one["ogAstar_ms"] = og_ms;
        one["og_dist"] = ogDist;

        // Helper for timing
        auto time_and_run = [&](auto func, double &result){
            auto t1 = std::chrono::high_resolution_clock::now();
            result = func();
            auto t2 = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double, std::milli>(t2 - t1).count();
        };

        // ---- approx A* meet ----
        double approxMeetDist;
        double approxMeet_ms = time_and_run(
            [&](){ return approxAstarMeet(g, src, dst, 1.2); }, 
            approxMeetDist
        );
        one["approxAstarMeet_ms"] = approxMeet_ms;
        one["approxAstarMeet_dist"] = approxMeetDist;

        // ---- bidirectional OG ----
        double bidirOGDist;
        double bidirOG_ms = time_and_run(
            [&](){ return bidirOGAstar(g, src, dst); },
            bidirOGDist
        );
        one["bidirOGAstar_ms"] = bidirOG_ms;
        one["bidirOGAstar_dist"] = bidirOGDist;

        // ---- bidirectional approx ----
        double bidirApproxDist;
        double bidirApprox_ms = time_and_run(
            [&](){ return bidirApproxAstar(g, src, dst, 1.2); },
            bidirApproxDist
        );
        one["bidirApproxAstar_ms"] = bidirApprox_ms;
        one["bidirApproxAstar_dist"] = bidirApproxDist;

        // ---- error percentage (relative to OG A*) ----
        auto err = [&](double approxVal){
            return 100.0 * std::abs(approxVal - ogDist) / (ogDist + eps);
        };

        one["error_approxMeet_%"] = err(approxMeetDist);
        one["error_bidirOG_%"] = err(bidirOGDist);
        one["error_bidirApprox_%"] = err(bidirApproxDist);

        output["results"].push_back(one);
    }

    std::ofstream fout("algo_benchmark_output.json");
    fout << output.dump(2);
    fout.close();

    std::cout << "Benchmark complete. Output in algo_benchmark_output.json\n";
    return 0;
}
