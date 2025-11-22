#pragma once
#include <queue>
#include <cmath>
#include "../../Phase-1/graph.hpp"
#include "../../Phase-1/loadGraph1.hpp"
double eucl_dist_test(Graph& g, ll x, ll y);
double ogAstar(Graph& g, ll src, ll target);
double approxAstarMeet(Graph& g, ll src, ll target, double w = 1.5);
double bidirOGAstar(Graph& g, ll src, ll target);
double bidirApproxAstar(Graph& g, ll src, ll target, double w = 1.5);