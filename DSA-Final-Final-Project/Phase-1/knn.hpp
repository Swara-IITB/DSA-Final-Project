#pragma once
#include "graph.hpp"
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<queue>
#include<algorithm>
std::vector<ll> euclidean(const Graph &g, std::string poi, ll k, double lat, double lon);
std::vector<ll>shortest_path(const Graph &g, std::string poi, ll k, double lat, double lon);
ll nn(const Graph &g, double lat, double lon);