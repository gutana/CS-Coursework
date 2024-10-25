#include "TileNode.h"
/*
Mohammad Al-Bayati
200339106 - CS210

The following is an implementation of the TileNode class.

*/

TileNode::TileNode(Tile *d)  // constructor
    : data(d), next(nullptr) // member initializer list
{
}

Tile *TileNode::getData() // get the data of the node.
{
    return data;
}

TileNode *TileNode::getNext() // get the next node in the linked list
{
    return next;
}

void TileNode::setData(Tile *newData) // set the data for the node
{
    data = newData;
}

void TileNode::setNext(TileNode *newNext) // set the next node
{
    next = newNext;
}