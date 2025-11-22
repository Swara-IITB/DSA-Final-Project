#include "preprocessing_mpn.hpp"
using namespace std;
static void run_dijkstra_all(const Graph &g,
                             ll src,
                             vector<double> &dist,
                             vector<ll> &parent)
{
    dist.assign(g.V, numeric_limits<double>::infinity());
    parent.assign(g.V, -1);

    priority_queue<pair<double, ll>,
                   vector<pair<double, ll>>,
                   greater<pair<double, ll>>>
        pq;

    dist[src] = 0;
    pq.push({0.0, src});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u])
            continue;

        for (auto &pr : g.adjList[u])
        {
            ll v = pr.first;
            ll eid = pr.second;
            const Edge &e = g.edges.at(eid);
            if (e.disable)
                continue;

            double w = e.len;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

PreprocessedMPN preprocess_mpn(
    const Graph &g,
    ll depot_node,
    const vector<ll> &pickups,
    const vector<ll> &dropoffs)
{
    PreprocessedMPN P;

    P.sources.push_back(depot_node);
    for (ll x : pickups)
        P.sources.push_back(x);
    for (ll x : dropoffs)
        P.sources.push_back(x);

    sort(P.sources.begin(), P.sources.end());
    P.sources.erase(unique(P.sources.begin(), P.sources.end()), P.sources.end());

    for (ll s : P.sources)
    {
        vector<double> dist;
        vector<ll> parent;
        run_dijkstra_all(g, s, dist, parent);
        P.dist[s] = dist;
        P.parent[s] = parent;
    }

    return P;
}
