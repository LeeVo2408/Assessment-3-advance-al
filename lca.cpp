#include "lca.hpp"
#include "kkt.hpp"
#include <vector>
#include "graph.hpp"
#include <limits>
#include <algorithm>

LCA::LCA(const Graph& F) {
    n = F.numVertices();
    adjList.assign(n, {});
    parent.assign(n, -1);
    level.assign(n, -1);
    log = 0;
    int p = 1; //p = 2^k
    while (p <= n) {
        p *= 2;
        ++log;
    }
    up.assign(n, std::vector<int>(log, -1)); //table size n x log, -1 means root node
    maxWeight.assign(n, std::vector<double>(log, 0.0));

    //construct adjacency list from graph F
    for (auto it = F.begin(); it != F.end(); ++it) {
        for (auto e : *it) {
            adjList[e.v1].insert(e);
            adjList[e.v2].insert(e);
        }
    }
    //build the tables
    preprocessing(); 
}