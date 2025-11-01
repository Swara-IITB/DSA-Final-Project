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
    std::vector<ll> sp_profile;// can be missisng as well
    bool oneway;
    std::string roadtype;
    bool disable = false;
};

class Graph{
    public :
        std::vector<std::vector<std::pair<ll,ll>>> adj;
        ll V;
        std::vector<Node> nodes;
        std::map<ll,Edge> edges;
        Graph(ll v);
        ~Graph();
        void add_node(ll id, double lat, double lon, std::vector<std::string> pois);
        void add_edge(ll id,ll u, ll v,double len,double avg_t, bool oneway,std::string roadtype,std::vector<ll> sp_profile={});
        void remove_edge(ll id);
        void modify_edge_len(ll id, double len);
        void modify_edge_way(ll id, bool way);
        void modify_edge_avg_t(ll id, double t);
        void modify_edge_roadtype(ll id, std::string road);

};