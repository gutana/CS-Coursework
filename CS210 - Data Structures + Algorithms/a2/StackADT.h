#ifndef _STACK_ADT
#define _STACK_ADT
#include "Tile.h"
/*
    StackADT
    Author: Daniel Page
    Date: Winter 2022
*/
class StackADT
{ // An example Stack ADT
public:
    virtual void push(Tile *elem) = 0; // push elem onto the top of the stack.
    virtual Tile *pop() = 0;           // pop the top element off the stack and return it.
    virtual Tile *peek() = 0;          // return the element that is on the top of the stack.
    virtual int size() = 0;            // how many elements are in the stack?
    virtual bool isEmpty() = 0;        // is the stack empty?
};
#endif