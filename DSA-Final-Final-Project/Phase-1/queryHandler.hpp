#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "graph.hpp"
#include "knn.hpp"
#include "shortestPath.hpp"
using json = nlohmann::json;
typedef long long ll;

json process_query_phase1(Graph& g, json query);

json process_remove_edge(Graph& g, json query);

json process_modify_edge(Graph& g,json query);

json process_shortest_path(Graph& g,json query);

json process_knn(Graph& g,json query);