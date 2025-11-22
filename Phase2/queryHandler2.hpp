#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "../Phase1/graph.hpp"
#include "Heuristic-kShortestPaths.hpp"
#include "Exact-kShortestPaths.hpp"
#include "approx_path.hpp"
#include "preprocess.hpp"
using json = nlohmann::json;
typedef long long ll;

json process_query_phase2(Graph& g, json query, std::chrono::time_point<std::chrono::high_resolution_clock> start_time, LandmarkOracle oracle);

json process_k_shortest_paths(Graph& g, json query);

json process_k_shortest_paths_heuristic(Graph& g, json query);

json process_approx_shortest_path(Graph& g, json query, std::chrono::time_point<std::chrono::high_resolution_clock> start_time, LandmarkOracle oracle);