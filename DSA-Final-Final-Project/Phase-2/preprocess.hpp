#ifndef PREPROCESS_HPP
#define PREPROCESS_HPP

#include "../Phase-1/graph.hpp"
#include <vector>

using ll = long long;

struct LandmarkOracle {
    int L;
    std::vector<ll> landmarks;
    // dist_from[i][v] = distance from landmark i to node v (L -> v)
    std::vector<std::vector<double>> dist_from;
    // dist_to[i][v] = distance from node v to landmark i (v -> L)
    std::vector<std::vector<double>> dist_to;
};

LandmarkOracle preprocessLandmarks(Graph& g, int L);
std::vector<double> dijkstraFromSource(const Graph& g, ll src, bool useReverse=false);
ll farthestNodeFromMinDist(const std::vector<double>& minDist);
double landmarkApproxDist(const LandmarkOracle& oracle, ll u, ll v);

#endif