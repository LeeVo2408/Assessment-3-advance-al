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
//from github repo
//https://gist.github.com/VladimirReshetnikov/ac9bcabc652dcbeaf83a3f1328a1099b
struct pairhash final {
    template<class X, class Y>
    std::size_t operator()(const std::pair<X, Y>& p) const noexcept {
        uintmax_t hash = std::hash<X>{}(p.first);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<Y>{}(p.second);
        return std::hash<uintmax_t>{}(hash);
    }
};
//make an ordered pair (smaller value first)
std::pair<int, int> makeOrderedPair(int a, int b);

#endif      //KKT_HPP_

