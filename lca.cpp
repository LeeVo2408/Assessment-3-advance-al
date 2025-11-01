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
double LCA::maxEdgeWeight(int u, int v) {}


//helper functions

void LCA::dfs(int u, int p) {
    for (auto e : adjList[u]) {
        int v;
        if (u == e.v1 ) v = e.v2; else v = e.v1;
        if (v == p) continue; //skip parent
        if (level[v] != -1) continue; //already visited
        parent[v] = u;
        up[v][0] = u; //1st ancestor of v
        maxWeight[v][0] = e.weight;
        level[v] = level[u] + 1;
        dfs(v, u);
    }
}

void LCA::preprocessing() {
    //initialise root(s)
    for (int root  = 0; root < n; ++root) {
        if (level[root] != -1) continue;
        parent[root] = -1;
        up[root][0] = -1;
        level[root] = 0;
        maxWeight[root][0] = 0.0;
        dfs(root, -1); //set parent[] and level[]
    }
    //build binary lifting tables
    for (int i = 1; i < log; ++i) {
        for (int v = 0; v < n; ++v) {
            if (up[v][i - 1] == -1) continue; //no 2^(i-1) ancestor
            up[v][i] = up[up[v][i - 1]][i - 1];
            maxWeight[v][i] = std::max(maxWeight[v][i - 1], maxWeight[up[v][i - 1]][i - 1]);
        }
    }
}