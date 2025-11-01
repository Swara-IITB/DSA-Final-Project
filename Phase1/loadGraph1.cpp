#include "loadGraph1.hpp"
using json = nlohmann::json;

Graph loadGraph_parse(const std::string &filename){
    std::ifstream file(filename);
    if(!file.is_open()) throw std::runtime_error("unable to open" + filename);
    json graphdata;
    file >> graphdata;

    ll v = graphdata["meta"]["nodes"];
    Graph g(v);

    for(auto& n : graphdata["nodes"]){
        g.add_node(n["id"],n["lat"],n["lon"],n["pois"]);// what if they don't give v nodes?
    }
    for(auto& e : graphdata["edges"]){
        g.add_edge(e["id"],e["u"],e["v"],e["length"],e["average_time"],e["oneway"],e["roadtype"])
        if(!e["speed_profile"].empty()) g.edges[e["id"]].sp_profile = e["speed_profile"];
    }
    return g;
}