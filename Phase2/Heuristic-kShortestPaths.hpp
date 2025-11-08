#pragma once
#include "graph.hpp"
#include<iostream>
#include<queue>
#include<algorithm>
std::vector<std::pair<std::vector<ll>, double>> ksp(const Graph& g, ll source, ll target, ll k, ll threshold);
std::vector<ll> dijsktra(const Graph& g, ll source, ll target);
double pathlength(std::vector<ll>path, const Graph&g, std::vector<ll> timesUsed);