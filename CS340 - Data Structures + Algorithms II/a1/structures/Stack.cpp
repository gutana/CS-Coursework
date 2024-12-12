#include "Stack.h"
#include "Tree.h"

#include <string>
#include <iostream>

template <typename T>
Stack<T>::Stack()
{
}

template <typename T>
Stack<T>::~Stack()
{
}

template <typename T>
void Stack<T>::Push(T item)
{
    data.push_back(item);
}

template <typename T>
int Stack<T>::Size() const
{
    return data.size();
}

template <typename T>
T Stack<T>::Pop()
{
    if (data.size() == 0)
        throw("Attempted to pop item off of empty stack..."); //  type to catch is const char*

    T tempData = data.back();
    data.pop_back();
    return tempData;
}

template <typename T>
T Stack<T>::Top() const
{
    if (data.size() > 0)
        return data.back();
    else
        throw("Error: attempt to access top of empty stack");
}

template <typename T>
void Stack<T>::Print() const
{
    for (auto i : data)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

// Let compiler know we will want these 
template class Stack<std::string>;
template class Stack<TreeNode<std::string> *>;