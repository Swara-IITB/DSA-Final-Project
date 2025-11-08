#pragma once
#include <map>
#include <vector>
typedef long long ll;

struct Node{
    ll id;
    double lat;
    double lon;
    std::vector<std::string> pois;
};

struct Edge{
    ll id;
    ll u;
    ll v;
    double len;
    double avg_t;
    bool oneway;
    std::string roadtype;
    std::vector<double> sp_profile;// can be missisng as well
    bool disable=false; 
};

class Graph{
    public :
        std::vector<std::vector<ll>> adjMatrix;
        std::vector<std::vector<std::pair<ll,ll>>> adjList;
        ll V;
        std::vector<Node> nodes;
        std::map<ll,Edge> edges;
        Graph(ll v);
        void add_node(ll id, double lat, double lon, std::vector<std::string> pois);
        void add_edge(ll id,ll u, ll v,double len,double avg_t, bool oneway,std::string roadtype,std::vector<double> sp_profile={});
        void remove_edge(ll id);
        void modify_edge_len(ll id, double len);
        void modify_edge_avg_t(ll id, double t);
        void modify_edge_sp_profile(ll id, std::vector<double> sp);
        void modify_edge_roadtype(ll id, std::string road);
};