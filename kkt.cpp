#include "union_find.hpp"
#include "graph.hpp"
#include <unordered_map>
#include "kkt.hpp"
#include <random>
#include "lca.hpp"

//Boruvka step to return the chosen edges and the connected components (supernode) for next round
//time complexity: O(ma(n)) ~ O(m)
std::pair<std::vector<Graph::Edge>, Graph> boruvkaStep(const Graph& G) {
    int n = G.numVertices();
    UnionFind UF(n);
    std::vector<Graph::Edge> cheapest(n);
    std::vector<bool> hasCheapest(n, false);
    for (int u = 0; u < n; ++u) {
        for (auto e : *G.neighbours(u)) {
            if (u != e.v1) continue;                        //avoid duplicate edge
            int comp1 = UF.find(e.v1);
            int comp2 = UF.find(e.v2);

            if (comp1 == comp2) continue;                   //v1 and v2 are in same component

            //update cheapest edge for component root1
            if (!hasCheapest[comp1] || e.weight < cheapest[comp1].weight) {
                cheapest[comp1] = e;
                hasCheapest[comp1] = true;
            }
            //update cheapest edge for component root2
            if (!hasCheapest[comp2] || e.weight < cheapest[comp2].weight) {
                cheapest[comp2] = e;
                hasCheapest[comp2] = true;
            }
        }
    }
    std::vector<Graph::Edge> chosen;  //edges chosen in this Boruvka step (will be part of MST)
    for (int v = 0; v < n; ++v) {
        if (!hasCheapest[v]) continue;
        auto e = cheapest[v];
        int comp1 = UF.find(e.v1);
        int comp2 = UF.find(e.v2);

        if (comp1 == comp2) continue;

        chosen.push_back(e); //add the chosen edge
        UF.merge(comp1, comp2);
    }
    //construct the contract graph G1
    
    //vertices in the same component in UF are contracted into a supernode
    std::vector<int> vertexSuperNode(n);                //keep track of vertex's current supernode
    std::unordered_map<int, int> superNodes;            //map the component to new supernode id
    int compCount = 0;
    for (int v = 0; v < n; ++v) {
        int comp = UF.find(v);
        //assign new id (compCount) if the component is not mapped yet
        if (superNodes.find(comp) == superNodes.end()) {
            superNodes[comp] = compCount;
            vertexSuperNode[v] = superNodes[comp];
            ++compCount;
        }
        else {
            vertexSuperNode[v] = superNodes[comp];
        }
    }
    
    //choosing the lightest edge crossing the cut
    std::unordered_map<std::pair<int,int>, Graph::Edge, pairhash> lightest;
    lightest.reserve(n*4);
    
    for (int u = 0; u < n; ++u) {
        for (auto e : *G.neighbours(u)) {
            if (u != e.v1) continue;                    //avoid duplicate edge
            int su = vertexSuperNode[u];                //supernode of vertex u
            int sv = vertexSuperNode[e.v2];             //supernode of vertex v

            if (su == sv) continue;                     //both endpoints are in same supernode (delete self-loop)
            std::pair<int,int> k = makeOrderedPair(su, sv);
            if (!lightest.count(k) || lightest.at(k).weight > e.weight) {
                lightest[k] = {e.weight, su, sv, e.edgeId};
            }
        }
    }

    //now add edges to the contracted graph

    Graph contracted(compCount);
    for (const auto& e : lightest) {
        contracted.addEdge(e.second);
    }

    return {chosen, contracted};
}

//random function to choose edges with probability 1/2
bool randomChoice() {
    static std::mt19937 mt {std::random_device{}()}; ;
    static std::bernoulli_distribution dist(0.5);
    return dist(mt);
}

Graph kktMST(const Graph& G) {
    int n = G.numVertices();
    Graph mst(n);
    //base case
    if (n <= 1) return mst;
    
    // Check if graph has any edges
    if (G.edgeWeightSum()==0) return mst;

    //running 2 Boruvka steps on G
    auto s1 = boruvkaStep(G);
    auto s2 = boruvkaStep(s1.second);
    const auto B1 = s1.first;     //chosen edges from first Boruvka step
    const auto B2 = s2.first;     //chosen edges from second Boruvka step
    Graph G1 = s2.second;      //contracted graph


    Graph H(G1.numVertices());
    //random sampling each edge with probability 1/2
    for (int u = 0; u < G1.numVertices(); ++u) {
        for (auto e : *G1.neighbours(u)) {
            if (u != e.v1) continue;
            if (randomChoice()) {
                H.addEdge(e);
            }
        }
    }
    //recursive call on H to find MSF of subproblem
    Graph F = kktMST(H);

    //find F-heavy edges in G1 and remove them
    LCA lca(F); 
    Graph G2(G1.numVertices()); //graph after removing F-heavy edges

    for (int u = 0; u < G1.numVertices(); ++u) {
        for (auto e : *G1.neighbours(u)) {
            if (u != e.v1) continue;
            double maxW = lca.maxEdgeWeight(e.v1, e.v2);
            if (e.weight > maxW) continue; // Skip F-heavy edges
            G2.addEdge(e);
        }
    }
    //recursive call on G2 to find MSF
    Graph F2 = kktMST(G2);
    
    //mst is union of F2 and B
    for (int u = 0; u < F2.numVertices(); ++u) {
        for (auto e : *F2.neighbours(u)) {
            if (u != e.v1) continue;
            mst.addEdge(G.edgeByID(e.edgeId));
        }
    }

    for (auto e : B1) {
        mst.addEdge(e);
    }

    for (auto e : B2) {
        mst.addEdge(G.edgeByID(e.edgeId));
    }

    return mst;
}


//helper functions
std::pair<int, int> makeOrderedPair(int a, int b) {
    if (a > b) std::swap(a, b);
    return {a, b};
}