#include <iostream>
#include <fstream>
#include "Phase1/Graph.hpp"
#include "Phase1/loadGraph1.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]){
    if(argc<3){
        std::cerr << "Usage: ./phase1 <query number> <graph.json>";
        return 1;
    }
    int phase = std::stoi(argv[1]);
    std::string filename = argv[2];

    Graph g = loadGraph_parse(filename); // need to handle errors


    return 0;
}