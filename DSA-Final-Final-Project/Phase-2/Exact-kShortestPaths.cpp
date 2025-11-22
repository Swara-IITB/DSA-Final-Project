#include "Exact-kShortestPaths.hpp"
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>
#include <cmath>
#include <map>
#include <utility>

typedef long long ll;

double calculateRootCost(const Graph &theGraph, const std::vector<ll> &pathSegment)
{
    double totalCost = 0.0;
    if (pathSegment.size() < 2)
        return 0.0;

    for (size_t i = 0; i < pathSegment.size() - 1; ++i)
    {
        ll u = pathSegment[i];
        ll v = pathSegment[i + 1];

        // Find the edge ID that connects u and v
        for (const auto &edgeInfo : theGraph.adjList.at(u))
        {
            if (edgeInfo.first == v)
            {
                ll edgeId = edgeInfo.second;
                // Add the edge length
                totalCost += theGraph.edges.at(edgeId).len;
                break;
            }
        }
    }
    return totalCost;
}

std::pair<std::vector<ll>, double>
findShortestPath(const Graph &theGraph,
                 ll startNode, ll endNode,
                 const std::set<ll> &bannedNodes,
                 const std::set<ll> &bannedEdges)
{
    const double INF = 1e18;
    std::vector<double> dist(theGraph.V, INF);
    std::vector<ll> parent(theGraph.V, -1);

    using NodePair = std::pair<double, ll>;
    std::priority_queue<NodePair, std::vector<NodePair>, std::greater<NodePair>> pq;

    if (bannedNodes.count(startNode))
        return {{}, INF};

    dist[startNode] = 0;
    pq.push({0, startNode});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;
        if (u == endNode)
            break;

        for (auto &edgeInfo : theGraph.adjList.at(u))
        {
            ll v = edgeInfo.first;
            ll edgeId = edgeInfo.second;

            if (bannedNodes.count(v))
                continue;
            if (bannedEdges.count(edgeId))
                continue;

            const Edge &edgeData = theGraph.edges.at(edgeId);
            if (edgeData.disable)
                continue;

            double weight = edgeData.len;

            if (d + weight < dist[v])
            {
                dist[v] = d + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[endNode] >= INF)
        return {{}, INF};

    std::vector<ll> path;
    ll current = endNode;
    while (current != -1)
    {
        path.push_back(current);
        current = parent[current];
    }
    std::reverse(path.begin(), path.end());
    return {path, dist[endNode]};
}

struct candidatePath
{
    double cost;
    std::vector<ll> path;
    bool operator>(const candidatePath &other) const
    {
        return cost > other.cost;
    }
};

std::vector<std::pair<std::vector<ll>, double>>
getKBestPaths(const Graph &theGraph, ll source, ll target, ll k)
{
    std::vector<std::pair<std::vector<ll>, double>> kShortestPaths;
    if (k <= 0)
        return kShortestPaths;

    auto firstPath = findShortestPath(theGraph, source, target, {}, {});
    if (firstPath.first.empty())
        return kShortestPaths;

    kShortestPaths.push_back(firstPath);

    std::priority_queue<candidatePath, std::vector<candidatePath>, std::greater<candidatePath>> pq;

    std::set<std::vector<ll>> seenPaths;
    seenPaths.insert(firstPath.first);

    for (ll count = 1; count < k; count++)
    {

        const auto &lastShortestPath = kShortestPaths.back().first;

        for (ll i = 0; i < (ll)lastShortestPath.size() - 1; i++)
        {

            ll spurNode = lastShortestPath[i];

            std::vector<ll> root(lastShortestPath.begin(), lastShortestPath.begin() + i + 1);
            double rootDist = calculateRootCost(theGraph, root);

            std::set<ll> bannedNodes;
            for (ll node : root)
            {
                if (node != spurNode)
                {
                    bannedNodes.insert(node);
                }
            }

            std::set<ll> bannedEdges;
            for (const auto &prev : kShortestPaths)
            {
                const auto &path = prev.first;

                if ((ll)path.size() > i + 1 && std::equal(root.begin(), root.end(), path.begin()))
                {
                    ll u = path[i];
                    ll v = path[i + 1];

                    for (auto &adj : theGraph.adjList.at(u))
                    {
                        if (adj.first == v)
                        {
                            bannedEdges.insert(adj.second);
                            break;
                        }
                    }
                }
            }

            auto spurPathResult = findShortestPath(theGraph,
                                                   spurNode,
                                                   target,
                                                   bannedNodes,
                                                   bannedEdges);

            if (spurPathResult.first.empty())
                continue;

            std::vector<ll> totalPath = root;

            totalPath.insert(totalPath.end(),
                             spurPathResult.first.begin() + 1,
                             spurPathResult.first.end());

            double totalDist = rootDist + spurPathResult.second;

            if (seenPaths.count(totalPath) == 0)
            {
                pq.push({totalDist, totalPath});
                seenPaths.insert(totalPath);
            }
        }

        if (pq.empty())
            break;

        auto nextBest = pq.top();
        pq.pop();
        kShortestPaths.push_back({nextBest.path, nextBest.cost});
    }

    return kShortestPaths;
}