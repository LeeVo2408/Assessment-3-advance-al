#include "boruvka.hpp"
#include "union_find.hpp"
#include "graph.hpp"
#include <vector>

Graph boruvkaMST(const Graph& G) {
    int n = G.numVertices();
    Graph mst(n);

    if (n == 0) return mst;

    UnionFind UF(n);
    int edgeCount = 0;

    while (edgeCount < n -1) {
        std::vector<Graph::Edge> cheapest(n);
        std::vector<bool> hasCheapest(n, false); //if component's cheapest edge is set
        //iterate all edges
        for (int v = 0; v < n; ++v) {
            for (auto e : *G.neighbours(v)) {
                if (v != e.v1) continue; //avoid duplicate edge
                int root1 = UF.find(e.v1);
                int root2 = UF.find(e.v2);

                if (root1 == root2) continue; //v1 and v2 are in same component
            }
        }
    }
    return mst;
}