#ifndef MPN_SCHEDULER_HPP
#define MPN_SCHEDULER_HPP

#include "graph.hpp"
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <cmath>
#include <algorithm>

typedef long long ll;

struct MPNResult
{
    std::vector<ll> route;
    std::unordered_map<ll, double> finish_time;
    std::unordered_map<ll, ll> penalty;
    ll total_penalty;
};

MPNResult run_mpn_scheduler(const Graph &g,
                            ll depot_node,
                            const std::vector<ll> &order_ids,
                            const std::vector<ll> &pickups,
                            const std::vector<ll> &dropoffs,
                            const std::vector<double> &max_times,
                            const std::vector<ll> &multipliers);

#endif
