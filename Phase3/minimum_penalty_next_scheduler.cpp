#include "minimum_penalty_next_scheduler.hpp"
using namespace std;
static long long encode_key(ll src, ll dst)
{
    return src * 1000000007LL + dst;
}

static double cached_dijkstra_dist(const Graph &g,
                                   ll src, ll dst,
                                   unordered_map<long long, double> &cache)
{
    long long key = encode_key(src, dst);
    auto it = cache.find(key);
    if (it != cache.end())
        return it->second;

    vector<double> dist(g.V, numeric_limits<double>::infinity());
    priority_queue<pair<double, ll>, vector<pair<double, ll>>, greater<pair<double, ll>>> pq;

    dist[src] = 0;
    pq.push({0.0, src});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;
        if (u == dst)
            break;

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
                pq.push({dist[v], v});
            }
        }
    }

    cache[key] = dist[dst];
    return dist[dst];
}
static vector<ll> cached_dijkstra_path(const Graph &g,
                                       ll src, ll dst,
                                       unordered_map<long long, vector<ll>> &path_cache)
{
    long long key = encode_key(src, dst);
    auto it = path_cache.find(key);
    if (it != path_cache.end())
        return it->second;

    vector<double> dist(g.V, numeric_limits<double>::infinity());
    vector<ll> parent(g.V, -1);

    priority_queue<pair<double, ll>, vector<pair<double, ll>>, greater<pair<double, ll>>> pq;

    dist[src] = 0;
    pq.push({0.0, src});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;
        if (u == dst)
            break;

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

    vector<ll> path;
    if (dist[dst] == numeric_limits<double>::infinity())
    {
        path_cache[key] = path;
        return path;
    }

    ll curr = dst;
    while (curr != -1)
    {
        path.push_back(curr);
        curr = parent[curr];
    }
    reverse(path.begin(), path.end());

    path_cache[key] = path;
    return path;
}
MPNResult run_mpn_scheduler(const Graph &g,
                            ll depot_node,
                            const vector<ll> &order_ids,
                            const vector<ll> &pickups,
                            const vector<ll> &dropoffs,
                            const vector<double> &max_times,
                            const vector<ll> &multipliers)
{
    struct OrderData
    {
        ll id, p, d, mult;
        double mx;
    };

    vector<OrderData> orders;
    for (int i = 0; i < (int)order_ids.size(); i++)
    {
        orders.push_back({order_ids[i],
                          pickups[i],
                          dropoffs[i],
                          multipliers[i],
                          max_times[i]});
    }

    unordered_map<long long, double> dist_cache;
    unordered_map<long long, vector<ll>> path_cache;

    ll current = depot_node;
    double global_time = 0.0;

    vector<ll> route;
    route.push_back(current);

    unordered_map<ll, double> finish_time;
    unordered_map<ll, ll> penalties;
    ll total_penalty = 0;

    while (!orders.empty())
    {

        double best_pen = numeric_limits<double>::infinity();
        int best_idx = -1;
        double best_pickup_dist = 0;

        for (int i = 0; i < (int)orders.size(); i++)
        {
            auto &o = orders[i];

            double dist1 = cached_dijkstra_dist(g, current, o.p, dist_cache);
            double dist2 = cached_dijkstra_dist(g, o.p, o.d, dist_cache);

            double finish = global_time + dist1 + dist2;

            double p = 0;
            if (finish > o.mx)
            {
                double late = finish - o.mx;
                ll units = ceil(late / 10.0);
                p = units * o.mult;
            }

            if (p < best_pen)
            {
                best_pen = p;
                best_idx = i;
                best_pickup_dist = dist1;
            }
            else if (fabs(p - best_pen) < 1e-9)
            {
                if (o.mult > orders[best_idx].mult)
                {
                    best_idx = i;
                    best_pickup_dist = dist1;
                }
                else if (o.mult == orders[best_idx].mult)
                {
                    if (dist1 < best_pickup_dist)
                    {
                        best_idx = i;
                        best_pickup_dist = dist1;
                    }
                }
            }
        }

        OrderData chosen = orders[best_idx];
        orders.erase(orders.begin() + best_idx);
        vector<ll> p1 = cached_dijkstra_path(g, current, chosen.p, path_cache);
        if (!p1.empty())
        {
            p1.erase(p1.begin()); 
            route.insert(route.end(), p1.begin(), p1.end());
        }

        global_time += cached_dijkstra_dist(g, current, chosen.p, dist_cache);
        vector<ll> p2 = cached_dijkstra_path(g, chosen.p, chosen.d, path_cache);
        if (!p2.empty())
        {
            p2.erase(p2.begin());
            route.insert(route.end(), p2.begin(), p2.end());
        }

        global_time += cached_dijkstra_dist(g, chosen.p, chosen.d, dist_cache);

        finish_time[chosen.id] = global_time;

        ll pen_val = 0;
        if (global_time > chosen.mx)
        {
            double late = global_time - chosen.mx;
            ll units = ceil(late / 10.0);
            pen_val = units * chosen.mult;
        }

        penalties[chosen.id] = pen_val;
        total_penalty += pen_val;

        current = chosen.d;
    }

    MPNResult result;
    result.route = route;
    result.finish_time = finish_time;
    result.penalty = penalties;
    result.total_penalty = total_penalty;

    return result;
}
