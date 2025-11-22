#include "queryHandler2.hpp"
using json = nlohmann::json;
typedef long long ll;

json process_k_shortest_paths(Graph& g, json query){
    ll source = query["source"].get<ll>();
    ll target = query["target"].get<ll>();
    ll k = query["k"].get<ll>();
    std::vector<std::pair<std::vector<ll>, double>> ans = getKBestPaths(g, source, target, k);
    json r;
    r["id"] = query["id"];
    r["paths"] = json::array();

    for (auto& p : ans) {
        json obj;
        obj["path"] = p.first;     
        obj["length"] = p.second;  

        r["paths"].push_back(obj);
    }
    return r;
}

json process_k_shortest_paths_heuristic(Graph& g, json query){
    ll source = query["source"].get<ll>();
    ll target = query["target"].get<ll>();
    ll k = query["k"].get<ll>();
    ll overlap_threshold = query["overlap_threshold"].get<ll>();
    std::vector<std::pair<std::vector<ll>, double>> ans = ksp(g, source, target, k, overlap_threshold);
    json r;
    r["id"] = query["id"];
    r["paths"] = json::array();

    for (auto& p : ans) {
        json obj;
        obj["path"] = p.first;     
        obj["length"] = p.second;  

        r["paths"].push_back(obj);
    }
    return r;
}

json process_approx_shortest_path(Graph& g, json query, std::chrono::time_point<std::chrono::high_resolution_clock> start_time, LandmarkOracle oracle){
    json result;
    result["id"] = query["id"];
    std::vector<json> distances;
    double t_budget = query["time_budget_ms"].get<double>() ;
    for (auto &q : query["queries"]){
        json r;
        ll s = q["source"].get<ll>(), t = q["target"].get<ll>();
        r["source"] = json(s);
        r["target"] = json(t);
        r["approx_shortest_distance"] = json(landmarkApproxDist(oracle,s,t));                                                         //w to be decided along with the algo
        distances.push_back(r);
        auto now = std::chrono::high_resolution_clock::now();
        if(std::chrono::duration<double, std::milli>(now - start_time).count()>t_budget*0.999) break;// might need to change
    }
    result["distances"] = json(distances);
    return result;
}


json process_query_phase2(Graph& g, json query, std::chrono::time_point<std::chrono::high_resolution_clock> start_time,LandmarkOracle oracle){
    try {
            std::string type = query["type"];
            if (type == "k_shortest_paths")
                return process_k_shortest_paths(g, query);
            else if (type == "k_shortest_paths_heuristic")
                return process_k_shortest_paths_heuristic(g, query);
            else if (type == "approx_shortest_path")
                return process_approx_shortest_path(g, query, start_time, oracle);
            else
                return {{"id", query["id"]}, {"error", "unknown query type"}};
        }
        catch (const std::exception &e) {
            return {{"id", query["id"]}, {"error", e.what()}};
        }
}
