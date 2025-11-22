
#include "DeliveryScheduling.hpp"
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

double time(const Graph& g, std::vector<ll> path) {
    double ans = 0;
    if (path.empty()) return 1e15;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        ll u = path[i], v = path[i+1];
        if (u < 0 || v < 0 || u >= g.V || v >= g.V) { ans += 1e15; continue; }
        ll edgeid = g.adjMatrix[u][v];
        if (edgeid < 0) { ans += 1e15; continue; }
        auto it = g.edges.find(edgeid);
        if (it == g.edges.end()) { ans += 1e15; continue; }
        ans += it->second.avg_t;
    }
    return ans;
}

double distance(const Graph& g, std::vector<ll> path) {
    double ans = 0;
    if (path.empty()) return 1e15;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        ll u = path[i], v = path[i+1];
        if (u < 0 || v < 0 || u >= g.V || v >= g.V) { ans += 1e15; continue; }
        ll edgeid = g.adjMatrix[u][v];
        if (edgeid < 0) { ans += 1e15; continue; }
        auto it = g.edges.find(edgeid);
        if (it == g.edges.end()) { ans += 1e15; continue; }
        ans += it->second.len;
    }
    return ans;
}

std::vector<ll> shortest_path(const Graph& g, ll src, ll dst) {
    std::vector<ll> parent(g.V, -1);
    std::vector<double> dist(g.V, std::numeric_limits<double>::infinity());
    std::priority_queue<
        std::pair<double,ll>,
        std::vector<std::pair<double,ll>>,
        std::greater<std::pair<double,ll>> > pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto x = pq.top();
        auto d = x.first;
        auto u = x.second;
        pq.pop();
        if (d > dist[u]) continue;
        if (u == dst) break;

        for (auto &pr : g.adjList[u]) {
            ll v = pr.first;
            ll eid = pr.second;
            auto it = g.edges.find(eid);
            if (it == g.edges.end()) continue;
            const Edge &e = it->second;
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
    if (dist[dst] == std::numeric_limits<double>::infinity()) return path;
    for (ll cur = dst; cur != -1; cur = parent[cur])
        path.push_back(cur);
    std::reverse(path.begin(), path.end());
    return path;
}

double route_cost(const Graph& g, const std::vector<ll>& stops) {
    if (stops.size() < 2) return 0.0;
    double ans = 0.0;
    for (size_t i = 0; i + 1 < stops.size(); ++i) {
        auto p = shortest_path(g, stops[i], stops[i+1]);
        ans += time(g, p);
    }
    return ans;
}

void compute_best_and_second_best_delta(
    const Graph& g,
    const std::vector<ll>& R,
    ll P, ll D,
    double &best, double &second_best)
{
    best = 1e15; 
    second_best = 1e15;
    double oldC = route_cost(g, R);
    int k = R.size();

    for (int pi = 1; pi <= k; ++pi) {
        for (int di = pi + 1; di <= k + 1; ++di) {
            std::vector<ll> cand;
            cand.reserve(k+2);

            for (int i = 0; i < k; ++i) {
                if (i == pi) cand.push_back(P);
                cand.push_back(R[i]);
                if (i+1 == di) cand.push_back(D);
            }
            if (pi == k) cand.push_back(P);
            if (di == k+1) cand.push_back(D);

            double newC = route_cost(g, cand);
            double delta = newC - oldC;

            if (delta < best) { second_best = best; best = delta; }
            else if (delta < second_best) second_best = delta;
        }
    }
}

std::vector<ll> best_insertion(
    const Graph& g, const std::vector<ll>& R, ll P, ll D, double &deltaOut)
{
    double oldC = route_cost(g, R);
    double best = 1e15;
    std::vector<ll> bestR;
    int k = R.size();

    for (int pi = 1; pi <= k; ++pi) {
        for (int di = pi + 1; di <= k + 1; ++di) {
            std::vector<ll> cand;
            cand.reserve(k+2);

            for (int i = 0; i < k; ++i) {
                if (i == pi) cand.push_back(P);
                cand.push_back(R[i]);
                if (i+1 == di) cand.push_back(D);
            }
            if (pi == k) cand.push_back(P);
            if (di == k+1) cand.push_back(D);

            double newC = route_cost(g, cand);
            double delta = newC - oldC;

            if (delta < best) {
                best = delta;
                bestR = cand;
            }
        }
    }

    if (bestR.empty()) {
        std::vector<ll> tmp = R;
        tmp.push_back(P);
        tmp.push_back(D);
        deltaOut = route_cost(g, tmp) - oldC;
        return tmp;
    }

    deltaOut = best;
    return bestR;
}
std::vector<ll> compress_consecutive(const std::vector<ll>& v) {
    if (v.empty()) return v;
    std::vector<ll> out;
    out.reserve(v.size());
    out.push_back(v[0]);
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i] != v[i - 1]) out.push_back(v[i]);
    }
    return out;
}

