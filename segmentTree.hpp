#ifndef SEGMENT_TREE_HPP
#define SEGMENT_TREE_HPP

#include <vector>

class SegmentTree {
public:
    SegmentTree(const std::vector<int>& array);
    void build(const std::vector<int>& array, int node, int start, int end);
    int query(int start, int end);
    void update( int index, int val);

    
private:
    void updateUtil(int node, int index, int start, int end, int val);
    int queryUtil(int node, int start, int end, int treeLeft, int Treeright);
    
    std::vector<int> segmentTree;
    int size;

};

#endif // SEGMENT_TREE_HPP
