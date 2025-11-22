#include "graph3.hpp"
#include "DeliveryScheduling.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
using json = nlohmann::json;

json process_query_phase3(Graph& g , json query);