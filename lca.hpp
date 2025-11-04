#ifndef LCA_HPP_ 
#define LCA_HPP_ 

#include "graph.hpp"
#include <vector>
#include <set>

//LCA class to find heaviest edge in path between two nodes in a tree
//Using Binary Lifting method instructed on GeeksforGeeks
//https://www.geeksforgeeks.org/dsa/query-to-find-the-maximum-and-minimum-weight-between-two-nodes-in-the-given-tree-using-lca/

//O(nlogn) preprocessing and O(logn) per query
class LCA {
    public:
    //default constructor
    LCA() = default;
    //construct adjacency list from the given tree F and preprocessing binary lifting tables
    explicit LCA(const Graph& F);

    //return max edge weight in path between u and v
    double maxEdgeWeight(int u, int v);

    private:
    int n;                              //number of nodes in the tree
    int log;                            //max power of 2
    std::vector<std::vector<Graph::Edge>> adjList {};
    std::vector<int> parent;                //parent of each node
    std::vector<int> level;                    //level of each node in the tree (depth)
    std::vector<std::vector<int>> up;           //up[i][j]: the 2^j-th ancestor of node i
    std::vector<std::vector<double>> maxWeight; //maxWeight[v][j]: max edge weight from v to its 2^j-th ancestor  
    std::vector<int> rootID; //store root (component) ID of node
    //helper functions
    //compute dfs traversal to set parent and level arrays
    void dfs(int u, int p, int r); //u: node, p: parent of u, r: root of u

    //preprocessing to fill lca and maxWeight tables
    void preprocessing();
};

#endif      // LCA_HPP_