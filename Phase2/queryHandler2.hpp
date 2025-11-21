#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "graph.hpp"
#include "Heuristic-kShortestPaths.hpp"
#include "Exact-kShortestPaths.hpp"
#include "ApproxShortestPath.hpp"
using json = nlohmann::json;
typedef long long ll;

json process_query_phase2(Graph& g, json query, std::chrono::time_point<std::chrono::high_resolution_clock> start_time);


