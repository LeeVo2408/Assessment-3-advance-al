#include "SegmentTree.hpp"
#include <algorithm>

SegmentTree::SegmentTree(const std::vector<int>& array) {
    size = array.size();
    segmentTree.resize(4 * size);
    build(array, 1, 0, size - 1);
}

void SegmentTree::build(const std::vector<int>& array, int node, int start, int end) {
    if (start == end) {
        segmentTree[node] = array[start];
        return;
    }

    int mid = (start + end) / 2;
    build(array, 2 * node, start, mid);
    build(array, 2 * node + 1, mid + 1, end);
    segmentTree[node] = segmentTree[2 * node] + segmentTree[2 * node + 1];
}

void SegmentTree::update(int index, int val) {
    updateUtil(1, 0, size - 1, index, val);
}

void SegmentTree::updateUtil(int node, int start, int end, int index, int val) {
    if (start == end) {
        segmentTree[node] = val;
    } else {
        int mid = (start + end) / 2;
        if (index <= mid) {
            updateUtil(2 * node, start, mid, index, val);
        } else {
            updateUtil(2 * node + 1, mid + 1, end, index, val);
        }
        segmentTree[node] = segmentTree[2 * node] + segmentTree[2 * node + 1];
    }
}

int SegmentTree::query(int start, int end) {
    return queryUtil(1, 0, size - 1, start, end);
}

int SegmentTree::queryUtil(int node, int start, int end, int treeLeft, int treeRight) {
    if (treeRight < start || end < treeLeft) {
        return 0;
    }

    if (treeLeft <= start && end <= treeRight) {
        return segmentTree[node];
    }

    int mid = (start + end) / 2;
    return queryUtil(2 * node, start, mid, treeLeft, treeRight) +
           queryUtil(2 * node + 1, mid + 1, end, treeLeft, treeRight);
}

//reference code https://www.geeksforgeeks.org/dsa/introduction-to-segment-trees-2/