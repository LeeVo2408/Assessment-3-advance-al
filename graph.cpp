#include "graph.hpp"
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <algorithm>

// Graph member functions
Graph::Graph() = default;

Graph::Graph(int n, std::vector<Edge> vec)
             : adjList {std::vector<std::set<Edge> >(n)} {
  for (const Edge& e : vec) {
    addEdge(e);
  }
}

void Graph::addEdge(const Edge& e) {
  if (e.v1 >= 0 && e.v2 >= 0 &&
      e.v1 < numVertices() && e.v2 < numVertices()) {
    adjList.at(e.v1).insert(e);
    adjList.at(e.v2).insert(e);
  }
}

int Graph::numVertices() const {
  return static_cast<int>(adjList.size());
}

double Graph::edgeWeightSum() const {
  double totalWeight {0.0};
  for (int i = 0; i < numVertices(); ++i) {
    for (const Edge& e : *neighbours(i)) {
      totalWeight += e.weight;
    }
  }
  // undirected graph -> we have counted every edge twice
  return totalWeight/2;
}