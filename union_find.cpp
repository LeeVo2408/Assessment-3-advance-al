#include <vector>
#include <unordered_set>
#include <numeric>
#include "union_find.hpp"

UnionFind::UnionFind(int N) : parent(N),
                              sizes(N, 1),
                              componentsCount(N) {
    std::iota(parent.begin(), parent.end(), 0);
}

int UnionFind::find(int element) {
    //while element is not the root, apply path compression
    if (parent[element] != element) {
        parent[element] = find(parent[element]);
    }
    return parent[element];
}

void UnionFind::merge(int element1, int element2) {
    int comp1 = find(element1);
    int comp2 = find(element2);
    if (comp1 == comp2) {
        return;
    }
    if (sizes[comp1] < sizes[comp2]) {
        parent[comp1] = comp2;
        sizes[comp2] += sizes[comp1];
    }

    else {
        parent[comp2] = comp1;
        sizes[comp1] += sizes[comp2];
    }
    --componentsCount;
}

bool UnionFind::sameSet(int element1, int element2) {
  return find(element1) == find(element2);
}

int UnionFind::numberOfComponents() const {
    return componentsCount;
}