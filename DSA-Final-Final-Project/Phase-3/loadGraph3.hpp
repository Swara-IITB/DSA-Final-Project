#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include "graph3.hpp"

Graph loadGraph_parse3(const std::string &filename);