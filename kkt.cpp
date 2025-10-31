#include "union_find.hpp"
#include "graph.hpp"
#include <unordered_map>
#include "kkt.hpp"
#include <random>

//Boruvka step to return the chosen edges and the connected components (supernode) for next round
std::pair<std::vector<Graph::Edge>, Graph> boruvkaStep(const Graph& G) {
    int n = G.numVertices();
    UnionFind UF(n);
    std::vector<Graph::Edge> cheapest(n);
    std::vector<bool> hasCheapest(n, false);
    for (int u = 0; u < n; ++u) {
        for (auto e : *G.neighbours(u)) {
            if (u != e.v1) continue;                        //avoid duplicate edge
            int comp1 = UF.find(e.v1);
            int comp2 = UF.find(e.v2);

            if (comp1 == comp2) continue;                   //v1 and v2 are in same component

            //update cheapest edge for component root1
            if (!hasCheapest[comp1] || e.weight < cheapest[comp1].weight) {
                cheapest[comp1] = e;
                hasCheapest[comp1] = true;
            }
            //update cheapest edge for component root2
            if (!hasCheapest[comp2] || e.weight < cheapest[comp2].weight) {
                cheapest[comp2] = e;
                hasCheapest[comp2] = true;
            }
        }
    }
    std::vector<Graph::Edge> chosen;  //edges chosen in this Boruvka step (will be part of MST)
    for (int v = 0; v < n; ++v) {
        if (!hasCheapest[v]) continue;
        auto e = cheapest[v];
        int comp1 = UF.find(e.v1);
        int comp2 = UF.find(e.v2);

        if (comp1 == comp2) continue;

        chosen.push_back(e); //add the chosen edge
        UF.merge(comp1, comp2);
    }

    //construct the contract graph G1

    //vertices in the same component in UF are contracted into a supernode
    std::vector<int> vertexSuperNode(n);                //keep track of vertex's current supernode
    std::unordered_map<int, int> superNodes;            //map the component to new supernode id
    int compCount = 0;
    for (int v = 0; v < n; ++v) {
        int comp = UF.find(v);
        //if the component is not mapped yet
        if (superNodes.find(comp) == superNodes.end()) {
            superNodes[comp] = compCount;
            vertexSuperNode[v] = superNodes[comp];
            ++compCount;
        }
        else {
            vertexSuperNode[v] = superNodes[comp];
        }
    }
    //now add edges to the contracted graph
    Graph G1(compCount);
    for (int u = 0; u < n; ++u) {
        for (auto e : *G.neighbours(u)) {
            if (u != e.v1) continue;                    //avoid duplicate edge
            int su = vertexSuperNode[u];                //supernode of vertex u
            int sv = vertexSuperNode[e.v2];             //supernode of vertex v

            if (su == sv) continue;                     //both endpoints are in same supernode (delete self-loop)
            
            G1.addEdge({e.weight, su, sv});
        }
    }
    return {chosen, G1};
}