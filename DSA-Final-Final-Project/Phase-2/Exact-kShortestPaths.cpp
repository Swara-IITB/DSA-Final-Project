#include "Exact-kShortestPaths.hpp"

typedef long long ll;
std::pair<std::vector<ll>, double>
findShortestPath(const Graph &g,
                 ll startNode, ll endNode,
                 const std::set<ll> &bannedNodes,
                 const std::set<ll> &bannedEdges)
{
    const double INF = 1e18;
    std::vector<double> dist(g.V, INF);
    std::vector<ll> parent(g.V, -1);

    using NodePair = std::pair<double, ll>;
    std::priority_queue<NodePair, std::vector<NodePair>, std::greater<NodePair>> pq;

    if (bannedNodes.count(startNode))
    {
        return {{}, INF};
    }

    dist[startNode] = 0.0;
    pq.push({0.0, startNode});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;
        if (u == endNode)
            break;

        for (auto &edgeInfo : g.adjList.at(u))
        {
            ll v = edgeInfo.first;
            ll edgeId = edgeInfo.second;

            if (bannedNodes.count(v))
                continue;
            if (bannedEdges.count(edgeId))
                continue;

            const Edge &ed = g.edges.at(edgeId);
            if (ed.disable)
                continue;

            double w = ed.len;

            if (d + w < dist[v])
            {
                dist[v] = d + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[endNode] >= INF)
    {
        return {{}, INF};
    }

    std::vector<ll> path;
    ll cur = endNode;
    while (cur != -1)
    {
        path.push_back(cur);
        cur = parent[cur];
    }
    std::reverse(path.begin(), path.end());

    return {path, dist[endNode]};
}

static double pathCost(const Graph &g, const std::vector<ll> &path)
{
    double total = 0.0;
    if (path.size() < 2)
        return 0.0;

    for (size_t i = 0; i + 1 < path.size(); i++)
    {
        ll u = path[i];
        ll v = path[i + 1];

        // find edge u -> v
        for (auto &edgeInfo : g.adjList.at(u))
        {
            if (edgeInfo.first == v)
            {
                ll edgeId = edgeInfo.second;
                const Edge &ed = g.edges.at(edgeId);
                total += ed.len;
                break;
            }
        }
    }
    return total;
}

struct candidatePath
{
    double cost;
    std::vector<ll> path;
};

static bool operator>(const candidatePath &a, const candidatePath &b)
{
    return a.cost > b.cost;
}

std::vector<std::pair<std::vector<ll>, double>>
getKBestPaths(const Graph &g, ll source, ll target, ll k)
{
    std::vector<std::pair<std::vector<ll>, double>> result;
    if (k <= 0)
        return result;

    auto first = findShortestPath(g, source, target, {}, {});
    if (first.first.empty())
        return result; // no path at all

    result.push_back(first);

    std::priority_queue<
        candidatePath,
        std::vector<candidatePath>,
        std::greater<candidatePath>>
        candPQ;

    std::set<std::vector<ll>> seen;
    seen.insert(first.first);

    const double INF = 1e18;

    for (ll kth = 1; kth < k; kth++)
    {

        for (ll pi = 0; pi < (ll)result.size(); pi++)
        {
            const auto &basePath = result[pi].first;
            for (ll i = 0; i + 1 < (ll)basePath.size(); i++)
            {
                ll spurNode = basePath[i];

                std::vector<ll> root(basePath.begin(), basePath.begin() + i + 1);

                std::set<ll> bannedNodes;
                for (ll j = 0; j < i; j++)
                {
                    bannedNodes.insert(root[j]);
                }

                std::set<ll> bannedEdges;
                for (const auto &prev : result)
                {
                    const auto &p = prev.first;

                    if ((ll)p.size() > i &&
                        std::equal(root.begin(), root.end(), p.begin()))
                    {
                        ll u = p[i];
                        ll v = p[i + 1];

                        for (auto &adj : g.adjList.at(u))
                        {
                            if (adj.first == v)
                            {
                                bannedEdges.insert(adj.second);
                                break;
                            }
                        }
                    }
                }

                auto spurRes = findShortestPath(g, spurNode, target,
                                                bannedNodes, bannedEdges);
                if (spurRes.first.empty())
                {
                    continue;
                }

                std::vector<ll> totalPath = root;
                totalPath.insert(totalPath.end(),
                                 spurRes.first.begin() + 1,
                                 spurRes.first.end());

                if (seen.count(totalPath))
                {
                    continue;
                }

                double totalCost = pathCost(g, totalPath);
                candPQ.push({totalCost, totalPath});
                seen.insert(totalPath);
            }
        }

        if (candPQ.empty())
        {
            break;
        }

        candidatePath nextBest = candPQ.top();
        candPQ.pop();
        result.push_back({nextBest.path, nextBest.cost});
    }

    return result;
}
