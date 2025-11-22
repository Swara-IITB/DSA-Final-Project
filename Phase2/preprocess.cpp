#include "preprocess.hpp"
typedef long long ll;
std::vector<double> dijkstra( Graph &g, int src) {
    int n = g.V;
    std::vector<double> dist(n, 1e18);
    dist[src] = 0;
    std::priority_queue<std::pair<double,ll>, std::vector<std::pair<double,ll>>, std::greater<std::pair<double,ll>>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto &[v, w] : g.adjList[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

std::vector<int> selectLandmarks(Graph &g, int k) {
    int n = g.V;
    std::vector<int> landmarks;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n-1);
    int first = dis(gen);
    landmarks.push_back(first);
    std::vector<double> minDist(n, 1e18);
    while ((int)landmarks.size() < k) {
        int last = landmarks.back();
        std::vector<double> distLast = dijkstra(g, last);
        for (int i = 0; i < n; i++) {
            minDist[i] = std::min(minDist[i], distLast[i]);
        }
        int nextLandmark = std::max_element(minDist.begin(), minDist.end()) - minDist.begin();
        landmarks.push_back(nextLandmark);
    }
    return landmarks;
}

std::vector<std::vector<double>> preprocess(Graph& g){
    ll k;
    if(g.V>100) k=50;
    else k = g.V;
    std::vector<int> landmarks = selectLandmarks(g, k);
    std::vector<std::vector<double>> landmarkDist(k, std::vector<double>(g.V, 1e18));
    for (int i = 0; i < k; i++) {
        landmarkDist[i] = dijkstra(g, landmarks[i]);
    }
    return landmarkDist;
}
