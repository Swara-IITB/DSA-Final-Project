#include "shortestPath.hpp"
#include <iostream>
typedef long long ll;

std::pair<ll,std::vector<ll>> shortestPath(Graph & g,ll src, ll target, string mode,const std::vector<ll> forbidNodes, const std::vector<string> forbidRoads ){
    if(mode=="time") return modeTime(src,target,forbidNodes,forbidRoads);
    if(mode=="distance") return modeDist(src,target,forbidNodes,forbidRoads);
    // what if mode is not given?
    std::cerr << "mode not specified" << std::endl;
    return std::make_pair(0,{});
}
std::pair<ll,std::vector<ll>> modeTime(Graph & g,const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::vector<string>& forbidRoads){
    
}
std::pair<ll,std::vector<ll>> modeDist(Graph & g,const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::vector<string>& forbidRoads){

}
