#include "DeliveryScheduling.hpp"

double time(const Graph& g, std::vector<ll>path){
    double ans=0; Edge edge;
    if(path.empty()){return 1e15;}
    for(ll i=0; i+1<path.size();i++){
        ll edgeid = g.adjMatrix[path[i]][path[i+1]];
<<<<<<< HEAD
        try{
            edge = g.edges.at(edgeid);
        }
        catch(const std::out_of_range& ex){
            ans+=0;
        }
=======
        try{edge = g.edges.at(edgeid);}
        catch(const std::out_of_range& ex){ans+=1e15;}
>>>>>>> 3953e379c73f9f4d354b1c0198c707c2fb4e74ed
        ans+= edge.avg_t;
    }
    return ans;
}

double distance(const Graph& g, std::vector<ll>path){
    double ans=0; Edge edge;
    for(ll i=0; i+1<path.size();i++){
        ll edgeid = g.adjMatrix[path[i]][path[i+1]];
        try{edge = g.edges.at(edgeid);}
        catch(const std::out_of_range& ex){ans+=1e15;}
        ans+= edge.len;
    }
    if(path.empty()){return 1e15;}
    return ans;
}

std::vector<ll> shortest_path(const Graph& g, ll pickup, ll drop){
    std::vector<ll> parent(g.V, -1);
    std::vector<double> dist(g.V, std::numeric_limits<double>::infinity());
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
     dist[pickup] = 0.0;
    pq.push({0.0, pickup});

    while (!pq.empty()) {
        auto x = pq.top();
        auto d = x.first;
        auto u = x.second;
        pq.pop();
        if (d > dist[u]) continue;
        if (u == drop) break;
        for (auto& pr : g.adjList[u]) {
            ll v = pr.first;
            ll eid = pr.second;
            auto it = g.edges.find(eid);
            if (it == g.edges.end()) continue;

            const Edge& e = it->second;
            if (e.disable) continue;

            double w = e.len;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    std::vector<ll> path;
    if (dist[drop] == std::numeric_limits<double>::infinity()) {
        return path;
    }

    for (ll cur = drop; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int nearestCenter_byPath(const Graph& g, ll pickupNode,const std::vector<ll>& centers){
    double best = 1e15;
    int idx = 0;
    for (int i = 0; i < centers.size(); i++) {
        auto x = shortest_path(g, pickupNode, centers[i]);
        double d = distance(g,x);
        if (d < best) { best = d; idx = i; }
    }
    return idx;
}

std::vector<std::vector<Orders>> cluster_by_shortest_path(const Graph &g, std::vector<Orders>& ordersList, int K){
    std::vector<ll> centers(K);
    for(int i=0; i<K;i++){
        centers[i] = ordersList[i].pickup;
    }
    std::vector<int> assign(ordersList.size(),0);
    for(int j=0;j<3;j++){
        for(int i=0;i<ordersList.size();i++){
            assign[i] = nearestCenter_byPath(g, ordersList[i].pickup, centers);
        }
        for (int k = 0; k < K; k++){
            std::vector<int> idxs;
            for (int i = 0; i < ordersList.size(); i++){
                if (assign[i] == k) idxs.push_back(i);
            }

            if (idxs.empty()) continue;
            double bestSum = 1e15;
            ll bestNode = ordersList[idxs[0]].pickup;

            for (int ii : idxs){
                ll candidate = ordersList[ii].pickup;
                double sum = 0;
                for (int jj : idxs) {
                    auto z = shortest_path(g, candidate, ordersList[jj].pickup);
                    sum+= distance(g, z);
                    if (sum > bestSum) break;
                }
                if (sum < bestSum) {
                    bestSum = sum;
                    bestNode = candidate;
                }
            }
            centers[k] = bestNode;
        }
    }
    std::vector<std::vector<Orders>> buckets(K);
    for (int i = 0; i < ordersList.size(); i++)
        buckets[assign[i]].push_back(ordersList[i]);

    return buckets;
}

std::vector<ll> best_insertion(const Graph& g,const std::vector<ll>& R,ll P, ll D,double &deltaOut){
    if (R.size() == 1) {
    std::vector<ll> tmp = R;
    tmp.push_back(P);
    tmp.push_back(D);
    deltaOut = time(g,shortest_path(g, R[0], P)) +
               time(g,shortest_path(g, P, D));
    return tmp;
}
    double best = 1e15;
    std::vector<ll> bestR;
    double oldCost = time(g, R);

    for (int pi = 1; pi <= R.size(); pi++) {
        for (int di = pi+1; di <= R.size()+1; di++) {
            std::vector<ll> tmp;
            tmp.reserve(R.size()+2);

            for (int i=0; i< R.size(); i++) {
                if (i == pi) tmp.push_back(P);
                if (i == di) tmp.push_back(D);
                tmp.push_back(R[i]);
            }
            if (di == R.size()+1) tmp.push_back(D);
            double newCost = time(g, tmp);
            double delta = newCost - oldCost;
            if (delta < best) {
                best = delta;
                bestR = tmp;
            }
        }
    }
    deltaOut = best;
    if (bestR.empty()) {
        std::vector<ll> tmp = R;
        tmp.push_back(P);
        tmp.push_back(D);
        deltaOut = 0;
        return tmp;
    }
    return bestR;
}

Assignments build_route_regret(const Graph& g,ll depot, std::vector<Orders> orders,int driver_id){
    Assignments A;
    A.driver_id = driver_id;

    int m = orders.size();
    std::vector<bool> used(m, false);
    std::vector<ll> R = {depot};

    for (int t = 0; t < m; t++) {
        double bestReg = -1;
        int pick = -1;
        std::vector<ll> bestRoute;

        for (int i = 0; i < m; i++) {
            if (used[i]) continue;

            ll P = orders[i].pickup;
            ll D = orders[i].drop;

            double b1 = 1e15, b2 = 1e15;
            for (int pi = 1; pi <= R.size(); pi++) {
                for (int di = pi+1; di <= R.size()+1; di++) {
                    double delta;
                    auto _ = best_insertion(g, R, P, D, delta);
                    if (delta < b1) { b2 = b1; b1 = delta; }
                    else if (delta < b2) b2 = delta;
                }
            }
            if (b1 > 1e14) {
                std::vector<ll> tmp = R;
                tmp.push_back(P);
                tmp.push_back(D);
                used[i] = true;
                A.order_ids.push_back(orders[i].order_id);
                R = tmp;
                goto continue_outer_loop;
            }
            double regret = b2 - b1;
            if (regret > bestReg) {
                bestReg = regret;
                pick = i;
            }
        }
        continue_outer_loop:;
        double delta;
        R = best_insertion(g, R, orders[pick].pickup, orders[pick].drop, delta);
        used[pick] = true;
        A.order_ids.push_back(orders[pick].order_id);
    }

    A.route = R;
    return A;
}

std::vector<Assignments> clustering_method(const Graph& g,ll num_delivery_guys, ll depot_node, std::vector<Orders> ordersList){
    // std::vector<std::vector<ll>> timeMatrix;
    // timeMatrix.resize(g.V,std::vector<ll>(g.V,-1));
    // for(auto o : ordersList){
    //     auto x = o.pickup;
    //     auto y = o.drop;
    //     auto z = shortest_path(g,x,y);
    //     auto w = shortest_path(g,depot_node,x);
    //     timeMatrix[x][y] = time(g,z);
    //     timeMatrix[depot_node][x] = time(g,w);
    // }

    auto buckets = cluster_by_shortest_path(g, ordersList, num_delivery_guys);
    std::vector<Assignments> ans;
    for (int d = 0; d < num_delivery_guys; d++) {
        if (buckets[d].empty()) {
            Assignments A;
            A.driver_id = d;
            A.route = { depot_node };
            A.order_ids = {};
            ans.push_back(A);
            continue;
        }

        Assignments A =
            build_route_regret(g, depot_node, buckets[d], d);
        ans.push_back(A);
    }
    return ans;
}





/* this block has another idea(incomplete)..... using deterministic method when there are almost no clusters and using cluster method otherwise

double pickup_disp(const Graph& g, std::vector<Orders>& ordersList){
    if(ordersList.empty()) return 0;
    double cx=0, cy=0;
    for(auto o: ordersList){
        cx+= g.nodes[o.pickup].lat;
        cy+= g.nodes[o.pickup].lon;
    }
    cx/=ordersList.size();
    cy/=ordersList.size();

    double sum=0.0;
    for(auto o: ordersList){
        double dx = g.nodes[o.pickup].lat - cx;
        double dy = g.nodes[o.pickup].lon - cy;
        sum += dx*dx + dy*dy;
    }
    return sum/ordersList.size();
}

std::vector<Assignments> choose(const Graph& g,ll num_delivery_guys,  ll depot_node, std::vector<Orders> ordersList){
    if(ordersList.size()==0 || num_delivery_guys<=1){
        return deterministic_method(g, num_delivery_guys, depot_node, ordersList);
    }

    double disp = pickup_disp(g, ordersList);
    if(disp < 0.00002){return deterministic_method(g, num_delivery_guys, depot_node, ordersList);}
    else if(disp > 0.0003){return clustering_method(g, num_delivery_guys, depot_node, ordersList);}

    return deterministic_method(g, num_delivery_guys, depot_node, ordersList);
}

double pathDeliveryTime(std::vector<std::vector<ll>>& timeMatrix, std::vector<ll> path, std::unordered_map<ll,ll>& dropMark){
    double T = 0;
    double sum = 0;
    for (int i=0; i+1<path.size(); i++){
        T += timeMatrix[path[i]][path[i+1]];
        if (dropMark.count(path[i+1])) sum += T;
    }
    return sum;
}

std::vector<Assignments> deterministic_method(const Graph& g,ll num_delivery_guys, ll depot_node, std::vector<Orders> ordersList){
    ////////////// We need to precompute these
    std::vector<std::vector<ll>> timeMatrix;
    timeMatrix.resize(g.V,std::vector<ll>(g.V,-1));
    for(auto o : ordersList){
        auto x = o.pickup;
        auto y = o.drop;
        auto z = shortest_path(g,x,y);
        auto w = shortest_path(g,depot_node,x);
        timeMatrix[x][y] = time(g,z);
        timeMatrix[depot_node][x] = time(g,w);
    }
    ///////////////////////////////////////////

    std::vector<std::pair<Orders, double>> arr;
    for(auto o: ordersList){
        double c = timeMatrix[depot_node][o.pickup] + timeMatrix[o.pickup][o.drop];
        arr.push_back({o,c});
    }

    std::sort(arr.begin(), arr.end(),[](auto &a, auto &b){ return a.second < b.second;});
    std::vector<std::vector<ll>> routes(num_delivery_guys);
    std::vector<std::vector<ll>> driverOrders(num_delivery_guys);
    std::vector<std::unordered_map<ll,ll>> dropMark(num_delivery_guys);

    for (int d=0; d<num_delivery_guys; d++){
        routes[d].push_back(depot_node);
    }
    for(auto x: arr){
        auto o = x.first;
        ll p = o.pickup; ll d = o.drop;

        for (int d=0; d<num_delivery_guys; d++){
            double oldSum = pathDeliveryTime(timeMatrix, routes[d], dropMark[d]);
        }
    }
    /////////////////////////////NOT DONE YET////////////////////////////////////
}

*/