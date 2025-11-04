#include "graph.hpp"
#include <vector>
#include <utility>
#include <string>
typedef long long ll;

std::pair<ll,std::vector<ll>> shortestPath(Graph & g, ll src, ll target, string mode,const std::vector<ll> forbidNodes, const std::vector<string> forbidRoads );

std::pair<ll,std::vector<ll>> modeTime(Graph & g,const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::vector<string>& forbidRoads);
std::pair<ll,std::vector<ll>> modeDist(Graph & g,const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::vector<string>& forbidRoads);