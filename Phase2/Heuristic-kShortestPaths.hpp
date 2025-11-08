#pragma once
#include "graph.hpp"
#include<iostream>
#include<queue>
#include<algorithm>
std::vector<std::pair<std::vector<ll>, double>> ksp(const Graph& g, ll source, ll target, ll k, ll threshold);
std::pair<std::vector<ll>,double> dijsktra(const Graph& g, ll source, ll target,std::vector<ll>& timesUsed);