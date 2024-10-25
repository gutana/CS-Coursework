#include "WordData.h"
#include "WordNode.h"

/*
    Record node class
    WordData*his is a node for a linked list.
    Author: Daniel Page
    Date: Winter 2022
*/

// default constructor
WordNode::WordNode()
{
    this->data = nullptr; // set all elements to nullptr
    this->next = nullptr;
}

// constructor for a node with data
WordNode::WordNode(WordData *data)
{
    this->data = data;
    this->next = nullptr;
}

// get the data
WordData *WordNode::getData()
{
    return this->data;
}

// get the next node
WordNode *WordNode::getNext()
{
    return next;
}

// update the data of the node
void WordNode::setData(WordData *data)
{
    this->data = data;
}

// update the next node of the node
void WordNode::setNext(WordNode *next)
{
    this->next = next;
}