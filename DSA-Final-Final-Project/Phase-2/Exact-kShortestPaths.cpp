#include "Exact-kShortestPaths.hpp"
typedef long long ll;
std::pair<std::vector<ll>, double>
run_dijkstra(const Graph& g, ll start,ll goal,const std::set<ll>& banned_nodes,const std::set<ll>& banned_edges){
    const double INF = 1e18;
    std::vector<double> dist(g.V, INF);
    std::vector<ll> parent(g.V, -1);
    std::vector<ll> parent_edge(g.V, -1);
    std::vector<bool> visited(g.V, false);
    auto cmp = [](const std::pair<double,ll>& a,
                  const std::pair<double,ll>& b){
        return a.first > b.first;
    };
    std::priority_queue<std::pair<double,ll>,std::vector<std::pair<double,ll>>,decltype(cmp)> pq(cmp);
    if (banned_nodes.count(start)) return {{}, INF};
    dist[start] = 0;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [d, node] = pq.top();
        pq.pop();
        if (visited[node]) continue;
        visited[node] = true;
        if (node == goal) break;
        for (auto &edge_info : g.adjList.at(node)) {
            ll next = edge_info.first;
            ll edge_id = edge_info.second;
            if (banned_nodes.count(next)) continue;
            if (banned_edges.count(edge_id)) continue;
            const Edge& ed = g.edges.at(edge_id);
            if (ed.disable) continue;
            double newDist = d + ed.len;
            if (newDist < dist[next]) {
                dist[next] = newDist;
                parent[next] = node;
                parent_edge[next] = edge_id;
                pq.push({newDist, next});
            }
        }
    }
    if (dist[goal] >= INF) return {{}, INF};
    std::vector<ll> path;
    ll cur = goal;
    while (cur != -1) {
        path.push_back(cur);
        cur = parent[cur];
    }
    std::reverse(path.begin(), path.end());
    return {path, dist[goal]};
}
struct candidatePath {
    double cost;
    std::vector<ll> path;
};
std::vector<std::pair<std::vector<ll>, double>>
exact_ksp(const Graph& g, ll source, ll target, ll k)
{
    std::vector<std::pair<std::vector<ll>,double>> result;
    if (k <= 0) return result;
    auto first = run_dijkstra(g, source, target, {}, {});
    if (first.first.empty()) return result;
    result.push_back(first);
    auto cmp = [](const candidatePath& a, const candidatePath& b){
        return a.cost > b.cost;
    };
    std::priority_queue<
        candidatePath,
        std::vector<candidatePath>,
        decltype(cmp)
    > pq(cmp);
    for (ll pi = 1; pi < k; pi++) {
        const auto& prev_path = result.back().first;
        for (ll i = 0; i < (ll)prev_path.size() - 1; i++) {
            ll deviation_node = prev_path[i];
            std::vector<ll> root(prev_path.begin(),
                                 prev_path.begin() + i + 1);
            std::set<ll> banned_nodes;
            std::set<ll> banned_edges;
            for (ll nd : root) {
                if (nd != deviation_node) banned_nodes.insert(nd);
            }
            for (auto& prev : result) {
                const auto& path = prev.first;
                if (path.size() > i &&
                    std::equal(root.begin(), root.end(), path.begin())) {
                    ll u = path[i];
                    ll v = path[i+1];
                    for (auto &adj : g.adjList.at(u)) {
                        if (adj.first == v) {
                            banned_edges.insert(adj.second);
                        }
                    }
                }
            }
           auto dev_res = run_dijkstra(g,
                                        deviation_node,
                                        target,
                                        banned_nodes,
                                        banned_edges);
            if (dev_res.first.empty()) continue;
            std::vector<ll> full = root;
            full.insert(full.end(),
                        dev_res.first.begin() + 1,
                        dev_res.first.end());
            double cost = 0;
            for (ll j = 0; j + 1 < (ll)full.size(); j++) {
                ll a = full[j];
                ll b = full[j+1];
                for (auto &adj : g.adjList.at(a)) {
                    if (adj.first == b) {
                        cost += g.edges.at(adj.second).len;
                        break;
                    }
                }
            }
            pq.push({cost, full});
        }
        if (pq.empty()) break;
        result.push_back({pq.top().path, pq.top().cost});
        pq.pop();
    }
    return result;
}
