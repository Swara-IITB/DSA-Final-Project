#include "queryHandler3.hpp"
using json = nlohmann::json;
typedef long long ll;
json process_query_phase3(Graph& g , json query){
    try {
           if(query.contains("delivery_boy")){
                return phase3_ps_our(g,query);
           } else {
                return phase3_ps(g,query);
           }
        }
        catch (const std::exception &e) {
            json err;
            err["error"] = e.what();
            return err;
        }
}

json phase3_ps(Graph& g, json query){
    ll depot = query["fleet"]["depot_node"].get<ll>();
    ll num_delievery_guys = query["fleet"]["num_delivery_guys"].get<ll>();
    std::vector<Orders> order_list;
    for( auto& order : query["orders"] ){
    ll order_id = order["order_id"].get<ll>();
    ll pickup = order["pickup"].get<ll>();
    ll dropoff = order["dropoff"].get<ll>();
    Orders Or = {order_id,pickup,dropoff};
    order_list.push_back(Or);
    }
    std::vector<Assignments> ans = clustering_method(g,num_delievery_guys,depot,order_list);
    json result;
    std::vector<json> as;
    double t =0;
    for(auto& a : ans){
    json j;
    j["driver_id"] = json(a.driver_id);
    j["route"] = json(a.route);
    j["order_ids"] = json(a.order_ids);
    t = std::max(t,time(g,a.route));
    as.push_back(j);
    }
    result["assignments"] = json(as);
    result["metrics"]["total_delivery_time_s"] = json(t);

    return result;
}

json phase3_ps_our(Graph& g, json query){
    ll depot = query["depot_node"].get<ll>();
    std::vector<ll> order_ids, pickups, dropoffs, multipliers;
    std::vector<double> max_times;
    for(auto& order : query["delivery_boy"]["orders"]){
        ll o_id, pickup, dropoff, penalty_multiplier;
        double max_time;
        o_id = order["order_id"].get<ll>();
        pickup = order["pickup"].get<ll>();
        dropoff = order["dropoff"].get<ll>();
        penalty_multiplier = order["penalty_multiplier"].get<ll>();
        max_time = order["max_time"].get<double>();
        order_ids.push_back(o_id);
        pickups.push_back(pickup);
        dropoffs.push_back(dropoff);
        multipliers.push_back(penalty_multiplier);
        max_times.push_back(max_time);
    }
    auto ans = run_mpn_scheduler(g, depot, order_ids, pickups, dropoffs, max_times, multipliers);
    json result;
    result["path"] = ans.route;
    result["total_penalty"] = ans.total_penalty;
    return result;
}
// taking max time assuming everyone starts at the same time