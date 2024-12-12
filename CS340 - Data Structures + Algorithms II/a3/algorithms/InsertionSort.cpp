#include "SortingAlgorithms.h"

#include <iostream>
#include <algorithm>

template <typename T>
void InsertionSort<T>::Sort(std::vector<T> *v)
{
    for (int i = 1; i < v->size(); i++) // for every element starting at 1
    {
        // starting at j = i, we go backwards. if element before us is larger, we swap. go until it's not.
        for (int j = i; (j != 0) && (*v)[j - 1] > (*v)[j]; j--)
        {
            std::swap((*v)[j], (*v)[j - 1]);
        }
    }
}

typedef unsigned int uint32_t;
template class InsertionSort<uint32_t>;