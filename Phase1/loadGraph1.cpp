#include "loadGraph1.hpp"
typedef long long ll;
using json = nlohmann::json;

Graph loadGraph_parse(const std::string &filename){
    std::ifstream file(filename);
    if(!file.is_open()) throw std::runtime_error("unable to open" + filename);
    json graphdata;
    file >> graphdata;

    ll v = graphdata["meta"]["nodes"];
    Graph g(v);

    for(auto& n : graphdata["nodes"]){
        if (n.contains("pois")) g.add_node(n["id"], n["lat"], n["lon"], n["pois"]);
        else g.add_node(n["id"], n["lat"], n["lon"], {});// pois can be missing
    }
    for(auto& e : graphdata["edges"]){
        if(e.contains("speed_profile")) g.add_edge(e["id"],e["u"],e["v"],e["length"],e["average_time"],e["oneway"],e["roadtype"],e["speed_profile"]);
        else g.add_edge(e["id"],e["u"],e["v"],e["length"],e["average_time"],e["oneway"],e["roadtype"],{});     
    }
    return g;
}