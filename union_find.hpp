#ifndef UNION_FIND_HPP_ 
#define UNION_FIND_HPP_ 

#include <vector>

//union find data structure using path compression
class UnionFind {
    private:
     std::vector<int> parent;
     std::vector<int> sizes;
     std::vector<int> ranks; 
     int componentsCount;
    public:
     explicit UnionFind(int N);

     // return the name of the root of the tree containing element (with path compression)
     int find(int element);

     // merge the sets containing element1 and element2 (union by rank)
     void merge(int element1, int element2);
     
     // are element1 and element2 in the same set?
     bool sameSet(int element1, int element2);

     // return the number of components
     int numberOfComponents() const; 
};

#endif      // UNION_FIND_HPP_ 