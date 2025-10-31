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
                if (v != e.v1) continue; //avoid duplicate edge
                int root1 = UF.find(e.v1);
                int root2 = UF.find(e.v2);

                if (root1 == root2) continue; //v1 and v2 are in same component

                //update cheapest edge for component root1
                if (!hasCheapest[root1] || e.weight < cheapest[root1].weight) {
                    cheapest[root1] = e;
                    hasCheapest[root1] = true;
                }
                //update cheapest edge for component root2
                if (!hasCheapest[root2] || e.weight < cheapest[root2].weight) {
                    cheapest[root2] = e;
                    hasCheapest[root2] = true;
                }
            }
        }
        //merge the vertices of the cheapest edges and add the edges to the spanning tree 
        int mergedCount = 0; //number of merges in this Boruvka round
        for (int v = 0; v < n; ++v) {
            if (!hasCheapest[v]) continue;
            auto e = cheapest[v];
            int root1 = UF.find(e.v1);
            int root2 = UF.find(e.v2);

            if (root1 == root2) continue;

            UF.merge(root1, root2);
            mst.addEdge(e);
            ++mergedCount;
        }
        if (mergedCount == 0) break; //no edges between 2 components left (disconnected)
    }
    return mst;
}