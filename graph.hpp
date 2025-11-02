#ifndef GRAPH_HPP_
#define GRAPH_HPP_ 

#include <string>
#include <set>
#include <vector>

// Class for undirected graphs with edge weights
class Graph {
 public:
  struct Edge {
    double weight {};
    int v1 {};
    int v2 {};
    int edgeId {};
    // Define operator< for ordering edges in sets, ignoring weight and edgeId
    bool operator<(const Edge& other) const {
      int a = v1, b = v2;
      int a1 = other.v1, b1 = other.v2;
      if (a > b) std::swap(a, b);
      if (a1 > b1) std::swap(a1, b1);
      if (a == a1) {
        return b < b1;
      }
      return a < a1;
    }
  };

 private:
  std::vector<std::set<Edge> > adjList {};
  int nextEdgeId {}; // to assign unique edge IDs

 public:
  // default constructor
  Graph();
  // construct graph with n vertices and optionally provide
  // a vector of edges
  explicit Graph(int n, std::vector<Edge> = {});

  void addEdge(Edge);
  int numVertices() const;
  double edgeWeightSum() const;

  using iterator = std::vector<std::set<Edge> >::const_iterator;

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
  
};


#endif      // GRAPH_HPP_