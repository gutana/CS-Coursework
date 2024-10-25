#include "LinkedStack.h"
/*
Mohammad Al-Bayati
200339106 - CS210

The following is an implementation of the LinkedStack class.

*/

LinkedStack::LinkedStack() // Constructor (sets top to nullptr and count=0)
    : top(nullptr), count(0)
{
}

LinkedStack::~LinkedStack()
// Deconstructor (deletes each node in the linked list)
{
    TileNode *tile = top;

    while (tile != nullptr)
    {
        TileNode *tileNodeToDelete = tile;
        tile = tile->getNext(); // If we're on the last element, this will be nullptr, so won't loop
        delete tileNodeToDelete;
    }
}

void LinkedStack::push(Tile *elem) // push make a new node, insert at the head of the linked list with elem in it (top of the stack).
{
    count++;

    TileNode *oldTop = top; // Pointer to old top. if stack is empty, this will be a nullptr

    top = new TileNode(elem);
    top->setNext(oldTop); // top now either points to next element (old top) or nullptr, depending on if stack was empty prior
}

Tile *LinkedStack::pop() // pop the stack: remove the first node in the linked list, return its data.  If the stack is empty, return nullptr.
{
    if (isEmpty())
        return nullptr;

    count--;

    Tile *tileToReturn = top->getData(); // We'll be returning the data from the top tile
    TileNode *tileNodeToDelete = top;    // We'll be deleting the node that held the top tile

    top = top->getNext(); // Move top to the next element in the stack now

    delete tileNodeToDelete;
    return tileToReturn;
}

Tile *LinkedStack::peek() // return the top element of the stack, do not remove the node that has the top element.
{
    if (isEmpty()) // If empty, return null pointer
        return nullptr;

    return top->getData(); // Return data at the top without deleting
}

int LinkedStack::size() // return the number of elements in the stack
{
    return count;
}

bool LinkedStack::isEmpty() // is the stack empty?
{
    if (count == 0) // if count == 0, it's empty. Otherwise, it's not.
        return true;

    return false;
}