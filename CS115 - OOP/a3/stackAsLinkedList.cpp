// Name:  Mohammad Al-Bayati
// ID:    200339106
// Class: CS 115 OOP
// file:  stackAsLinkedList.cpp

#include "stackAsLinkedList.h"
#include <iostream>

StackAsLinkedList::StackAsLinkedList()
    : list()
{
}

void StackAsLinkedList::Purge()
{
    list.Purge();
    count = 0;
}

// Next 4 functions are for assignment 5

StackAsLinkedList::~StackAsLinkedList()
{
    Purge();
}

void StackAsLinkedList::Push(Rainfall &rainfall)
{
    // Just append the rainfall to the end of the list
    list.Append(&rainfall);
}

void StackAsLinkedList::Pop()
{
    // Try to get the last element. catch an error in case something goes wrong
    // e.g. there is nothing on the stack
    try
    {
        list.Extract(&Top());
    }
    catch (const domain_error &de)
    {
        std::cerr << "Domain_error: " << de.what() << '\n';
    }
}

Rainfall &StackAsLinkedList::Top() const
{
    // try to return the last item, throw a "stack is empty" error if there isn't one
    try
    {
        return *list.Last();
    }
    catch (const domain_error &de)
    {
        throw domain_error("stack is empty");
    }
}