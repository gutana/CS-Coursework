#ifndef _LINKED_STACK
#define _LINKED_STACK

#include "StackADT.h"
#include "Tile.h"
#include "TileNode.h"

/*
    LinkedStack class, it is a subclass of the abstract class StackADT (an interface)
    Author: Daniel Page
    Date: Winter 2022
*/
class LinkedStack : public StackADT
{
private:
    TileNode *top; // the top of the stack
    int count;     // number of elements
public:
    LinkedStack();         // Constructor (sets top to nullptr and count=0)
    ~LinkedStack();        // Deconstructor (deletes each node in the linked list)
    void push(Tile *elem); // push make a new node, insert at the head of the linked list with elem in it (top of the stack).
    Tile *pop();           // pop the stack: remove the first node in the linked list, return its data.  If the stack is empty, return nullptr.
    Tile *peek();          // return the top element of the stack, do not remove the node that has the top element.
    int size();            // return the number of elements in the stack
    bool isEmpty();        // is the stack empty?
};
#endif