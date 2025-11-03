#ifndef GRAPH_HPP_
#define GRAPH_HPP_ 

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
    int edgeId {-1};
    // Define operator< for ordering edges in sets
    bool operator<(const Edge& other) const {
        return std::tie(v1, v2, weight, edgeId) < std::tie(other.v1, other.v2, other.weight, other.edgeId);
    }
  };

 private:
  std::vector<std::set<Edge> > adjList {};
  int nextEdgeId {}; // to assign unique edge IDs
  std::unordered_map<int,Edge> idOriginal; //original edge by id

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
  //get original edge by ID 
  const Edge& edgeByID(int edgeId) const;
  
};


#endif      // GRAPH_HPP_

//nothing