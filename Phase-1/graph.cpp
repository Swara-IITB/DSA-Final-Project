#include "graph.hpp"
#include <iostream>
typedef long long ll;

Graph::Graph(ll v){
    this->V = v;//for you Priya<3
    revAdjList.resize(V); 
    adjList.resize(V);
    nodes.resize(V);
}
void Graph::add_node(ll id, double lat, double lon, std::vector<std::string> pois){
    Node n = {id,lat,lon,pois};
    nodes[id] = n;
}
void Graph::add_edge(ll id,ll u, ll v,double len,double avg_t, bool oneway,std::string roadtype,std::vector<double> sp_profile){
    Edge e = {id, u, v, len, avg_t, oneway, roadtype,sp_profile};                                           // changed from ll to double
    edges[id]=e;
    ll up = e.u, vp = e.v;
    revAdjList[vp].push_back({up,e.id});   
    adjList[up].push_back({vp,e.id}); // added adjList
    if(!oneway){ 
        revAdjList[up].push_back({vp,e.id});
        adjList[vp].push_back({up,e.id}); // added adjList
    }
}
bool Graph::remove_edge(ll id){
    if(edges.find(id)!=edges.end()){ 
        if(edges.at(id).disable) return false;
        edges.at(id).disable=true;
        return true;
    } else return false;

}
bool Graph::modify_edge_len(ll id, double len){
    if(edges.find(id)!=edges.end()){
        edges.at(id).len = len; 
        // edges.at(id).disable=false;
        return true;
    } else return false;
}
bool Graph::modify_edge_sp_profile(ll id, std::vector<double> sp){ 
    if(edges.find(id)!=edges.end()){
        edges.at(id).sp_profile = sp; 
        // edges.at(id).disable=false;
        return true;
    } else return false;
}
bool Graph::modify_edge_avg_t(ll id, double t){
    if(edges.find(id)!=edges.end()){
        edges.at(id).avg_t = t;
        // edges.at(id).disable=false;
        return true;
    } else return false;
}
bool Graph::modify_edge_roadtype(ll id, std::string road){
    if(edges.find(id)!=edges.end()){
        edges.at(id).roadtype = road;
        // edges.at(id).disable=false; 
        return true;
    } else return false;
}

// We are assuming oneway var never changes once given