std::vector<ll> expand_route(const Graph& g, const std::vector<ll>& stops) {
    std::vector<ll> full;
    if (stops.empty()) return full;

    full.push_back(stops[0]);  // start with first stop

    for (size_t i = 0; i + 1 < stops.size(); ++i) {
        auto p = shortest_path(g, stops[i], stops[i+1]);
        if (p.size() >= 2) {
            // Append p[1..end] (skip the first node to avoid duplication)
            for (size_t j = 1; j < p.size(); ++j)
                full.push_back(p[j]);
        }
        else {
            // fallback: if no path, just add the stop
            full.push_back(stops[i+1]);
        }
    }
    return full;
}

Assignments build_route_regret(
    const Graph& g, ll depot, std::vector<Orders> orders, int driver_id)
{
    Assignments A;
    A.driver_id = driver_id;
    int m = orders.size();
    std::vector<bool> used(m, false);
    std::vector<ll> R = { depot };

    for (int step = 0; step < m; ++step) {
        double bestReg = -1e18;
        int bestIdx = -1;

        for (int i = 0; i < m; ++i) {
            if (used[i]) continue;
            ll P = orders[i].pickup;
            ll D = orders[i].drop;

            double b1, b2;
            compute_best_and_second_best_delta(g, R, P, D, b1, b2);

            if (b1 > 1e14) {
                R.push_back(P);
                R.push_back(D);
                used[i] = true;
                A.order_ids.push_back(orders[i].order_id);
                goto next_step;
            }

            double regret = b2 - b1;
            if (regret > bestReg ||
                (std::abs(regret - bestReg) < 1e-12 && (bestIdx == -1 || orders[i].order_id < orders[bestIdx].order_id))) {
                bestReg = regret;
                bestIdx = i;
            }
        }

        if (bestIdx == -1) break;

        {
            double delta;
            R = best_insertion(g, R,
                orders[bestIdx].pickup,
                orders[bestIdx].drop,
                delta
            );
        }
        used[bestIdx] = true;
        A.order_ids.push_back(orders[bestIdx].order_id);

        next_step:;
    }

        auto stops = compress_consecutive(R);   // clean up stop list
        A.route = expand_route(g, stops);       // expand into full path
        return A;

}


int nearestCenter_byPath(const Graph& g, ll node, const std::vector<ll>& centers){
    double best = 1e15;
    int idx = 0;
    for (int i = 0; i < (int)centers.size(); i++) {
        auto p = shortest_path(g, node, centers[i]);
        double d = distance(g, p);
        if (d < best) { best = d; idx = i; }
    }
    return idx;
}

std::vector<std::vector<Orders>> cluster_by_shortest_path(
    const Graph &g, std::vector<Orders>& ordersList, int K)
{
    int N = ordersList.size();
    int Ksafe = std::min(N, K);
    std::vector<ll> centers(Ksafe);

    for (int i = 0; i < Ksafe; i++)
        centers[i] = ordersList[i].pickup;

    std::vector<int> assign(N,0);

    for (int rep = 0; rep < 3; rep++) {
        for (int i = 0; i < N; i++)
            assign[i] = nearestCenter_byPath(g, ordersList[i].pickup, centers);

        for (int k = 0; k < Ksafe; k++) {
            std::vector<int> idxs;
            for (int i = 0; i < N; i++)
                if (assign[i] == k) idxs.push_back(i);

            if (idxs.empty()) continue;

            double bestSum = 1e15;
            ll bestNode = ordersList[idxs[0]].pickup;

            for (int ii : idxs) {
                ll cand = ordersList[ii].pickup;
                double sum = 0;
                for (int jj : idxs) {
                    auto p = shortest_path(g, cand, ordersList[jj].pickup);
                    sum += distance(g, p);
                    if (sum > bestSum) break;
                }
                if (sum < bestSum) {
                    bestSum = sum;
                    bestNode = cand;
                }
            }
            centers[k] = bestNode;
        }
    }

    std::vector<std::vector<Orders>> buckets(Ksafe);
    for (int i = 0; i < N; i++)
        buckets[assign[i]].push_back(ordersList[i]);
    return buckets;
}

std::vector<Assignments> clustering_method(
    const Graph& g, ll num_delivery_guys, ll depot_node,
    std::vector<Orders> ordersList)
{
    auto buckets = cluster_by_shortest_path(g, ordersList, num_delivery_guys);
    std::vector<Assignments> ans;

    for (int d = 0; d < (int)buckets.size(); d++) {
        if (buckets[d].empty()) {
            Assignments A;
            A.driver_id = d;
            A.route = { depot_node };
            ans.push_back(A);
            continue;
        }
        Assignments A = build_route_regret(g, depot_node, buckets[d], d);
        ans.push_back(A);
    }
    return ans;
}
