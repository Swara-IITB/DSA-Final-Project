#include "shortestPath.hpp"
#include <iostream>
#include<cmath>
#include<queue>
#include<algorithm>
typedef long long ll;

std::pair<double,std::vector<ll>> shortestPath(Graph & g,ll src, ll target, string mode,const std::vector<ll> forbidNodes, const std::vector<string> forbidRoads ){
    std::unordered_map<std::string,ll> mforbidRoads;
    for(auto s : forbidRoads) mforbidRoads[s] = 1;
    if(mode=="time") return modeTime(g,src,target,forbidNodes,mforbidRoads);
    if(mode=="distance") return modeDist(g,src,target,forbidNodes,mforbidRoads);
    // what if mode is not given?
    std::cerr << "mode not specified" << std::endl;
    return std::make_pair(0,{});
}
std::pair<double,std::vector<ll>> modeTime(Graph & g,const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::unordered_map<string,ll>& mforbidRoads){
    if(src==target) return {0.0,{src}};
    double ans = 0.0;

    //Using Dijkstra's algorithm
    std::vector<double> st(g.V,1e18);
    st[src]=0.0;
    std::vector<bool> visited(g.V,false), forbidden(g.V,false);
    for(auto n : forbidNodes){
        forbidden[n] = true;
    }
    std::vector<ll> parent(g.V,-1);
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    pq.push({0.0,src});
    while(!pq.empty()){
        if(pq.top().second==target){
            ans = pq.top().first;
            break;
        }
        auto x = pq.top();
        pq.pop();
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);
            if(!forbidden[y.first] && mforbidRoads[ed.roadtype]!=1 && !ed.disable ){//new entry might be created in the map, but maybe it doesn't matter, check!
                if(!visited[y.first]){
                    if(ed.sp_profile.empty()){
                        if(ed.avg_t + st[x.second] < st[y.first]){ 
                            st[y.first] = ed.avg_t + st[x.second]; 
                            pq.push({st[y.first], y.first});
                            parent[y.first] = x.second;
                        } else continue;
                    } else {
                        double t = st[x.second]; // current time
                        double tmp = t, tlen=0.0;
                        ll idx = static_cast<ll>((tmp/(60.0*15.0)))% 96;
                        while(ed.len-tlen>ed.sp_profile[idx]*(((idx+1)*15.0*60.0)-tmp)){
                            tlen += ed.sp_profile[idx]*(((idx+1)*15.0*60.0)-tmp);
                            tmp = ((idx+1)%96)*15.0*60.0;
                            idx = (idx + 1) % 96 ;
                        }
                        tmp += (ed.len-tlen) / ed.sp_profile[idx];
                        if(tmp < st[y.first]){ 
                            st[y.first] = tmp ; 
                            pq.push({tmp, y.first});
                            parent[y.first] = x.second;
                        } else continue;
                    }
                    
                }
            } else continue;
        }
        visited[x.second]=true;
    }
    if(ans==0.0) return {0,{}}; // empty output means not possible
    std::vector<ll> path;
    //for (ll v = target; v != -1; v = parent[v]) path.push_back(v);
    path.push_back(target);
    auto tmp = target;
    while(parent[tmp]!=src){
        tmp=parent[tmp];
        path.push_back(tmp);
    }
    path.push_back(src);
    std::reverse(path);
    return {ans,path};
}

std::pair<double,std::vector<ll>> modeDist(Graph & g,const ll& src,const ll& target,const std::vector<ll>& forbidNodes, const std::unordered_map<string,ll>& mforbidRoads){
    if(src==target) return {0.0,{src}};
    double ans = 0.0;

    //Using Dijkstra's algorithm
    std::vector<double> sp(g.V,1e18);
    sp[src]=0.0;
    std::vector<bool> visited(g.V,false), forbidden(g.V,false);
    for(auto n : forbidNodes){
        forbidden[n] = true;
    }
    std::vector<ll> parent(g.V,-1);
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    pq.push({0.0,src});
    while(!pq.empty()){
        if(pq.top().second==target){
            ans = pq.top().first;
            break;
        }
        auto x = pq.top();
        pq.pop();
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);
            if(!forbidden[y.first] && mforbidRoads[ed.roadtype]!=1 && !ed.disable ){//new entry might be created in the map, but maybe it doesn't matter, check!
                if(!visited[y.first]){
                    if(ed.len + sp[x.second] < sp[y.first]){ 
                        sp[y.first] = ed.len + sp[x.second]; 
                        pq.push({sp[y.first], y.first});
                        parent[y.first] = x.second;
                    }
                }
            } else continue;
        }
        visited[x.second]=true;
    }
    if(ans==0.0) return {0,{}}; // empty output means not possible
    std::vector<ll> path;
    //for (ll v = target; v != -1; v = parent[v]) path.push_back(v);
    path.push_back(target);
    auto tmp = target; 
    while(parent[tmp]!=src){
        tmp=parent[tmp];
        path.push_back(tmp);
    }
    path.push_back(src);
    std::reverse(path);
    return {ans,path};
}



// Assumptions : 
// 1. src and target will never be in forbidden nodes
// 2. it is not a multigraph, i.e. there's <= 1 edge between 2 nodes and no self loops
// 3. assuming speed profile always has 96 slots , whenever speed profile is present

// Todo for modularity:-
// make a helper function for path reconstruction
//  use auto [ currd , node ] = pq.top();
// store 1e18 in a var , constexpr double INF = 1e18;