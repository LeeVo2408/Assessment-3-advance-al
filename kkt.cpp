#include "union_find.hpp"
#include "graph.hpp"
#include <unordered_map>
#include "kkt.hpp"

//Boruvka step to return the chosen edges and the connected components (supernode) for next round
std::pair<std::vector<Graph::Edge>, Graph> boruvkaStep(const Graph& G) {
    int n = G.numVertices();
    UnionFind UF(n);
    std::vector<Graph::Edge> cheapest(n);
    std::vector<bool> hasCheapest(n, false);
    for (int v = 0; v < n; ++v) {
        for (auto e : *G.neighbours(v)) {
            if (v != e.v1) continue;                        //avoid duplicate edge
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
}

Graph kktMST(const Graph& G) {

}
