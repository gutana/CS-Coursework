#pragma once

#include <vector>

template <typename T>
class Stack
{
public:
    Stack();
    ~Stack();

    void Push(T item);
    T Pop();
    T Top() const;
    int Size() const;

    void Print() const;

private:
    std::vector<T> data;
};