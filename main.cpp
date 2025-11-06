
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include "graph.hpp"
#include "kkt.hpp"
#include "boruvka.hpp"
#include "union_find.hpp"
#include <queue>
#include <limits>

double INF = std::numeric_limits<double>::infinity();
//======ALGORITHM TO CHECK CUT PROPERTY=================
bool verifyCutProperty(const Graph& G, const Graph& F) {
    int n = G.numVertices();
    //a copy of MST's edges
    std::vector<Graph::Edge> H;
    for (int u = 0; u < n; ++u) {
      for (auto e : *F.neighbours(u)) {
        if (u == e.v1) H.push_back(e);
      }
    }
    //check if edge satisfy cut property
    for (auto eh : H) {
        UnionFind uf(n);
        //rebuild the mst without eh
        for (auto e : H) {
            if (e.edgeId == eh.edgeId) continue; //skip eh
            uf.merge(e.v1, e.v2);
        }

        int a = uf.find(eh.v1);
        int b = uf.find(eh.v2);
        double minCut = INF;

        for (int u = 0; u < n; ++u) {
            for (auto e : *G.neighbours(u)) {
                if (u != e.v1) continue;
                if (uf.find(e.v1) != uf.find(e.v2)) {
                    minCut = std::min(minCut, e.weight); //update min cut edge if edge end points is in diff components
                }
            }
        }
        //if eh is heavier than the min cut then it violates cycle property
        if (eh.weight > minCut + 1e-5) {
            return false;
        }
    }
    return true;
}



//--------KNOWN ALGORITHMS FROM THE COURSE TO VERIFY RANDOM MST RESULTS----------------
//Kruskal algorithm to check 
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

Graph primMST(const Graph& G) {
  Graph mst(G.numVertices());
  std::vector<bool> marked(G.numVertices());
  for (int v = 0; v < G.numVertices(); ++v) {
    if (!marked.at(v)) {
      primOnConnectedComponent(v, G, marked, mst);
    }
  }
  return mst;
}


//===========Boruvka ALGORITHM TEST=================

TEST(MstBoruvkaTest, EmptyGraph) {
  Graph G(0);
  Graph mst = boruvkaMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 0);
}

TEST(MstBoruvkaTest, noEdgeGraph) {
  Graph G(9, {});
  Graph mst = boruvkaMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 0);
}

TEST(MstBoruvkaTest, allEdgesSameWeight) {
  Graph G{7, {{5, 0,1}, {5, 0, 2}, {5, 1, 3}, {5, 1, 5}, {5, 2, 3}, {5, 2, 4}, {5, 3, 4}, 
              {5, 4, 5}, {880, 4, 5}, {5, 4, 6}, {5, 5, 6}}};
  Graph mst = boruvkaMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 30);
}

TEST(MstBoruvkaTest, AustralianCities) {
  Graph G {7, {{2600, 0, 1}, {3600, 0, 2}, {2800, 1, 3}, {3000, 1, 5},
               {1400, 2, 3}, {1900, 2, 4}, {900, 3, 4}, {880, 4, 5},
               {1000, 4, 6}, {700, 5, 6}}};
  Graph mst = boruvkaMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 9280);
  /*
  minimum spanning tree should look like this:
  Neighbors of 0: {0,1}[2600]
  Neighbors of 1: {1,3}[2800] {0,1}[2600]
  Neighbors of 2: {2,3}[1400]
  Neighbors of 3: {2,3}[1400] {3,4}[900] {1,3}[2800]
  Neighbors of 4: {4,5}[880] {3,4}[900]
  Neighbors of 5: {5,6}[700] {4,5}[880]
  Neighbors of 6: {5,6}[700]
  Edge weight sum is 9280
  */
}

