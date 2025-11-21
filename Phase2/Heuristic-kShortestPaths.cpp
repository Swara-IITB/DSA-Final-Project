#include "Heuristic-kShortestPaths.hpp"
std::pair<std::vector<ll>,double> dijsktra(const Graph& g, ll source, ll target, std::vector<ll> &timesUsed){
    std::vector<ll> ans;
    std::vector<bool> visit(g.V,false);
    std::vector<std::pair<ll,ll>>parent(g.V,{-1,-1});
    std::vector<double> sp(g.V, std::numeric_limits<double>::infinity());
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    sp[source]=0;
    pq.push({0.0,source});

    while(!pq.empty()){
        auto x = pq.top();
        pq.pop();
        for(auto y: g.adjList[x.second]){
            try{
                auto &edge = g.edges.at(y.second);
                if(!edge.disable && !visit[y.first]){ 
                    if(edge.len + sp[x.second] < sp[y.first]){
                        sp[y.first] = edge.len + sp[x.second];
                        parent[y.first]={x.second,y.second};
                        pq.push({sp[y.first], y.first});
                    }
                }
            }
            catch(const std::out_of_range& ex){
                std::cerr<<"Requested edge is not found in the graph"<<std::endl;
            }
        }
        visit[x.second]=true;
    }

    if(parent[target].first== -1){return {ans,sp[target]};}
    else{
        ll u = target;
        while(parent[u].first!=-1){
            ans.push_back(u);
            ll e = parent[u].second;
            if (e >= 0 && e < (ll)timesUsed.size()) timesUsed[e]++;
            u = parent[u].first;  
        }                  
        ans.push_back(source);
    }
    std::reverse(ans.begin(),ans.end());
    return {ans,sp[target]};
}

std::vector<std::pair<std::vector<ll>, double>> ksp(const Graph& g, ll source, ll target, ll k, ll threshold){
    std::vector<std::pair<std::vector<ll>, double>> ans;
    std::vector<ll> timesUsed(g.edges.size(),0);
    while(k>0){
        if(k==1){
            auto path = dijsktra(g,source,target,timesUsed);
            if(path.first.empty()){break;}
            ans.push_back(path);
        }
        else{
            auto copy = g;
            for (auto i=0; i<copy.edges.size();i++) {
                if(ans.size()==0){continue;}
                double perc = 0.0;
                if (!ans.empty()) { // added check because it was crashing with FPE
                    perc = 100.0 * timesUsed[i] / ans.size();
                }
                if (perc > 0) {
                    copy.edges[i].len *= 1 + (perc/100.0)*(threshold/100.0);
                }
            }
            auto path = dijsktra(copy,source,target,timesUsed);
            if(path.first.empty()){break;}
            ans.push_back(path);
        }
        k--;
    }

    std::sort(ans.begin(),ans.end(),[](const auto &a, const auto &b){return a.second < b.second;});
    return ans;
}