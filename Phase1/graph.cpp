#include "graph.hpp"
#include <iostream>
typedef long long ll;

Graph::Graph(ll v){
    this->V = v;//for you Priya<3
    adjMatrix.resize(V,std::vector<ll>(V)); // changed to ll instead of pair<ll,ll>
    adjList.resize(V);
    nodes.resize(V);
}
void Graph::add_node(ll id, double lat, double lon, std::vector<std::string> pois){
    Node n = {id,lat,lon,pois};
    nodes[id] = n;
}
void Graph::add_edge(ll id,ll u, ll v,double len,double avg_t, bool oneway,std::string roadtype,std::vector<ll> sp_profile={}){
    Edge e = {id, u, v, len, avg_t, oneway, roadtype,sp_profile};
    edges[id]=e;
    ll up = e.u, vp = e.v;
    adjMatrix[up][vp] = e.id;   
    adjList[up].push_back({vp,e.id}); // added adjList
    if(!oneway){ 
        adjMatrix[vp][up] = e.id;
        adjList[vp].push_back({up,e.id}); // added adjList
    }
}
void Graph::remove_edge(ll id){
    edges.at(id).disable=true;
}
void Graph::modify_edge_len(ll id, double len){
    edges.at(id).len = len;
    edges.at(id).disable=false;
}
void Graph::modify_edge_sp_profile(ll id, std::vector<ll> sp){ // replaced way with speed profile 
    edges.at(id).sp_profile = sp;                              // due to the updated problem statement
    edges.at(id).disable=false;
}
void Graph::modify_edge_avg_t(ll id, double t){
    edges.at(id).avg_t = t;
    edges.at(id).disable=false;
}
void Graph::modify_edge_roadtype(ll id, std::string road){
    edges.at(id).roadtype = road;
    edges.at(id).disable=false;
}

// We are assuming oneway var never changes once given