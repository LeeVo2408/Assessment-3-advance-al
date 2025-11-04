#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include "graph.hpp"
#include "boruvka.hpp"
#include "Kruskal.cpp"
#include "Prims.cpp"
#include "kkt.hpp"


// ! boruvka TEST 
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

TEST(BoruvkaTest, AustralianCities) {
  Graph G {7, {{2600, 0, 1}, {3600, 0, 2}, {2800, 1, 3}, {3000, 1, 5},
               {1400, 2, 3}, {1900, 2, 4}, {900, 3, 4}, {880, 4, 5},
               {1000, 4, 6}, {700, 5, 6}}};
  Graph mst = boruvkaMST(G);
  Graph krusalMST = kruskalMST(G);
  Graph primsMST = computePrimsMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), krusalMST.edgeWeightSum(), primsMST.edgeWeightSum(), 9280);
}

TEST(BoruvkaTest, tinyEWG) {
  Graph G {8, {{0.35, 4, 5}, {0.37, 4, 7}, {0.28, 5, 7}, {0.16, 0, 7},
               {0.32, 1, 5}, {0.38, 0, 4}, {0.17, 2, 3}, {0.19, 1, 7},
               {0.26, 0, 2}, {0.36, 1, 2}, {0.29, 1, 3}, {0.34, 2, 7},
               {0.40, 6, 2}, {0.52, 3, 6}, {0.58, 6, 0}, {0.93, 6, 4}}};
  Graph mst = boruvkaMST(G);
  Graph krusalMST = kruskalMST(G);
  Graph primsMST = computePrimsMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), krusalMST.edgeWeightSum(), primsMST.edgeWeightSum(), 1.81);

}

TEST(BoruvkaTest, disconnectedGraph) {
  Graph G {8, { {1, 0, 1}, {1, 1, 2}, {1, 2, 3}, {1, 4, 5}, {1, 5, 6},
                {1, 6, 7} }};
  Graph mst = boruvkaMST(G);
  Graph krusalMST = kruskalMST(G);
  Graph primsMST = computePrimsMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), krusalMST.edgeWeightSum(), primsMST.edgeWeightSum(),6.0);
}

// larger test case from Algorithms by Sedgewick and Wayne
TEST(MstTest, mediumEWG) {
  Graph G {"mediumEWG.txt"};
  Graph mst = boruvkaMST(G);
  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);


 EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
 EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
 EXPECT_NEAR(primsMST.edgeWeightSum(), 10.46351,  0.00001);

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

TEST(MstTest, smallRandomEuclidean) {
  const int N = 10;
  const int numEdges = 30;
  unsigned seed = 982'832;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);


 EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
 EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
 EXPECT_NEAR(primsMST.edgeWeightSum(), 29.52429828536199,  0.00001);
}

TEST(MstTest, smallRandomEuclidean2) {
  const int N = 15;
  const int numEdges = 50;
  unsigned seed = 892'893;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);

  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(),  59.098762290261007,  0.00001);
}

TEST(MstTest, mediumRandomEuclidean) {
  const int N = 250;
  const int numEdges = 1200;
  unsigned seed = 329'823;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(),  13315.932367669795,  0.00001);
}

TEST(MstTest, largeRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 15'000;
  unsigned seed = 11'829'119;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);

  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(), 114444.10921709349,  0.00001);
}

TEST(MstTest, largeSparseRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 2'000;
  unsigned seed = 823'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);

  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(), 339061.30473209539,  0.00001);
}

TEST(MstTest, 100KVertices) {
  const int N = 100'000;
  const int numEdges = 2'500'000;
  unsigned seed = 223'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = boruvkaMST(G);
  Graph mst_res = computePrimsMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}


// ! KKT TEST 

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


