#include "MinMaxHeap.h"

#include <string>
#include <iostream>
#include <cmath>

#define LEFTCHILD(i) (2 * i)
#define RIGHTCHILD(i) ((2 * i) + 1)
#define PARENT(i) (i / 2)

#define ROOT 1

template <typename T>
MinMaxHeap<T>::MinMaxHeap()
{
    // fill our first elem with a junk item, so our 1-indexing works
    data.push_back(0);
}

template <typename T>
MinMaxHeap<T>::MinMaxHeap(const std::vector<T> &array)
    : data(array)
{
    if (data.size() == 0)
        return;

    // move the first (0th) item to the back, because it won't be seen by our 0 indexing
    // because we're copying from a 0 indexed array, but this implementation uses 1 index
    // for easier indexing, the 0th element won't show up right now. so we're copying it to the end
    data.push_back(data[0]);

    // heapify the array
    for (int i = data.size() / 2; i > 0; i--)
    {
        percolateDown(i);
    }
}

template <typename T>
MinMaxHeap<T>::~MinMaxHeap()
{
}

template <typename T>
T MinMaxHeap<T>::deleteMin()
{
    if (data.size() <= 1)
    {
        throw "Attempt to delete min on empty MinMaxHeap";
    }

    T lowestElement = data[ROOT];
    T lastElement = data.back();
    data.pop_back();

    data[ROOT] = lastElement;
    percolateDown(ROOT);

    return lowestElement;
}

template <typename T>
T MinMaxHeap<T>::deleteMax()
{
    if (data.size() <= 1)
    {
        throw "Attempt to delete max on empty MinMaxHeap";
    }

    int indexofLargestElement;
    int heapSize = this->size();

    if (heapSize == 1)
    {
        indexofLargestElement = ROOT;
    }
    else if (heapSize == 2)
    {
        indexofLargestElement = LEFTCHILD(ROOT);
    }
    else
    {
        indexofLargestElement = (data[2] > data[3] ? 2 : 3);
    }

    T largestElement = data[indexofLargestElement];
    T lastElement = data.back();
    data.pop_back();

    data[indexofLargestElement] = lastElement;
    percolateDown(indexofLargestElement);
    return largestElement;
}

template <typename T>
T MinMaxHeap<T>::findMin() const
{
    return data[ROOT];
}

template <typename T>
T MinMaxHeap<T>::findMax() const
{
    if (data[LEFTCHILD(ROOT)] > data[RIGHTCHILD(ROOT)])
    {
        return data[LEFTCHILD(ROOT)];
    }
    return data[RIGHTCHILD(ROOT)];
}

template <typename StreamPrintable>
void MinMaxHeap<StreamPrintable>::printArray() const
{
    for (int i = ROOT; i < data.size(); i++)
    {
        std::cout << data[i] << ' ';
    }
    std::cout << '\n';
}

template <typename T>
int MinMaxHeap<T>::size() const
{
    // -1 to account for our 1 indexing (data[0] is junk)
    return data.size() - 1;
}

template <typename T>
void MinMaxHeap<T>::insertHeap(T item)
{
    data.push_back(item);
    percolateUp(data.size() - 1);
}

template <typename T>
void MinMaxHeap<T>::clear()
{
    data.clear();
}

template <typename T>
MinMaxHeap<T> MinMaxHeap<T>::buildHeap(std::vector<T> &array)
{
    return MinMaxHeap(array);
}

template <typename T>
bool MinMaxHeap<T>::isDepthEven(int index) const
{
    int depth = log2(index);
    return ((depth % 2) == 0);
}

template <typename T>
void MinMaxHeap<T>::percolateUp(int index)
{
    if (index == ROOT)
        return;

    if (isDepthEven(index)) // Min heap
    {
        if (data[index] > data[PARENT(index)])
        {
            std::swap(data[index], data[PARENT(index)]);
            percolateUpMax(PARENT(index));
        }
        else
        {
            percolateUpMin(index);
        }
    }
    else // Max heap
    {
        if (data[index] < data[PARENT(index)])
        {
            std::swap(data[index], data[PARENT(index)]);
            percolateUpMin(PARENT(index));
        }
        else
        {
            percolateUpMax(index);
        }
    }
}

