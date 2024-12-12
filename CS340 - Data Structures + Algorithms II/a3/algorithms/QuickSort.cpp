#include "SortingAlgorithms.h"

#include <iostream>
#include <algorithm>

// Algorithm will stop recursively sorting when sub array size is less than k (for quick insertion sort).
// By default, k is -1 so it completely sort.
template <typename T>
void QuickSort<T>::Sort(std::vector<T> *v, int k)
{
    quickSort(v, 0, v->size() - 1, k);
}

template <typename T>
void QuickSort<T>::quickSort(std::vector<T> *v, int left, int right, int k)
{
    if (left >= right || left < 0 || right >= v->size()) // bounds checking
        return;

    // early return for quick-insertion sort if k is supplied
    // k is -1 by default, so this will never be true unless a positive k is given
    if ((right - left) < k)
    {
        return;
    }

    int p = choosePivot(v, left, right);
    p = partition(v, left, right, p);
    quickSort(v, left, p);
    quickSort(v, p + 1, right);
}

// finds median of 3 and places it at end
template <typename T>
int QuickSort<T>::choosePivot(std::vector<T> *v, int left, int right) // median 3 partition
{
    if (left < 0 || right > v->size() - 1)
        throw "Choose Pivot received improper bounds...";

    int center = (left + right) / 2;

    if ((*v)[center] < (*v)[left])
        std::swap((*v)[left], (*v)[center]); // left <= center

    if ((*v)[right] < (*v)[left])
        std::swap((*v)[left], (*v)[right]); // left <= right

    if ((*v)[right] < (*v)[center])
        std::swap((*v)[center], (*v)[right]); // center <= right

    std::swap((*v)[center], (*v)[right]);

    int pivot = right;
    return pivot;
}

template <typename T>
int QuickSort<T>::partition(std::vector<T> *v, int low, int high, int pivot)
{
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if ((*v)[j] < (*v)[pivot])
        {
            i++;
            std::swap((*v)[i], (*v)[j]);
        }
    }
    i++;
    std::swap((*v)[i], (*v)[pivot]);
    return i;
}

typedef unsigned int uint32_t;
template class QuickSort<uint32_t>;