TEST(KktTest, AustralianCities) {
  Graph G {7, {{2600, 0, 1}, {3600, 0, 2}, {2800, 1, 3}, {3000, 1, 5},
               {1400, 2, 3}, {1900, 2, 4}, {900, 3, 4}, {880, 4, 5},
               {1000, 4, 6}, {700, 5, 6}}};
  Graph mst = kktMST(G);
  Graph krusalMST = kruskalMST(G);
  Graph primsMST = computePrimsMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), krusalMST.edgeWeightSum(), primsMST.edgeWeightSum(), 9280);
}

TEST(KktTest, tinyEWG) {
  Graph G {8, {{0.35, 4, 5}, {0.37, 4, 7}, {0.28, 5, 7}, {0.16, 0, 7},
               {0.32, 1, 5}, {0.38, 0, 4}, {0.17, 2, 3}, {0.19, 1, 7},
               {0.26, 0, 2}, {0.36, 1, 2}, {0.29, 1, 3}, {0.34, 2, 7},
               {0.40, 6, 2}, {0.52, 3, 6}, {0.58, 6, 0}, {0.93, 6, 4}}};
  Graph mst = kktMST(G);
  Graph krusalMST = kruskalMST(G);
  Graph primsMST = computePrimsMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), krusalMST.edgeWeightSum(), primsMST.edgeWeightSum(), 1.81);

}

TEST(KktTest, disconnectedGraph) {
  Graph G {8, { {1, 0, 1}, {1, 1, 2}, {1, 2, 3}, {1, 4, 5}, {1, 5, 6},
                {1, 6, 7} }};
  Graph mst = kktMST(G);
  Graph krusalMST = kruskalMST(G);
  Graph primsMST = computePrimsMST(G);
  EXPECT_DOUBLE_EQ(mst.edgeWeightSum(), krusalMST.edgeWeightSum(), primsMST.edgeWeightSum(),6.0);
}

TEST(KktTest, mediumEWG) {
  Graph G {"mediumEWG.txt"};
  Graph mst = kktMST(G);
  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);


 EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
 EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
 EXPECT_NEAR(primsMST.edgeWeightSum(), 10.46351,  0.00001);

}


TEST(KktTest, smallRandomEuclidean) {
  const int N = 10;
  const int numEdges = 30;
  unsigned seed = 982'832;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);


 EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
 EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
 EXPECT_NEAR(primsMST.edgeWeightSum(), 29.52429828536199,  0.00001);
}

TEST(KktTest, smallRandomEuclidean2) {
  const int N = 15;
  const int numEdges = 50;
  unsigned seed = 892'893;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);

  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(),  59.098762290261007,  0.00001);
}

TEST(KktTest, mediumRandomEuclidean) {
  const int N = 250;
  const int numEdges = 1200;
  unsigned seed = 329'823;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(),  13315.932367669795,  0.00001);
}

TEST(KktTest, largeRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 15'000;
  unsigned seed = 11'829'119;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);

  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(), 114444.10921709349,  0.00001);
}

TEST(KktTest, largeSparseRandomEuclidean) {
  const int N = 1000;
  const int numEdges = 2'000;
  unsigned seed = 823'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);

  Graph primsMST = computePrimsMST(G);
  Graph krusalMST = kruskalMST(G);

  EXPECT_NEAR(mst.edgeWeightSum(), primsMST.edgeWeightSum(), 0.00001);
  EXPECT_NEAR(mst.edgeWeightSum(), krusalMST.edgeWeightSum(),  0.00001);
  EXPECT_NEAR(primsMST.edgeWeightSum(), 339061.30473209539,  0.00001);
}


TEST(kktTest, 100KVertices) {
  const int N = 100'000;
  const int numEdges = 2'500'000;
  unsigned seed = 223'238;
  Graph G = randomEuclideanGraph(N, numEdges, seed);
  Graph mst = kktMST(G);
  Graph mst_res = computePrimsMST(G);
  EXPECT_NEAR(mst.edgeWeightSum(), mst_res.edgeWeightSum(), 0.00001);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