template <typename T>
void MinMaxHeap<T>::percolateUpMin(int index)
{
    while ((PARENT(PARENT(index)) > 0) && (data[index] < data[PARENT(PARENT(index))]))
    {
        std::swap(data[index], data[PARENT(PARENT(index))]);
        index = PARENT(PARENT(index));
    }
}

template <typename T>
void MinMaxHeap<T>::percolateUpMax(int index)
{
    while ((PARENT(PARENT(index)) > 0) && (data[index] > data[PARENT(PARENT(index))]))
    {
        std::swap(data[index], data[PARENT(PARENT(index))]);
        index = PARENT(PARENT(index));
    }
}

template <typename T>
void MinMaxHeap<T>::percolateDown(int index)
{
    if (isDepthEven(index))
    {
        percolateDownMin(index);
    }
    else
    {
        percolateDownMax(index);
    }
}

template <typename T>
void MinMaxHeap<T>::percolateDownMin(int hole)
{
    // if we have at least one child
    if (LEFTCHILD(hole) < data.size())
    {
        int smallestDescendantIndex = getIndexOfSmallestChildOrGrandchild(hole);

        if (smallestDescendantIndex == -1)
            throw "MinMaxHeap percolateDownMin could not find smallest child/grandchild";

        // if our smallest descendent is a grandchild
        if (smallestDescendantIndex > RIGHTCHILD(hole))
        {
            if (data[smallestDescendantIndex] < data[hole])
            {
                std::swap(data[smallestDescendantIndex], data[hole]);

                if (data[smallestDescendantIndex] > data[PARENT(smallestDescendantIndex)])
                {
                    std::swap(data[smallestDescendantIndex], data[PARENT(smallestDescendantIndex)]);
                }
                percolateDown(smallestDescendantIndex);
            }
        }
        else // smallest descendent is hole's child
        {
            if (data[smallestDescendantIndex] < data[hole])
            {
                std::swap(data[smallestDescendantIndex], data[hole]);
            }
        }
    }
}

template <typename T>
void MinMaxHeap<T>::percolateDownMax(int hole)
{
    // if we have at least one child
    if (LEFTCHILD(hole) < data.size())
    {
        int largestDescendantIndex = getIndexOfLargestChildOrGrandchild(hole);

        if (largestDescendantIndex == -1)
            throw "MinMaxHeap percolateDownMin could not find smallest child/grandchild";

        // if our largest descendent is a grandchild
        if (largestDescendantIndex > RIGHTCHILD(hole))
        {
            if (data[largestDescendantIndex] > data[hole])
            {
                std::swap(data[largestDescendantIndex], data[hole]);
                if (data[largestDescendantIndex] < data[PARENT(largestDescendantIndex)])
                {
                    std::swap(data[largestDescendantIndex], data[PARENT(largestDescendantIndex)]);
                }
                percolateDown(largestDescendantIndex);
            }
        }
        else // smallest descendent is hole's child
        {
            if (data[largestDescendantIndex] > data[hole])
            {
                std::swap(data[largestDescendantIndex], data[hole]);
            }
        }
    }
}

template <typename T>
int MinMaxHeap<T>::getNumChildren(int index) const
{
    if (LEFTCHILD(index) >= data.size())
    {
        return 0;
    }
    if (RIGHTCHILD(index) >= data.size())
    {
        return 1;
    }
    return 2;
}

template <typename T>
int MinMaxHeap<T>::getIndexOfSmallestChild(int index) const
{
    int numChilden = getNumChildren(index);

    if (numChilden == 0)
        return -1;

    bool hasRightChild = (numChilden == 2); // if we have 2, then we have a right one

    if (!hasRightChild)
        return LEFTCHILD(index);

    if (data[LEFTCHILD(index)] < data[RIGHTCHILD(index)])
        return LEFTCHILD(index);

    return RIGHTCHILD(index);
}

