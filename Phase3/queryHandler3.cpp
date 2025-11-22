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
    ll num_delievery_guys = query["fleet"]["num_delievery_guys"].get<ll>();
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

}
// taking max time assuming everyone starts at the same time