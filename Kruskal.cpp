
#include "graph.hpp"
#include <vector>
#include <set>
#include "union_find.hpp"



Graph kruskalMST(const Graph& G) {
  int n = G.numVertices();
  Graph mst(n);
  std::set<Graph::Edge> edgeList;
  UnionFind uf(n);
  for (int u = 0; u < n; ++u) {
    for (auto e : *G.neighbours(u)) {
      if (u != e.v1) continue;
      edgeList.insert(e);
    }
  }
  for (auto e : edgeList) {
    if (!uf.sameSet(e.v1, e.v2)) {
      mst.addEdge(e);
      uf.merge(e.v1,e.v2);
    }
  }
  return mst;
}

