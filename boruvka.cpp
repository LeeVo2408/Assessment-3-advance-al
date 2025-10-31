#include "boruvka.hpp"
#include "union_find.hpp"
#include "graph.hpp"
#include <vector>

Graph boruvkaMST(const Graph& G) {
    int n = G.numVertices();
    Graph mst(n);

    if (n == 0) return mst;

    UnionFind UF(n);
    //while spanning tree is not completed
    while (UF.numberOfComponents() > 1) {
        std::vector<Graph::Edge> cheapest(n);
        std::vector<bool> hasCheapest(n, false); //if component's cheapest edge is set
        //iterate all edges
        for (int v = 0; v < n; ++v) {
            for (auto e : *G.neighbours(v)) {
                if (v != e.v1) continue;            //avoid duplicate edge
                int comp1 = UF.find(e.v1);
                int comp2 = UF.find(e.v2);

                if (comp1 == comp2) continue;       //v1 and v2 are in same component

                //update cheapest edge for component 1
                if (!hasCheapest[comp1] || e.weight < cheapest[comp1].weight) {
                    cheapest[comp1] = e;
                    hasCheapest[comp1] = true;
                }
                //update cheapest edge for component 2
                if (!hasCheapest[comp2] || e.weight < cheapest[comp2].weight) {
                    cheapest[comp2] = e;
                    hasCheapest[comp2] = true;
                }
            }
        }
        
        //merge the vertices of the cheapest edges and add the edges to the spanning tree 
        int mergedCount = 0;                //number of merges in this Boruvka round
        for (int v = 0; v < n; ++v) {
            if (!hasCheapest[v]) continue;
            auto e = cheapest[v];
            int comp1 = UF.find(e.v1);
            int comp2 = UF.find(e.v2);

            if (comp1 == comp2) continue;

            UF.merge(comp1, comp2);
            mst.addEdge(e);
            ++mergedCount;
        }
        if (mergedCount == 0) break; //no edges between 2 components left (disconnected)
    }
    return mst;
}