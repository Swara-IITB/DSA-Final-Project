#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include "graph.hpp"
typedef long long ll;

double eucl_dist_nodes(Graph& g,ll x, ll y); // takes node ids
double eucl_dist_latlon(double x1, double y1, double x2, double y2);// takes lat lons
// h() = w*eucl_dist(s,t)
// w>1
ll unidir_approx_Astar_meet(Graph& g, ll src, ll target, int queries_left, double time_left_ms, double acc_error);

// w>1 , stop when they meet or time taken is too long
ll bidir_approx_Astar_meet(Graph& g, ll src, ll target, int queries_left, double time_left_ms, double acc_error);

// w>1 , stop when eucl_dist b/w the two dir is < xm*avg_edge_len
// inc xm will lower the accuracy but might take less time and vice versa
ll bidir_approx_Astar_range(Graph& g, ll src, ll target, int queries_left, double time_left_ms, double acc_error);

// need to test which one works better and with what parameters