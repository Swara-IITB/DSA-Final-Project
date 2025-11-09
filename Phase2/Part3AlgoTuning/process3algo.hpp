#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
using json = nlohmann::json;
typedef long long ll;

json process_3algo_approx1(Graph& g,json query,std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

json process_3algo_approx2(Graph& g,json query,std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

json process_3algo_approx3(Graph& g,json query,std::chrono::time_point<std::chrono::high_resolution_clock> start_time);