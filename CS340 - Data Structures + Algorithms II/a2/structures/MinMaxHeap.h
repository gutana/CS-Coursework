#pragma once

#include <vector>

// MinMaxHeap assumes no duplicate elements.

// Type must be comparable and stream printable
template <typename T>
class MinMaxHeap
{
private:
    MinMaxHeap(const std::vector<T> &);

public:
    MinMaxHeap();
    ~MinMaxHeap();

    T deleteMin();
    T deleteMax();

    T findMin() const;
    T findMax() const;

    void printArray() const;
    int size() const;

    void insertHeap(T);

    void clear();

    static MinMaxHeap buildHeap(std::vector<T> &);

private:
    std::vector<T> data;

    void percolateUp(int);
    void percolateDown(int);

    // percolate down implementation functions, depending on which heap we are in
    void percolateDownMin(int);
    void percolateDownMax(int);

    // percolate up implementation functions, depending on which heap we are in
    void percolateUpMin(int);
    void percolateUpMax(int);

public:
    // helper functions
    bool isDepthEven(int) const;

private:
    // returns number of children
    int getNumChildren(int) const;

    int getIndexOfSmallestChild(int) const;
    int getIndexOfSmallestChildOrGrandchild(int) const;

    int getIndexOfLargestChild(int) const;
    int getIndexOfLargestChildOrGrandchild(int) const;
};