// example test case from Algorithms by Sedgewick and Wayne
TEST(MstBoruvkaTest, tinyEWG) {
  Graph G {8, {{0.35, 4, 5}, {0.37, 4, 7}, {0.28, 5, 7}, {0.16, 0, 7},
               {0.32, 1, 5}, {0.38, 0, 4}, {0.17, 2, 3}, {0.19, 1, 7},
               {0.26, 0, 2}, {0.36, 1, 2}, {0.29, 1, 3}, {0.34, 2, 7},
               {0.40, 6, 2}, {0.52, 3, 6}, {0.58, 6, 0}, {0.93, 6, 4}}};
  Graph mst = boruvkaMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 1.81);
  /*
  minimum spanning tree should look like this:
  Neighbors of 0: {0,2}[0.26] {0,7}[0.16]
  Neighbors of 1: {1,7}[0.19]
  Neighbors of 2: {6,2}[0.4] {2,3}[0.17] {0,2}[0.26]
  Neighbors of 3: {2,3}[0.17]
  Neighbors of 4: {4,5}[0.35]
  Neighbors of 5: {4,5}[0.35] {5,7}[0.28]
  Neighbors of 6: {6,2}[0.4]
  Neighbors of 7: {5,7}[0.28] {1,7}[0.19] {0,7}[0.16]
  Edge weight sum is 1.81
  */
}

TEST(MstBoruvkaTest, disconnectedGraph) {
  Graph G {8, { {1, 0, 1}, {1, 1, 2}, {1, 2, 3}, {1, 4, 5}, {1, 5, 6},
                {1, 6, 7} }};
  Graph mst = boruvkaMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 6.0);
}

// larger test case from Algorithms by Sedgewick and Wayne
TEST(MstBoruvkaTest, mediumEWG) {
  Graph G {"mediumEWG.txt"};
  Graph mst = boruvkaMST(G);
  std::cout << mst.edgeWeightSum();
  EXPECT_NEAR(mst.edgeWeightSum(), 10.46351, 0.00001);
}

double euclideanDist(std::pair<int, int> p1, std::pair<int, int> p2) {
  return std::sqrt(std::pow(p1.first - p2.first, 2) +
                   std::pow(p1.second - p2.second, 2));
}

Graph randomEuclideanGraph(int N, int numEdges, unsigned seed) {
  std::mt19937 mt {seed};
  std::uniform_int_distribution<int> pointDist {0, N};
  std::vector<std::pair<int, int> > points(N);
  std::generate(points.begin(), points.end(),
      [&mt, &pointDist](){return std::pair {pointDist(mt), pointDist(mt)};});
  // create graph with N vertices
  // add numEdges random edges between them with Euclidean distance weight
  Graph G {N};
  std::uniform_int_distribution<int> indexDist {0, N - 1};
  for (int i = 0; i < numEdges; ++i) {
    int index1 = indexDist(mt);
    int index2 = indexDist(mt);
    G.addEdge({euclideanDist(points.at(index1), points.at(index2)),
                index1, index2});
  }
  return G;
}

TEST(MstBoruvkaTest, smallRandomEuclidean) {
  const int N = 10;
  const int numEdges = 30;
  unsigned seed = 982'832;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}

TEST(MstBoruvkaTest, smallRandomEuclideanCutProperty) {
  const int N = 10;
  const int numEdges = 30;
  unsigned seed = 982'832;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
}

TEST(MstBoruvkaTest, smallRandomEuclidean2) {
  const int N = 15;
  const int numEdges = 50;
  unsigned seed = 892'893;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}

TEST(MstBoruvkaTest, smallRandomEuclidean2CutProperty) {
  const int N = 15;
  const int numEdges = 50;
  unsigned seed = 892'893;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
}

TEST(MstBoruvkaTest, mediumRandomEuclidean) {
  const int N = 250;
  const int numEdges = 1200;
  unsigned seed = 329'823;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);}

TEST(MstBoruvkaTest, mediumRandomEuclidean2CutProperty) {
  const int N = 250;
  const int numEdges = 1200;
  unsigned seed = 329'823;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
}

TEST(MstBoruvkaTest, largeRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 15'000;
  unsigned seed = 11'829'119;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}

TEST(MstBoruvkaTest, largeRandomEuclideanCutProperty) {
  const int N = 1000;
  const int numEdges = 15'000;
  unsigned seed = 11'829'119;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
}

TEST(MstBoruvkaTest, largeSparseRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 2'000;
  unsigned seed = 823'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph mst_res = kruskalMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);}

TEST(MstBoruvkaTest, largeSparseRandomEuclideanCutProperty) {
  const int N = 1000;
  const int numEdges = 2'000;
  unsigned seed = 823'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
}  

TEST(MstTest, 100KVertices) {
  const int N = 100'000;
  const int numEdges = 2'500'000;
  unsigned seed = 223'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}


//===========RANDOMISED ALGORITHM TEST=================

TEST(kktMST, EmptyGraph) {
  Graph G(0);
  Graph mst = kktMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 0);
}

TEST(kktMST, noEdgeGraph) {
  Graph G(9, {});
  Graph mst = kktMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 0);
}

TEST(kktMST, allEdgesSameWeight) {
  Graph G{7, {{5, 0,1}, {5, 0, 2}, {5, 1, 3}, {5, 1, 5}, {5, 2, 3}, {5, 2, 4}, {5, 3, 4}, 
              {5, 4, 5}, {880, 4, 5}, {5, 4, 6}, {5, 5, 6}}};
  Graph mst = kktMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 30);
}

TEST(kktMST, AustralianCities) {
  Graph G {7, {{2600, 0, 1}, {3600, 0, 2}, {2800, 1, 3}, {3000, 1, 5},
               {1400, 2, 3}, {1900, 2, 4}, {900, 3, 4}, {880, 4, 5},
               {1000, 4, 6}, {700, 5, 6}}};
  Graph mst = kktMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 9280);
}

// example test case from Algorithms by Sedgewick and Wayne
TEST(kktMST, tinyEWG) {
  Graph G {8, {{0.35, 4, 5}, {0.37, 4, 7}, {0.28, 5, 7}, {0.16, 0, 7},
               {0.32, 1, 5}, {0.38, 0, 4}, {0.17, 2, 3}, {0.19, 1, 7},
               {0.26, 0, 2}, {0.36, 1, 2}, {0.29, 1, 3}, {0.34, 2, 7},
               {0.40, 6, 2}, {0.52, 3, 6}, {0.58, 6, 0}, {0.93, 6, 4}}};
  Graph mst = kktMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 1.81);
}

