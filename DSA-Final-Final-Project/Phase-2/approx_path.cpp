#include "approx_path.hpp"
#include <algorithm>
#include <limits>

static const double INF = 1e18;

double landmarkApproxDist(const LandmarkOracle& oracle, ll u, ll v) {
    int L = oracle.L;
    double lower = 0.0;
    double upper = INF;

    for (int i = 0; i < L; ++i) {
        double df_u = oracle.dist_from[i][u];
        double df_v = oracle.dist_from[i][v];
        // lower bound candidate
        if (df_u < INF && df_v < INF) {
            double lb = std::fabs(df_u - df_v);
            if (lb > lower) lower = lb;
        }
        // upper bound candidate: dist(u -> Li) + dist(Li -> v)
        double du_to_li = oracle.dist_to[i][u]; // v->L computed via reverse Dijkstra run from L
        double li_to_v  = oracle.dist_from[i][v];
        if (du_to_li < INF && li_to_v < INF) {
            double ub = du_to_li + li_to_v;
            if (ub < upper) upper = ub;
        }
    }

    // if upper is INF (no path via any landmark), fall back to lower
    if(upper >=INF && lower == 0.0 && u!= v) return -1; 
    if (upper >= INF) return lower;

    // combine lower and upper — midpoint is simple and effective
    double approx = (lower + upper) * 0.5;
    if(u!=v && approx == 0.0) return -1;
    return approx;
}
