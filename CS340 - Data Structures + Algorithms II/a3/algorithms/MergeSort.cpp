#include "SortingAlgorithms.h"

#include <iostream>

template <typename T>
void MergeSort<T>::Sort(std::vector<T> *v)
{
    std::vector<T> *vectorCopy = new std::vector<T>();

    vectorCopy->reserve(v->size());
    for (T item : (*v))
    {
        vectorCopy->push_back(item);
    }

    mergeSort(v, vectorCopy, v->size());
    delete vectorCopy;
}

template <typename T>
void MergeSort<T>::mergeSort(std::vector<T> *A, std::vector<T> *B, int n)
{
    splitMerge(A, 0, n, B);
}

template <typename T>
void MergeSort<T>::splitMerge(std::vector<T> *B, int begin, int end, std::vector<T> *A)
{
    if (end - begin <= 1)
        return;

    int middle = (end + begin) / 2;

    splitMerge(A, begin, middle, B);
    splitMerge(A, middle, end, B);

    merge(B, begin, middle, end, A);
}

template <typename T>
void MergeSort<T>::merge(std::vector<T> *B, int begin, int middle, int end, std::vector<T> *A)
{
    int i = begin;
    int j = middle;

    for (int k = begin; k < end; k++)
    {
        if (i < middle && (j >= end || (*A)[i] <= (*A)[j]))
        {
            (*B)[k] = (*A)[i];
            i++;
        }
        else
        {
            (*B)[k] = (*A)[j];
            j++;
        }
    }
}

typedef unsigned int uint32_t;
template class MergeSort<uint32_t>;