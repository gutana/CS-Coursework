#pragma once

#include <vector>

// Various sorting algorithms. Will sort the vector passed in.

template <typename T>
class QuickSort
{
public:
    static void Sort(std::vector<T> *v, int k = -1);

private:
    static void quickSort(std::vector<T> *v, int low, int high, int k = -1);
    static int choosePivot(std::vector<T> *v, int low, int high);
    static int partition(std::vector<T> *v, int low, int high, int pivot);
};

template <typename T>
class MergeSort
{
public:
    static void Sort(std::vector<T> *v);

private:
    static void mergeSort(std::vector<T> *A, std::vector<T> *B, int n);
    static void splitMerge(std::vector<T> *B, int begin, int end, std::vector<T> *A);
    static void merge(std::vector<T> *B, int begin, int middle, int end, std::vector<T> *A);
};

template <typename T>
class InsertionSort
{
public:
    static void Sort(std::vector<T> *v);
};

template <typename T>
class QuickInsertionSort
{
public:
    static void Sort(std::vector<T> *v, int k);
};
