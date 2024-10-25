#include "Product.h"
#include "BSTNode.h"

// constructor, for a leaf node with no parent
BSTNode::BSTNode()
{
    data = nullptr;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    height = 0;
}

// constructor, for a leaf node with a known parent
BSTNode::BSTNode(BSTNode *parentNode)
{
    data = nullptr;
    parent = parentNode;
    left = nullptr;
    right = nullptr;
    height = 0;
}

// constructor for an internal node, with no known parent (possibly)
BSTNode::BSTNode(Product *myData)
{
    data = myData;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    height = 0;
}

// constructor for an internal node, with a known parent
BSTNode::BSTNode(BSTNode *parentNode, Product *myData)
{
    data = myData;
    parent = parentNode;
    left = nullptr;
    right = nullptr;
    height = 0;
}

// deconstructor
BSTNode::~BSTNode()
{
    // you are not responsible for deleting the attributes, the tree will do this.
}

bool BSTNode::isLeaf()
{
    // check if the left and right child are both null.  If true, it's a leaf!
    if (getLeft() == nullptr && getRight() == nullptr)
    {
        return true;
    }
    else
    {
        // not a leaf
        return false;
    }
}

bool BSTNode::isInternal()
{
    return !isLeaf(); // if it is not a leaf, it is internal
}

// get the left child
BSTNode *BSTNode::getLeft()
{
    return left;
}

// get the right child
BSTNode *BSTNode::getRight()
{
    return right;
}

// get the parent
BSTNode *BSTNode::getParent()
{
    return parent;
}

// get the data
Product *BSTNode::getData()
{
    return data;
}

// get the height of the node
int BSTNode::getHeight()
{
    return height;
}

// set the new left child
void BSTNode::setLeft(BSTNode *newLeft)
{
    left = newLeft;
}

// set the new right child
void BSTNode::setRight(BSTNode *newRight)
{
    right = newRight;
}

// set the new parent node
void BSTNode::setParent(BSTNode *newParent)
{
    parent = newParent;
}

// set the new data
void BSTNode::setData(Product *newData)
{
    data = newData;
}

// set the height
void BSTNode::setHeight(int newHeight)
{
    height = newHeight;
}