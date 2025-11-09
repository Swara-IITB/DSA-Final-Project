#include "ApproxShortestPath.hpp"
typedef long long ll;

// use A* , h() will be eucledian*w , w>1 because approx paths
// use bidirectional search
// if eucl dist between frontiers is say within x% of m*avg_edge_len, x and m parameters to be tuned?
// x will depend on density of edges? and m will depend on the avg time left?   
double eucl_dist_nodes(const Graph& g, ll x, ll y){
    return std::sqrt((g.nodes[x].lat - g.nodes[y].lat)*(g.nodes[x].lat - g.nodes[y].lat) + (g.nodes[x].lon - g.nodes[y].lon)*(g.nodes[x].lon - g.nodes[y].lon));
}

double eucl_dist_latlon(double x1, double y1, double x2, double y2){
    return std::sqrt(( x1- x2)*(x1 - x2) + (y1-y2)*(y1-y2));
}

// normal weighted Astar, might take longer
double unidir_approx_Astar_meet(const Graph& g, ll src, ll target, double w, int queries_left, double time_left_ms, double acc_error, double tick_multi=0.9){
    auto start = std::chrono::high_resolution_clock::now();
    w = 1+w*acc_error;
    if(src==target) return 0.0;
    double tick = (time_left_ms/queries_left)*tick_multi; // check with the stopping condn * (1.1 - acc_error / 20.0);
    //Using A*
    std::vector<double> sp(g.V,1e18);
    sp[src]=0.0;
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    pq.push({0.0+w*eucl_dist_nodes(g,src,target),src});
    while(!pq.empty()){
        // stop if taken too long
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<double, std::milli>(now - start).count()>tick){
            return sp[pq.top().second]+eucl_dist_nodes(g,pq.top().second,target)*(1+acc_error);
        }
        if(pq.top().second==target){
            break;
        }
        auto x = pq.top();
        pq.pop();
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);        // try and catch block needed?
            if(ed.len + sp[x.second] < sp[y.first]){ 
                sp[y.first] = ed.len + sp[x.second]; 
                pq.push({sp[y.first]+w*eucl_dist_nodes(g,y.first,target), y.first});
            }
        }
    }
    return sp[target];

}


// enough time to explore more even after finding a potential path
double bidir_approx_Astar_meet( const Graph& g, ll src, ll target, double w, int queries_left, double time_left_ms, double acc_error, double tick_multi=0.9){
    auto start = std::chrono::high_resolution_clock::now();
    w = 1+w*acc_error;
    if(src==target) return 0.0;
    double tick = (time_left_ms/queries_left)*tick_multi;                             // if u want to try with a diff w for rev
    double ans = 1e18;
    //Using A*
    std::vector<double> sp(g.V,1e18), sprev(g.V,1e18);
    std::vector<bool> visited(g.V,false), visitedrev(g.V,false);
    sp[src]=0.0; sprev[target] = 0.0;
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq, pqrev;
    pq.push({0.0+w*eucl_dist_nodes(g,src,target),src});
    pqrev.push({0.0+w*eucl_dist_nodes(g,src,target),target});
    while(!pq.empty() || !pqrev.empty()){
        // stop if taken too long
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<double, std::milli>(now - start).count()>tick){
            if(ans<1e18) return ans;
            if(pq.empty() || pqrev.empty()) return ans;
            return sp[pq.top().second]+sprev[pqrev.top().second]+eucl_dist_nodes(g,pq.top().second,pqrev.top().second)*(1+acc_error);
        }                                                                                                  //* (1 + 0.5 * acc_error);
        if(ans<1e18 !pq.empty() && !pqrev.empty() && && sp[pq.top().second]+sprev[pqrev.top().second]>=ans){
            return ans;
        }
        auto x = pq.top();
        pq.pop();
        visited[x.second] = true;
        for(auto &y: g.adjList[x.second]){
            Edge &ed = g.edges.at(y.second);        // try and catch block needed?
            if(ed.len + sp[x.second] < sp[y.first]){ 
                sp[y.first] = ed.len + sp[x.second]; 
                pq.push({sp[y.first]+w*eucl_dist_nodes(g,y.first,target), y.first});
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
                pqrev.push({sprev[y.first]+w*eucl_dist_nodes(g,y.first,src), y.first});
                if(visited[y.first]) ans = std::min(ans,sp[y.first]+sprev[y.first]);
            }                          
        }
    }
    return ans;
}



// very less time left or given and high error percentage
double bidir_approx_Astar_range(Graph& g, ll src, ll target, double w,int queries_left, double time_left_ms, double acc_error, double tick_multi=0.9){
    auto start = std::chrono::high_resolution_clock::now();
    w = 1+w*acc_error; // if error more then time less hence we need to search more aggressively
    double tick = (time_left_ms/queries_left)*tick_multi;
    if(src==target) return 0.0;                             // if u want to try with a diff w for rev
    double ans = 1e18;
    //Using A*
    std::vector<double> sp(g.V,1e18), sprev(g.V,1e18);
    std::vector<bool> visited(g.V,false), visitedrev(g.V,false);
    sp[src]=0.0; sprev[target] = 0.0;
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq, pqrev;
    pq.push({0.0+w*eucl_dist_nodes(g,src,target),src});
    pqrev.push({0.0+w*eucl_dist_nodes(g,src,target),target});
    while(!pq.empty() || !pqrev.empty()){
        // stop if taken too long
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<double, std::milli>(now - start).count()>tick){
            if(pq.empty() || pqrev.empty()) return ans;
            return sp[pq.top().second]+sprev[pqrev.top().second]+eucl_dist_nodes(g,pq.top().second,pqrev.top().second)*(1+acc_error);
        }
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
                pq.push({sp[y.first]+w*eucl_dist_nodes(g,y.first,target), y.first});
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
                pqrev.push({sprev[y.first]+w*eucl_dist_nodes(g,y.first,src), y.first});
                if(visited[y.first]){ 
                    ans = std::min(ans,sp[y.first]+sprev[y.first]);
                    return ans;
                }
            }                          
        }
    }
    return ans;
}

// Assumption , a path exists for sure between src and target
// testing metrics
// processing time, ratio of correct queries and maybe MSE
// based on time budget, error percentage , chosen algo and chosen w