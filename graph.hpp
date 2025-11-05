#ifndef GRAPH_HPP_
#define GRAPH_HPP_ 

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

// Class for undirected graphs with edge weights
class Graph {
 public:
  struct Edge {
    double weight {};
    int v1 {};
    int v2 {};
    int edgeId {-1};  //edgeID to keep track of the edges after contraction
    auto operator<=>(const Edge&) const = default;
  };

 private:
  std::vector<std::vector<Edge> > adjList {};
  int nextEdgeId {}; // to assign unique edge IDs
  std::unordered_map<int,Edge> idOriginal; //original edge by id

 public:
  // default constructor
  Graph();
  // construct graph with n vertices and optionally provide
  // a vector of edges
  explicit Graph(int n, std::vector<Edge> = {});

  // read list of edges in from a file
  explicit Graph(const std::string& inputFile);

  void addEdge(Edge);
  int numVertices() const;
  double edgeWeightSum() const;

  using iterator = std::vector<std::vector<Edge> >::const_iterator;

  // cbegin returns const iterator pointing to first element of adjList
  iterator begin() const {
    return adjList.cbegin();
  }

  iterator end() const {
    return adjList.cend();
  }

  // return iterator to a particular vertex
  iterator neighbours(int a) const {
    return adjList.cbegin() + a;
  }
  //get original edge by ID 
  const Edge& edgeByID(int edgeId) const;
  
};

// print out a Graph
std::ostream& operator<<(std::ostream&, const Graph&);

// print out an Edge
std::ostream& operator<<(std::ostream&, const Graph::Edge&);


#endif      // GRAPH_HPP_

