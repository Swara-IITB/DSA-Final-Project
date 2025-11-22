#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include "graph.hpp"

Graph loadGraph_parse(const std::string &filename);