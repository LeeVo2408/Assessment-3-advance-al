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
    int root1 = find(element1);
    int root2 = find(element2);
    if (root1 == root2) {
        return;
    }
    if (sizes[root1] < sizes[root2]) {
        parent[root1] = root2;
        sizes[root2] += sizes[root1];
    }

    else {
        parent[root2] = root1;
        sizes[root1] += sizes[root2];
    }
    --componentsCount;
}

bool UnionFind::sameSet(int element1, int element2) {
  return find(element1) == find(element2);
}

int UnionFind::numberOfComponents() const {
    return componentsCount;
}