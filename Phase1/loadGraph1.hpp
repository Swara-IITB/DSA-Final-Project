#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Phase-1/Graph.hpp"

Graph loadGraph_parse(const std::string &filename);