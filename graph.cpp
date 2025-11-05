#include "graph.hpp"
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <utility>
#include <functional>
#include <iostream>


// Graph member functions
Graph::Graph() = default;

Graph::Graph(int n, std::vector<Edge> vec)
             : adjList {std::vector<std::vector<Edge> >(n)}, nextEdgeId {0} {
  for (const Edge& e : vec) {
    addEdge(e);
  }
}


Graph::Graph(const std::string& inputFile) {
  std::ifstream infile {inputFile};
  if (!infile) {
    std::cerr << inputFile << " could not be opened\n";
    return;
  }
  // first line has number of vertices N
  int N {};
  infile >> N;
  adjList.resize(N);
  int i {};
  int j {};
  double weight {};
  // assume each remaining line is of form
  // origin dest weight
  while (infile >> i >> j >> weight) {
    addEdge({weight, i, j});
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

    adjList.at(e.v1).push_back(e);
    adjList.at(e.v2).push_back(e);
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

//get original edge in G from edgeID
const Graph::Edge &Graph::edgeByID(int edgeId) const {
  return idOriginal.at(edgeId);
}

// print out adjacency list of a Graph
std::ostream& operator<<(std::ostream& out, const Graph& G) {
  for (Graph::iterator it = G.begin(); it != G.end(); ++it) {
    for (const Graph::Edge& e : *it) {
      std::cout << e << ' ';
    }
    std::cout << '\n';
  }
  return out;
}

// print out an Edge
std::ostream& operator<<(std::ostream& out, const Graph::Edge& e) {
  out << '{' << e.v1 << ", " << e.v2 << "}[" << e.weight << ']';
  return out;
}