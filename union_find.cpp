#include <vector>
#include <unordered_set>
#include <numeric>
#include "union_find.hpp"

UnionFind::UnionFind(int N) : parent(N),
                              sizes(N, 1),
                              ranks(N,0),
                              componentsCount(N) {
    std::iota(parent.begin(), parent.end(), 0);
}

//union find with union by rank and path compression
int UnionFind::find(int element) {
    //Find the root of the current set
    int root = element;
    while (parent[root] != root) {
        root = parent[root]; 
    }
    //Path compression
    while (parent[element] != element) {
        int next = parent[element];
        parent[element] = root;
        element = next;
    }
    return root;
}


void UnionFind::merge(int element1, int element2) {
    int root1 = find(element1);
    int root2 = find(element2);
    if (root1 == root2) {
        return;
    }
    if (ranks[root1] < ranks[root2]) {
        parent[root1] = root2;

    } else if (ranks[root1] > ranks[root2]) {
        parent[root2] = root1;

    } 
    // Ranks are equal
    else { 
        parent[root2] = root1; 
        ranks[root1]++;       
    }
    --componentsCount;
}

bool UnionFind::sameSet(int element1, int element2) {
  return find(element1) == find(element2);
}

int UnionFind::numberOfComponents() const {
    return componentsCount;
}