TEST(kktTest, disconnectedGraph) {
  Graph G {8, { {1, 0, 1}, {1, 1, 2}, {1, 2, 3}, {1, 4, 5}, {1, 5, 6},
                {1, 6, 7} }};
  Graph mst = kktMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), 6.0);
}

// larger test case from Algorithms by Sedgewick and Wayne
TEST(kktTest, mediumEWG) {
  Graph G {"mediumEWG.txt"};
  Graph mst = kktMST(G);
  std::cout << mst.edgeWeightSum();
  EXPECT_NEAR(mst.edgeWeightSum(), 10.46351, 0.00001);
}

TEST(kktTest, mediumEWGCutProperty) {
  Graph G {"mediumEWG.txt"};
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
}

TEST(kktTest, smallRandomEuclidean) {
  const int N = 10;
  const int numEdges = 30;
  unsigned seed = 982'832;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}

TEST(kktTest, smallRandomEuclideanCutProperty) {
  const int N = 10;
  const int numEdges = 30;
  unsigned seed = 982'832;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
} 

TEST(kktTest, smallRandomEuclidean2) {
  const int N = 15;
  const int numEdges = 50;
  unsigned seed = 892'893;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}


TEST(kktTest, smallRandomEuclidean2CutProperty) {
  const int N = 15;
  const int numEdges = 50;
  unsigned seed = 892'893;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
} 

TEST(kktTest, mediumRandomEuclidean) {
  const int N = 250;
  const int numEdges = 1200;
  unsigned seed = 329'823;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);}

TEST(kktTest, mediumRandomEuclideanCutProperty) {
  const int N = 250;
  const int numEdges = 1200;
  unsigned seed = 329'823;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
} 

TEST(kktTest, largeRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 15'000;
  unsigned seed = 11'829'119;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}

TEST(kktTest, largeRandomEuclideanCutProperty) {
  const int N = 1000;
  const int numEdges = 15'000;
  unsigned seed = 11'829'119;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
} 

TEST(kktTest, largeSparseRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 2'000;
  unsigned seed = 823'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph mst_res = kruskalMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);}

TEST(kktTest, largeSparseRandomEuclideanCutProperty) {
  const int N = 1000;
  const int numEdges = 2'000;
  unsigned seed = 823'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  bool f = verifyCutProperty(G, mst);
  EXPECT_TRUE(f);
} 

TEST(kktTest, 100KVertices) {
  const int N = 100'000;
  const int numEdges = 2'500'000;
  unsigned seed = 223'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph mst_res = primMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}