#pragma once
#include <queue>
#include <cmath>
#include "../../Phase1/graph.hpp"
#include "../../Phase1/loadGraph1.hpp"
double eucl_dist_test(Graph& g, ll x, ll y);
double ogAstar(Graph& g, ll src, ll target);
double approxAstarMeet(Graph& g, ll src, ll target, double w = 1.5);
double bidirOGAstar(Graph& g, ll src, ll target);
double bidirApproxAstar(Graph& g, ll src, ll target, double w = 1.5);
double klandmarks(Graph& g, ll src, ll target, std::vector<std::vector<double>> landmarkDist);
double klandmarksupper(Graph& g, ll src, ll target, std::vector<std::vector<double>> landmarkDist);
double klandmarkslower(Graph& g, ll src, ll target, std::vector<std::vector<double>> landmarkDist);