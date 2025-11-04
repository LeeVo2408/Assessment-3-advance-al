#ifndef KKT_HPP_
#define KKT_HPP_

#include "graph.hpp"
#include <vector>
#include <utility>
#include <unordered_map>

//Boruvka step to return the chosen edges and the connected components (supernode) for next round
std::pair<std::vector<Graph::Edge>, Graph> boruvkaStep(const Graph& G);

//random function to choose edges with probability 1/2
bool randomChoice();

//KKT MST algorithm
Graph kktMST(const Graph& G);

#endif      //KKT_HPP_

