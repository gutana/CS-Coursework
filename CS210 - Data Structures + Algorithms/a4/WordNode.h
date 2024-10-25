#ifndef _NODE
#define _NODE
#include "WordData.h"
/*
    Record node class
    This is a node for a linked list containing WordData.
    Author: Daniel Page
    Date: Winter 2022
*/
class WordNode
{
private:
    WordData *data; // the data for the node
    WordNode *next; // the next node
public:
    WordNode();               // default constructor
    WordNode(WordData *data); // constructor
    // note: there is nothing to deallocate (for a deconstructor), let the linked list take care of it.
    WordData *getData();          // get the data of the node.
    WordNode *getNext();          // get the next node in the linked list
    void setData(WordData *data); // set the data for the node
    void setNext(WordNode *next); // set the next node
};
#endif