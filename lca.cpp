#include "lca.hpp"
#include "kkt.hpp"
#include <vector>
#include "graph.hpp"
#include <limits>
#include <algorithm>

const double NEG_INF = std::numeric_limits<double>::lowest();
const double INF = std::numeric_limits<double>::infinity();

LCA::LCA(const Graph& F) {
    n = F.numVertices();
    adjList.assign(n, {});
    parent.assign(n, -1);
    level.assign(n, -1);
    rootID.assign(n, -1);
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
            adjList[e.v1].push_back(e);
            adjList[e.v2].push_back(e);
        }
    }
    //build the tables
    preprocessing(); 
}

double LCA::maxEdgeWeight(int u, int v) {
    double maxW = NEG_INF;
    
    // Return INF for disconnected vertices (no path exists)
    if (rootID[u] != rootID[v]) return INF;
    
    // Same vertex - no edge weight on empty path
    if (u == v) return NEG_INF;

    //ensure u is deeper or equal to v
    if (level[u] < level[v]) {
        std::swap(u, v);
    }

    //lift u to the same level as v
    for (int i = log - 1; i >= 0; --i) {
        if (up[u][i] != -1 && level[up[u][i]] >= level[v]) {
            //max weight between v and its 2^i-th ancestor
            maxW = std::max(maxW, maxWeight[u][i]);
            u = up[u][i];
        }
    }

    if (u == v) {
        return maxW;
    }
    else {
        //lift both u and v to find closest node to LCA
        for (int i = log - 1; i >= 0; --i) {
            if (up[u][i] != -1 && up[u][i] != up[v][i]) {
                maxW = std::max(maxW, std::max(maxWeight[u][i], maxWeight[v][i]));
                u = up[u][i];
                v = up[v][i];
            }
        }
        //final step to reach LCA
        maxW = std::max(maxW, std::max(maxWeight[u][0], maxWeight[v][0]));
    }
    return maxW;
}


//helper functions

void LCA::dfs(int u, int p, int r) {
    for (auto e : adjList[u]) {
        int v;
        if (u == e.v1 ) v = e.v2; else v = e.v1;
        if (v == p) continue; //skip parent
        if (level[v] != -1) continue; //already visited
        parent[v] = u;
        up[v][0] = u; //1st ancestor of v
        maxWeight[v][0] = e.weight;
        level[v] = level[u] + 1;
        rootID[v] = r;  //update v's component
        dfs(v, u, r);
    }
}

void LCA::preprocessing() {
    //initialise root(s)
    for (int root  = 0; root < n; ++root) {
        if (level[root] != -1) continue;
        parent[root] = -1;
        up[root][0] = -1;
        level[root] = 0;
        maxWeight[root][0] = NEG_INF;
        rootID[root] = root;
        dfs(root, -1, root); //set parent[] and level[]
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
