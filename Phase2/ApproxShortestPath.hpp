#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <queue>
#include <cmath>
#include "../Phase1/graph.hpp"
typedef long long ll;

double eucl_dist_nodes(Graph& g,ll x, ll y); // takes node ids
double eucl_dist_latlon(double x1, double y1, double x2, double y2);// takes lat lons
// h() = w*eucl_dist(s,t)
// w>1
double unidir_approx_Astar_meet(Graph& g, ll src, ll target,double w, int queries_left, double time_left_ms, double acc_error,double tick_multi=0.9);

// w>1 , stop when they meet or time taken is too long
double bidir_approx_Astar_meet(Graph& g, ll src, ll target,double w, int queries_left, double time_left_ms, double acc_error,double tick_multi=0.9);

// w>1 , stop when eucl_dist b/w the two dir is < xm*avg_edge_len
// inc xm will lower the accuracy but might take less time and vice versa
double bidir_approx_Astar_range(Graph& g, ll src, ll target,double w, int queries_left, double time_left_ms, double acc_error,double tick_multi=0.9);

// need to test which one works better and with what parameters