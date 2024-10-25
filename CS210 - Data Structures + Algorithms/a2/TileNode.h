#ifndef _NODE
#define _NODE

#include "Tile.h"

/*
    Node class
    This is a node for a linked list.
    Author: Daniel Page
    Date: Winter 2022
*/
class TileNode
{
private:
    Tile *data;     // the data for the node
    TileNode *next; // the next node
public:
    TileNode(Tile *d); // constructor
    // note: there is nothing to deallocate (for a deconstructor), let the linked list take care of it.
    Tile *getData();                 // get the data of the node.
    TileNode *getNext();             // get the next node in the linked list
    void setData(Tile *newData);     // set the data for the node
    void setNext(TileNode *newNext); // set the next node
};
#endif