// Returns -1 if no children
template <typename T>
int MinMaxHeap<T>::getIndexOfSmallestChildOrGrandchild(int index) const
{
    int smallestChildIndex = getIndexOfSmallestChild(index);
    if (smallestChildIndex == -1)
    {
        return -1;
    }

    // if we have no grandchildren, just return our smallest child
    if (getNumChildren(LEFTCHILD(index)) == 0)
        return smallestChildIndex;

    // we have at least one grandchild. get the smallest child of each of our children
    int smallestChildOfLeftChildIndex = -1;
    int smallestChildOfRightChildIndex = -1;

    if (getNumChildren(LEFTCHILD(index) != 0))
    {
        smallestChildOfLeftChildIndex = getIndexOfSmallestChild(LEFTCHILD(index));
    }
    if (getNumChildren(RIGHTCHILD(index) != 0))
    {
        smallestChildOfRightChildIndex = getIndexOfSmallestChild(RIGHTCHILD(index));
    }

    // if we only have one child
    if (smallestChildOfRightChildIndex == -1)
    {
        if (data[smallestChildIndex] < data[smallestChildOfLeftChildIndex])
        {
            return smallestChildIndex;
        }
        else
        {
            return smallestChildOfLeftChildIndex;
        }
    }

    // we have two children

    // if left grandchild is the smallest grandchild, compare it to our child and return lowest
    if (data[smallestChildOfLeftChildIndex] < data[smallestChildOfRightChildIndex])
    {
        if (data[smallestChildOfLeftChildIndex] < data[smallestChildIndex])
        {
            return smallestChildOfLeftChildIndex;
        }
        else
        {
            return smallestChildIndex;
        }
    }
    else // compare right grandchild to our child
    {
        if (data[smallestChildOfRightChildIndex] < data[smallestChildIndex])
        {
            return smallestChildOfRightChildIndex;
        }
        else
        {
            return smallestChildIndex;
        }
    }
    throw "Unexpected error. MinMaxHeap::getIndexOfSmallestChildOrGrandchild could not find smallest";
}

template <typename T>
int MinMaxHeap<T>::getIndexOfLargestChild(int index) const
{
    int numChilden = getNumChildren(index);

    if (numChilden == 0)
        return -1;

    bool hasRightChild = (numChilden == 2); // if we have 2, then we have a right one

    if (!hasRightChild)
        return LEFTCHILD(index);

    if (data[LEFTCHILD(index)] > data[RIGHTCHILD(index)])
        return LEFTCHILD(index);

    return RIGHTCHILD(index);
}

// Returns -1 if no children
template <typename T>
int MinMaxHeap<T>::getIndexOfLargestChildOrGrandchild(int index) const
{
    int largestChildIndex = getIndexOfLargestChild(index);
    if (largestChildIndex == -1)
    {
        return -1;
    }

    // if we have no grandchildren, just return our largest child
    if (getNumChildren(LEFTCHILD(index)) == 0)
        return largestChildIndex;

    // we have at least one grandchild. get the larger child of each of our children
    int largestChildOfLeftChildIndex = -1;
    int largestChildOfRightChildIndex = -1;

    if (getNumChildren(LEFTCHILD(index) != 0))
    {
        largestChildOfLeftChildIndex = getIndexOfLargestChild(LEFTCHILD(index));
    }
    if (getNumChildren(RIGHTCHILD(index) != 0))
    {
        largestChildOfRightChildIndex = getIndexOfLargestChild(RIGHTCHILD(index));
    }

    // if we only have one child
    if (largestChildOfRightChildIndex == -1)
    {
        if (data[largestChildIndex] > data[largestChildOfLeftChildIndex])
        {
            return largestChildIndex;
        }
        else
        {
            return largestChildOfLeftChildIndex;
        }
    }

    // we have two children

    // if left grandchild is the largest grandchild, compare it to our child and return largest
    if (data[largestChildOfLeftChildIndex] > data[largestChildOfRightChildIndex])
    {
        if (data[largestChildOfLeftChildIndex] > data[largestChildIndex])
        {
            return largestChildOfLeftChildIndex;
        }
        else
        {
            return largestChildIndex;
        }
    }
    else // compare right grandchild to our child
    {
        if (data[largestChildOfRightChildIndex] > data[largestChildIndex])
        {
            return largestChildOfRightChildIndex;
        }
        else
        {
            return largestChildIndex;
        }
    }
    throw "Unexpected error. MinMaxHeap::getIndexOfLargestChildOrGrandchild could not find smallest";
}

// Let the compiler know we will want a float/int versions of this
template class MinMaxHeap<int>;
template class MinMaxHeap<float>;

#include "8PuzzleSearch.h"
template class MinMaxHeap<PuzzleStateWithPriority *>;
