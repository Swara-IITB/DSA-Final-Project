#include "graph.hpp"
#include <vector>
#include <utility>
#include<string>
#include<unordered_map>
typedef long long ll;

std::pair<double,std::vector<ll>> shortestPath(Graph & g, ll src, ll target, std::string mode, std::vector<ll> forbidNodes,  std::vector<std::string> forbidRoads );

std::pair<double,std::vector<ll>> modeTime(Graph & g,const ll& src,const ll& target, std::vector<ll>& forbidNodes,  std::unordered_map<std::string,ll>& mforbidRoads);
std::pair<double,std::vector<ll>> modeDist(Graph & g,const ll& src,const ll& target, std::vector<ll>& forbidNodes,  std::unordered_map<std::string,ll>& mforbidRoads);