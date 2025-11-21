#include "queryHandler2.hpp"
using json = nlohmann::json;
typedef long long ll;

json process_k_shortest_paths(Graph& g, json query){
    ll source = query["source"].get<ll>();
    ll target = query["target"].get<ll>();
    ll k = query["k"].get<ll>();
    std::vector<std::pair<std::vector<ll>, double>> ans = exact_ksp(g, source, target, k);
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

json process_approx_shortest_path(Graph& g, json query, std::chrono::time_point<std::chrono::high_resolution_clock> start_time){
    json result;
    result["id"] = query["id"];
    std::vector<json> distances;
    int q_left = query["queries"].size();
    double t_budget = query["time_budget_ms"].get<double>() ;
    double err = query["acceptable_error_pct"].get<double>()/100;
    for (auto &q : query["queries"]){
        json r;
        ll s = q["source"].get<ll>(), t = q["target"].get<ll>();
        r["source"] = json(s);
        r["target"] = json(t);
        auto now = std::chrono::high_resolution_clock::now();
        r["approx_shortest_distance"] = json(unidir_approx_Astar_meet(g,s,t,2,q_left,std::chrono::duration<double, std::milli>(now - start_time).count(),err));
        q_left--;                                                         //w to be decided along with the algo
        distances.push_back(r);
    }
    result["distances"] = json(distances);
    return result;
}


json process_query_phase2(Graph& g, json query, std::chrono::time_point<std::chrono::high_resolution_clock> start_time){
    try {
            std::string type = query["type"];
            if (type == "k_shortest_paths")
                return process_k_shortest_paths(g, query);
            else if (type == "k_shortest_paths_heuristic")
                return process_k_shortest_paths_heuristic(g, query);
            else if (type == "approx_shortest_path")
                return process_approx_shortest_path(g, query, start_time);
            else
                return {{"id", query["id"]}, {"error", "unknown query type"}};
        }
        catch (const std::exception &e) {
            return {{"id", query["id"]}, {"error", e.what()}};
        }
}
