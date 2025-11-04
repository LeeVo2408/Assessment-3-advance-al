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

//helper functions

//hash function for pair<int, int>
struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

//make an ordered pair (smaller value first)
std::pair<int, int> makeOrderedPair(int a, int b);

#endif      //KKT_HPP_

//nothing