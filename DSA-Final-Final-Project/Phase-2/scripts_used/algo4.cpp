#include "algo4.hpp"

double eucl_dist_test(Graph& g, ll x, ll y){
    return std::sqrt((g.nodes[x].lat - g.nodes[y].lat)*(g.nodes[x].lat - g.nodes[y].lat) + (g.nodes[x].lon - g.nodes[y].lon)*(g.nodes[x].lon - g.nodes[y].lon));
}

double ogAstar(Graph& g, ll src, ll target){
    if(src==target) return 0.0;
    double ans = 0.0;
    //Using A* algorithm
    std::vector<double> sp(g.V,1e18);
    sp[src]=0.0;
    std::vector<bool> visited(g.V,false);
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    pq.push({0.0,src});
    while(!pq.empty()){
        if(pq.top().second==target){
            ans = pq.top().first;
            break;
        }
        auto x = pq.top();
        pq.pop();
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);
                if(!visited[y.first]){
                    if(ed.len + sp[x.second] < sp[y.first]){ 
                        sp[y.first] = ed.len + sp[x.second]; 
                        pq.push({sp[y.first], y.first});
                    }
                }
        }
        visited[x.second]=true;
    }
   return ans;
}

double approxAstarMeet(Graph& g, ll src, ll target, double w){
    if(src==target) return 0.0;
    //Using A*
    std::vector<double> sp(g.V,1e18);
    sp[src]=0.0;
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    pq.push({0.0+w*eucl_dist_test(g,src,target),src});
    while(!pq.empty()){
        if(pq.top().second==target){
            break;
        }
        auto x = pq.top();
        pq.pop();
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);        // try and catch block needed?
            if(ed.len + sp[x.second] < sp[y.first]){ 
                sp[y.first] = ed.len + sp[x.second]; 
                pq.push({sp[y.first]+w*eucl_dist_test(g,y.first,target), y.first});
            }
        }
    }
    return sp[target];
}

double bidirOGAstar(Graph& g, ll src, ll target){
    if(src==target) return 0.0;                           // if u want to try with a diff w for rev
    double ans = 1e18;
    //Using A*
    std::vector<double> sp(g.V,1e18), sprev(g.V,1e18);
    std::vector<bool> visited(g.V,false), visitedrev(g.V,false);
    sp[src]=0.0; sprev[target] = 0.0;
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq, pqrev;
    pq.push({0.0+eucl_dist_test(g,src,target),src});
    pqrev.push({0.0+eucl_dist_test(g,src,target),target});
    while(!pq.empty() || !pqrev.empty()){                                                                                                 //* (1 + 0.5 * acc_error);
        if(ans<1e18 && !pq.empty() && !pqrev.empty() && sp[pq.top().second]+sprev[pqrev.top().second]>=ans){
            return ans;
        }
        auto x = pq.top();
        pq.pop();
        visited[x.second] = true;
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);        // try and catch block needed?
            if(ed.len + sp[x.second] < sp[y.first]){ 
                sp[y.first] = ed.len + sp[x.second]; 
                pq.push({sp[y.first]+eucl_dist_test(g,y.first,target), y.first});
                if(visitedrev[y.first]) ans = std::min(ans,sp[y.first]+sprev[y.first]);
            }                         
        }
        if(ans<1e18 && !pq.empty() && !pqrev.empty() && sp[pq.top().second]+sprev[pqrev.top().second]>=ans){
            return ans;
        }
        auto z = pqrev.top();
        pqrev.pop();
        visited[z.second] = true;
        for(auto &y: g.revAdjList[z.second]){
            Edge &ed = g.edges.at(y.second);        // try and catch block needed?
            if(ed.len + sprev[z.second] < sprev[y.first]){ 
                sprev[y.first] = ed.len + sprev[z.second]; 
                pqrev.push({sprev[y.first]+eucl_dist_test(g,y.first,src), y.first});
                if(visited[y.first]) ans = std::min(ans,sp[y.first]+sprev[y.first]);
            }                          
        }
    }
    return ans;
}

double bidirApproxAstar(Graph& g, ll src, ll target, double w){
    if(src==target) return 0.0;                             // if u want to try with a diff w for rev
    double ans = 1e18;
    //Using A*
    std::vector<double> sp(g.V,1e18), sprev(g.V,1e18);
    std::vector<bool> visited(g.V,false), visitedrev(g.V,false);
    sp[src]=0.0; sprev[target] = 0.0;
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq, pqrev;
    pq.push({0.0+w*eucl_dist_test(g,src,target),src});
    pqrev.push({0.0+w*eucl_dist_test(g,src,target),target});
    while(!pq.empty() || !pqrev.empty()){
        if(ans<1e18 && !pq.empty() && !pqrev.empty() && sp[pq.top().second]+sprev[pqrev.top().second]>=ans){
            return ans;
        }
        auto x = pq.top();
        pq.pop();
        visited[x.second] = true;
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);        // try and catch block needed?
            if(ed.len + sp[x.second] < sp[y.first]){ 
                sp[y.first] = ed.len + sp[x.second]; 
                pq.push({sp[y.first]+w*eucl_dist_test(g,y.first,target), y.first});
                if(visitedrev[y.first]){ 
                    ans = std::min(ans,sp[y.first]+sprev[y.first]);
                    return ans;
                }
            }                         
        }
        if(ans<1e18 &&!pq.empty() && !pqrev.empty() && sp[pq.top().second]+sprev[pqrev.top().second]>=ans){
            return ans;
        }
        auto z = pqrev.top();
        pqrev.pop();
        visited[z.second] = true;
        for(auto &y: g.revAdjList[z.second]){
            Edge &ed = g.edges.at(y.second);        // try and catch block needed?
            if(ed.len + sprev[z.second] < sprev[y.first]){ 
                sprev[y.first] = ed.len + sprev[z.second]; 
                pqrev.push({sprev[y.first]+w*eucl_dist_test(g,y.first,src), y.first});
                if(visited[y.first]){ 
                    ans = std::min(ans,sp[y.first]+sprev[y.first]);
                    return ans;
                }
            }                          
        }
    }
    return ans;
}

double klandmarks(Graph& g, ll src, ll target, std::vector<std::vector<double>> landmarkDist){
    double lower = 0, upper = 1e18;
    ll k = landmarkDist.size();
    for (int i = 0; i < k; i++) {
        lower = std::max(lower, std::abs(landmarkDist[i][src] - landmarkDist[i][target]));
        upper = std::min(upper, landmarkDist[i][src] + landmarkDist[i][target]);
    }
    double approxDist = (lower + upper) / 2.0;
    return approxDist;
}
double klandmarkslower(Graph& g, ll src, ll target, std::vector<std::vector<double>> landmarkDist){
    double lower = 0, upper = 1e18;
    ll k = landmarkDist.size();
    for (int i = 0; i < k; i++) {
        lower = std::max(lower, std::abs(landmarkDist[i][src] - landmarkDist[i][target]));
        // upper = min(upper, landmarkDist[i][src] + landmarkDist[i][target]);
    }
    return lower;
}

double klandmarksupper(Graph& g, ll src, ll target, std::vector<std::vector<double>> landmarkDist){
    double lower = 0, upper = 1e18;
    ll k = landmarkDist.size();
    for (int i = 0; i < k; i++) {
        // lower = max(lower, abs(landmarkDist[i][src] - landmarkDist[i][target]));
        upper = std::min(upper, landmarkDist[i][src] + landmarkDist[i][target]);
    }
    return upper;
}
