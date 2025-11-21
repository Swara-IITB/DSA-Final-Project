#pragma once
#include "graph3.hpp"
#include<queue>
#include<limits>
#include<algorithm>

struct Assignments{
    ll driver_id;
    std::vector<ll> route;
    std::vector<ll> order_ids;
};

struct Orders{
    ll order_id;
    ll pickup;
    ll drop;
};

std::vector<Assignments> scheduling(const Graph& g,ll num_delivery_guys, ll depot_node, std::vector<Orders>);