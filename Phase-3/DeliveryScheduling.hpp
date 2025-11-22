#pragma once
#include "graph3.hpp"
#include<queue>
#include<limits>
#include<algorithm>
#include <stdexcept>
#include <iostream>


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

double route_cost(const Graph& g, const std::vector<ll>& stops);
double time(const Graph& g, std::vector<ll>path);
double distance(const Graph& g, std::vector<ll>path);
std::vector<ll> shortest_path(const Graph& g, ll pickup, ll drop);
int nearestCenter_byPath(const Graph& g, ll pickupNode,const std::vector<ll>& centers);
std::vector<std::vector<Orders>> cluster_by_shortest_path(const Graph &g, std::vector<Orders>& ordersList, int K);
std::vector<ll> best_insertion(const Graph& g,const std::vector<ll>& R,ll P, ll D,double &deltaOut);
Assignments build_route_regret(const Graph& g,ll depot, std::vector<Orders> orders,int driver_id);
std::vector<Assignments> clustering_method(const Graph& g,ll num_delivery_guys, ll depot_node, std::vector<Orders> ordersList);