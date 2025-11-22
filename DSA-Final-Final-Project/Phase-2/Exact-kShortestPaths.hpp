#pragma once
#include "../Phase-1/graph.hpp"
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>
#include <cmath>
#include <map>
#include <utility>


typedef long long ll;

std::vector<std::pair<std::vector<ll>, double>>
getKBestPaths(const Graph &g, ll source, ll target, ll k);

std::pair<std::vector<ll>, double>
findShortestPath(const Graph &g,
                 ll startNode, ll endNode,
                 const std::set<ll> &bannedNodes,
                 const std::set<ll> &bannedEdges);