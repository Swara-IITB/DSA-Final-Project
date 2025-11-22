#include "graph3.hpp"
#include "DeliveryScheduling.hpp"
#include "minimum_penalty_next_scheduler.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
using json = nlohmann::json;

json process_query_phase3(Graph& g , json query);
json phase3_ps(Graph& g, json query);
json phase3_ps_our(Graph& g, json query);