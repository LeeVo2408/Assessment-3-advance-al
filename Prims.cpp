#include <fstream>
#include <utility>
#include <functional>
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <algorithm>
#include "graph.hpp"



// do Prim on a connected component. Modifies marked and mst
void primOnConnectedComponent(int start, const Graph& G,
                              std::vector<bool>& marked, Graph& mst) {
  // min priority queue using edge weight as key
  std::priority_queue<Graph::Edge, std::vector<Graph::Edge>,
                            std::greater<Graph::Edge> > edgeQueue {};
  marked.at(start) = true;
  for (const auto& e : *G.neighbours(start)) {
      edgeQueue.push(e);
  }
  while (!edgeQueue.empty()) {
    Graph::Edge minWeightEdge = edgeQueue.top();
    edgeQueue.pop();
    // if edge does not cross current cut move on to next edge
    if (marked.at(minWeightEdge.v1) == marked.at(minWeightEdge.v2)) {
      continue;
    }
    // otherwise add the edge to growing mst
    mst.addEdge(minWeightEdge);
    // find the vertex that is still unmarked
    int vertexToAdd = !marked.at(minWeightEdge.v1) ? minWeightEdge.v1 :
                                                     minWeightEdge.v2;
    marked.at(vertexToAdd) = true;
    // add cut edges of vertexToAdd to the priority queue
    for (const auto& e : *G.neighbours(vertexToAdd)) {
      // only add edge if it is in the current cut
      if (marked.at(e.v1) != marked.at(e.v2)) {
        edgeQueue.push(e);
      }
    }
  }
}

Graph computePrimsMST(const Graph& G
) {
  Graph mst(G.numVertices());
  std::vector<bool> marked(G.numVertices());
  for (int v = 0; v < G.numVertices(); ++v) {
    if (!marked.at(v)) {
      primOnConnectedComponent(v, G, marked, mst);
    }
  }
  return mst;
}