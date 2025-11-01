#include "graph.hpp"
#include <iostream>
typedef long long ll;

Graph::Graph(ll v){
    this->V = v;//for you Priya<3
    adj.resize(V,std::vector<std::pair<ll,ll>>(V));
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
    adj[up][vp].second = e.id;   
    if(!oneway) adj[vp][up].second = e.id;
}
void Graph::remove_edge(ll id){
    adj[edges.at(id).u][edges.at(id).v]=-1;// default for non existing edge
    adj[edges.at(id).v][edges.at(id).u]=-1;
    edges.erase(id);
}
void Graph::modify_edge_len(ll id, double len){
    edges.at(id).len = len;
}
void Graph::modify_edge_way(ll id, bool way){
    edges.at(id).oneway = way;
}
void Graph::modify_edge_avg_t(ll id, double t){
    edges.at(id).avg_t = t;
}
void Graph::modify_edge_roadtype(ll id, std::string road){
    edges.at(id).roadtype = road;
}
