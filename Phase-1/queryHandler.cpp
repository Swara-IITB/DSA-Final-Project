#include "queryHandler.hpp"
using json = nlohmann::json;
typedef long long ll;

json process_query_phase1(Graph& g, json query){
        try {
            std::string type = query["type"];
            if (type == "remove_edge")
                return process_remove_edge(g, query);
            else if (type == "modify_edge")
                return process_modify_edge(g, query);
            else if (type == "shortest_path")
                return process_shortest_path(g, query);
            else if (type == "knn")
                return process_knn(g, query);
            else
                return {{"id", query["id"]}, {"error", "unknown query type"}};
        }
        catch (const std::exception &e) {
            return {{"id", query["id"]}, {"error", e.what()}};
        }
}

json process_remove_edge(Graph& g, json query){
    if(query.contains("edge_id")){
        bool done = g.remove_edge(query["edge_id"].get<ll>());
        if(done) return {{"id", query["id"]}, {"done", json(true)}};
        else return {{"id", query["id"]}, {"done", json(false)}};
    } else return {{"id", query["id"]}, {"done", json(false)}};
}

json process_modify_edge(Graph& g,json query){
    if(query.contains("edge_id")){
        if(query.contains("patch")){
            if(query["patch"].contains("length")){
                g.modify_edge_len(query["edge_id"].get<ll>(),query["patch"]["length"].get<double>());
            }
            if(query["patch"].contains("average_time")){
                g.modify_edge_avg_t(query["edge_id"].get<ll>(),query["patch"]["average_time"].get<double>());
            }
            if(query["patch"].contains("speed_profile")){
                g.modify_edge_sp_profile(query["edge_id"].get<ll>(),query["patch"]["speed_profile"].get<std::vector<double>>());
            }
            if(query["patch"].contains("road_type")){
                g.modify_edge_roadtype(query["edge_id"].get<ll>(),query["patch"]["road_type"].get<std::string>());
            }
        } 
        if(g.edges.find(query["edge_id"])!=g.edges.end()) g.edges.at(query["edge_id"].get<ll>()).disable=false;    
        return {{"id", query["id"]}, {"done", json(true)}};
    } else return {{"id", query["id"]}, {"done", json(false)}};
}

json process_shortest_path(Graph& g,json query){
    ll src = query["source"].get<ll>(), target = query["target"].get<ll>();
    std::string mode = query["mode"];
    double min_dist_time=0.0;
    std::vector<ll> path={};
    if(query.contains("constraints")){
        std::vector<ll> fn = {};
        std::vector<std::string> fr = {};
        if(query["constraints"].contains("forbidden_nodes")) fn = query["constraints"]["forbidden_nodes"].get<std::vector<ll>>();
        if(query["constraints"].contains("forbidden_road_types")) fr = query["constraints"]["forbidden_road_types"].get<std::vector<std::string>>();
        auto[x,y]=shortestPath(g,src,target,mode,fn,fr);
        min_dist_time=x;
        path = y;
    } else {
        auto[x,y] = shortestPath(g,src,target,mode,{},{});
        min_dist_time=x;
        path = y;
    }
    if(path.empty()) return {{"id", query["id"]}, {"possible", json(false)}};
    json result;
    result["id"] = query["id"];
    result["possible"] = true;
    result["minimum_time/minimum_distance"] = min_dist_time;
    result["path"] = path;
    return result;
}

json process_knn(Graph& g,json query){
    std::string poi=query["poi"];
    double lat = query["query_point"]["lat"].get<double>(), lon = query["query_point"]["lon"].get<double>();
    ll k = query["k"].get<ll>();
    std::string metric = query["metric"];
    std::vector<ll> nodes={};
    if(metric=="shortest_path"){
        nodes = shortest_path(g,poi,k,lat,lon);
    } else if(metric=="euclidean"){
        nodes = euclidean(g,poi,k,lat,lon);
    } else return {{"id",query["id"]},{"error","metric unknown"}};
    json result;
    result["id"] = query["id"];
    result["nodes"] = nodes;
    return result;
}