#include "knn.hpp"
std::vector<ll> euclidean(const Graph &g, std::string poi, ll k, double lat, double lon){
    std::vector<std::pair<double,ll>> list;
    for(auto n : g.nodes){
        if(std::find(n.pois.begin(),n.pois.end(),poi)!=n.pois.end()){
            double dist = std::sqrt((n.lat - lat)*(n.lat - lat) + (n.lon-lon)*(n.lon-lon));
            list.push_back({dist,n.id});
        }
    }
    std::sort(list.begin(),list.end());
    std::vector<ll>ans;
    int x = (list.size()<k)? list.size() : k;
    for(int i=0;i<x;i++){
        ans.push_back(list[i].second);
    }
    return ans;
}

ll nn(const Graph &g, double lat, double lon){
    std::vector<std::pair<double,ll>> list;
    for(auto n: g.nodes){
        double dist = std::sqrt((n.lat - lat)*(n.lat - lat) + (n.lon-lon)*(n.lon-lon));
        list.push_back({dist,n.id});
    }
    std::sort(list.begin(),list.end());
    return list[0].second;
}

std::vector<ll>shortest_path(const Graph &g, std::string poi, ll k, double lat, double lon){
    ll nn_id = nn(g,lat,lon);
    std::vector<double> sp(g.V,__LONG_LONG_MAX__);
    sp[nn_id]=0;
    std::vector<bool> visit(g.V,false);
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    pq.push({0.0,nn_id});
    while(!pq.empty()){
        auto x = pq.top();
        pq.pop();
        for(auto y: g.adjList[x.second]){
            try{
                auto &edge = g.edges.at(y.second);
                if(!edge.disable && !visit[y.first]){ // Check other things like oneway or disabled if applicable
                    if(edge.len + sp[x.second] < sp[y.first]){ // wouldn't y.first be a node id? we need to add edge weight 
                        sp[y.first] = edge.len + sp[x.second]; // we are storing edge id in second position of each pair
                        pq.push({sp[y.first], y.first});
                    }
                }
            }
            catch(const std::out_of_range& ex){
                std::cerr<<"Requested edge is not found in the graph"<<std::endl;
            }
        }
        visit[x.second]=true;
    }

    std::vector<std::pair<double,ll>>ans;
    for(int i=0;i<g.V;i++){
        auto n = g.nodes[i];
        if(sp[i] != __LONG_LONG_MAX__ && std::find(n.pois.begin(), n.pois.end(),poi) != n.pois.end()){
            ans.push_back({sp[i],n.id});
        }
    }
    std::sort(ans.begin(),ans.end());

    std::vector<ll>final;
    int x = (ans.size()<k)? ans.size() : k;
    for(int i=0;i<x;i++){
        final.push_back(ans[i].second);
    }
    return final;
}