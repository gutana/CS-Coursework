#include "SortingAlgorithms.h"

#include <iostream>

template <typename T>
void QuickInsertionSort<T>::Sort(std::vector<T> *v, int k)
{
    QuickSort<T>::Sort(v, k);
    InsertionSort<T>::Sort(v);
}

typedef unsigned int uint32_t;
template class QuickInsertionSort<uint32_t>;