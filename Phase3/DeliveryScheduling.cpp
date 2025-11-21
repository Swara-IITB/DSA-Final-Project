#include "DeliveryScheduling.hpp"

ll time(const Graph& g, std::vector<ll>path){
    double ans=0;
    for(ll i=0; i<path.size();i++){
        ll edgeid = g.adjMatrix[path[i]][path[i+1]];
        Edge edge = g.edges.at(edgeid);
        ans+= edge.avg_t;
    }
    return ans;
}

std::vector<ll> shortest_path(const Graph& g, ll pickup, ll drop){
    std::vector<ll> parent(g.V, -1);
    std::vector<double> dist(g.V, std::numeric_limits<double>::infinity());
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
     dist[pickup] = 0.0;
    pq.push({0.0, pickup});

    while (!pq.empty()) {
        auto x = pq.top();
        auto d = x.first;
        auto u = x.second;
        pq.pop();
        if (d > dist[u]) continue;
        if (u == drop) break;
        for (auto& pr : g.adjList[u]) {
            ll v = pr.first;
            ll eid = pr.second;
            auto it = g.edges.find(eid);
            if (it == g.edges.end()) continue;

            const Edge& e = it->second;
            if (e.disable) continue;

            double w = e.len;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    std::vector<ll> path;
    if (dist[drop] == std::numeric_limits<double>::infinity()) {
        return path;
    }

    for (ll cur = drop; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Assignments> scheduling(const Graph& g,ll num_delivery_guys, ll depot_node, std::vector<Orders>){

}