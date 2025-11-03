#include "graph.hpp"
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <algorithm>

// Graph member functions
Graph::Graph() = default;

Graph::Graph(int n, std::vector<Edge> vec)
             : adjList {std::vector<std::set<Edge> >(n)}, nextEdgeId {0} {
  for (const Edge& e : vec) {
    addEdge(e);
  }
}

void Graph::addEdge(Edge e) {
  if (e.v1 >= 0 && e.v2 >= 0 &&
      e.v1 < numVertices() && e.v2 < numVertices()) {
    if (e.edgeId == -1) {
      e.edgeId = nextEdgeId++; //assign unique edge ID
      idOriginal.insert({e.edgeId, e});
    }
    else {
      if (e.edgeId >= nextEdgeId) {
        nextEdgeId = e.edgeId + 1; //update nextEdgeId to avoid collision
      }
    }
    //update new edgeID
    if (!idOriginal.count(e.edgeId)) {
      idOriginal.insert({e.edgeId, e});
    }

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

const Graph::Edge &Graph::edgeByID(int edgeId) const {
  return idOriginal.at(edgeId);
}

//nothing