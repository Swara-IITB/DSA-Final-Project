#pragma once
#include "../Phase-1/graph.hpp"
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <limits>
#include <set>
#include <iostream>

typedef long long ll;
std::vector<std::pair<std::vector<ll>, double>>
exact_ksp(const Graph& g, ll source, ll target, ll k);
std::pair<std::vector<ll>, double>
run_dijkstra(const Graph& g,ll start,ll goal,const std::set<ll>& banned_nodes,const std::set<ll>& banned_edges);