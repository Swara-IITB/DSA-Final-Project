#include "process3algo.hpp"

using json = nlohmann::json;
typedef long long ll;

json process_3algo_approx1(Graph& g,json query,std::chrono::time_point<std::chrono::high_resolution_clock> start_time,double w){
    json result;
    result["id"] = query["id"];
    std::vector<json> distances;
    int q_left = query["queries"].size();
    double t_budget = query["time_budget_ms"].get<double>() ;
    double err = query["acceptable_error_pct"].get<double>()/100;
    for (auto &q : query["queries"]){
        json r;
        ll s = q["source"].get<ll>(), t = q["target"].get<ll>();
        r["source"] = s;
        r["target"] = t;
        auto now = std::chrono::high_resolution_clock::now();
        r["approx_shortest_distance"] = unidir_approx_Astar_meet(g,s,t,w,q_left,std::chrono::duration<double, std::milli>(now - start_time).count(),err);
        q_left--;
        distances.push_back(r);
    }
    result["distances"] = distances;
    return result;
}

json process_3algo_approx2(Graph& g,json query,std::chrono::time_point<std::chrono::high_resolution_clock> start_time,double w){
    json result;
    result["id"] = query["id"];
    std::vector<json> distances;
    int q_left = query["queries"].size();
    double t_budget = query["time_budget_ms"].get<double>() ;
    double err = query["acceptable_error_pct"].get<double>()/100;
    for (auto &q : query["queries"]){
        json r;
        ll s = q["source"].get<ll>(), t = q["target"].get<ll>();
        r["source"] = s;
        r["target"] = t;
        auto now = std::chrono::high_resolution_clock::now();
        r["approx_shortest_distance"] = bidir_approx_Astar_meet(g,s,t,w,q_left,std::chrono::duration<double, std::milli>(now - start_time).count(),err);
        q_left--;
        distances.push_back(r);
    }
    result["distances"] = distances;
    return result;
}

json process_3algo_approx3(Graph& g,json query,std::chrono::time_point<std::chrono::high_resolution_clock> start_time, double w){
    json result;
    result["id"] = query["id"];
    std::vector<json> distances;
    int q_left = query["queries"].size();
    double t_budget = query["time_budget_ms"].get<double>() ;
    double err = query["acceptable_error_pct"].get<double>()/100;
    for (auto &q : query["queries"]){
        json r;
        ll s = q["source"].get<ll>(), t = q["target"].get<ll>();
        r["source"] = s;
        r["target"] = t;
        auto now = std::chrono::high_resolution_clock::now();
        r["approx_shortest_distance"] = bidir_approx_Astar_range(g,s,t,w,q_left,std::chrono::duration<double, std::milli>(now - start_time).count(),err);
        q_left--;
        distances.push_back(r);
    }
    result["distances"] = distances;
    return result;
}