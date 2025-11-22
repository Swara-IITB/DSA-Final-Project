#include "preprocess.hpp"
#include <queue>
#include <algorithm>
#include <limits>
#include <cmath>


static const double INF = 1e18;

// Single Dijkstra; if useReverse==false uses g.adjList, else uses g.revAdjList
std::vector<double> dijkstraFromSource(const Graph& g, ll src, bool useReverse) {
    std::vector<double> dist(g.V, INF);
    dist[src] = 0.0;
    using P = std::pair<double, ll>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [cd, u] = pq.top(); pq.pop();
        if (cd != dist[u]) continue;

        const auto& neighbours = (useReverse ? g.revAdjList[u] : g.adjList[u]); 
        for (auto &pr : neighbours) {
            ll v = pr.first;
            ll eID = pr.second;
            const auto &e = g.edges.at(eID);
            if (e.disable) continue;
            double w = e.len;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// Choose next landmark = node with maximum of minDist[] (standard farthest-point)
ll farthestNodeFromMinDist(const std::vector<double>& minDist) {
    double mx = -1.0;
    ll idx = 0;
    for (ll i = 0; i < (ll)minDist.size(); ++i) {
        if (minDist[i] < INF && minDist[i] > mx) {
            mx = minDist[i];
            idx = i;
        }
    }
    return idx;
}

LandmarkOracle preprocessLandmarks(Graph& g, int L) {
    LandmarkOracle oracle;
    oracle.L = L;
    oracle.landmarks.reserve(L);
    oracle.dist_from.assign(L, std::vector<double>(g.V, INF));
    oracle.dist_to.assign(L, std::vector<double>(g.V, INF));

    // pick first landmark randomly or 0
    ll first = 0;
    oracle.landmarks.push_back(first);
    oracle.dist_from[0] = dijkstraFromSource(g, first, false);
    // dist_to: run Dijkstra on reverse graph from 'first' => gives dist(v -> first)
    oracle.dist_to[0] = dijkstraFromSource(g, first, true);

    // minDist[v] = min distance from v to any chosen landmark (using dist_from as proxy)
    std::vector<double> minDist(g.V, INF);
    for (ll v = 0; v < g.V; ++v) {
        minDist[v] = oracle.dist_from[0][v];
    }

    for (int i = 1; i < L; ++i) {
        ll next = farthestNodeFromMinDist(minDist);
        oracle.landmarks.push_back(next);
        oracle.dist_from[i] = dijkstraFromSource(g, next, false);
        oracle.dist_to[i]   = dijkstraFromSource(g, next, true);

        // update minDist using newly computed dist_from[i]
        for (ll v = 0; v < g.V; ++v) {
            if (oracle.dist_from[i][v] < minDist[v]) minDist[v] = oracle.dist_from[i][v];
        }
    }

    return oracle;
}

