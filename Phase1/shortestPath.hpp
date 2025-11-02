#include "graph.hpp"
#include <vector>
#include <utility>
#include <string>
typedef long long ll;

std::pair<ll,std::vector<ll>> shortestPath(ll src, ll target, string mode,const std::vector<ll> forbidNodes, const std::vector<string> forbidRoads );

std::pair<ll,std::vector<ll>> modeTime(const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::vector<string>& forbidRoads);
std::pair<ll,std::vector<ll>> modeDist(const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::vector<string>& forbidRoads);