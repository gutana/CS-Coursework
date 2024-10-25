#ifndef BST_NODE
#define BST_NODE
#include "Product.h"
#include <string>

class BSTNode
{
private:
    Product *data;
    BSTNode *parent;
    BSTNode *left;
    BSTNode *right;
    int height = 0; // the height of the node, by default it is zero.
public:
    BSTNode();                                     // constructor for a leaf node that has no parent known, default constructor
    BSTNode(BSTNode *parentNode);                  // constructor for a leaf node that has a parent known.
    BSTNode(Product *myData);                      // constructor for an internal node
    BSTNode(BSTNode *parentNode, Product *myData); // constructor for an internal node where the parent is known
    ~BSTNode();                                    // deconstructor
    bool isLeaf();                                 // returns true if the node is a leaf, false otherwise
    bool isInternal();                             // returns true if the node is an internal node, false otherwise
    BSTNode *getLeft();                            // get the left child
    BSTNode *getRight();                           // get the right child
    BSTNode *getParent();                          // get the parent
    Product *getData();                            // get the data of the node, a product.
    int getHeight();                               // get the height of the node
    void setLeft(BSTNode *newLeft);                // set the left child
    void setRight(BSTNode *newRight);              // set the right child
    void setParent(BSTNode *newParent);            // set the parent
    void setData(Product *newData);                // set the data
    void setHeight(int newHeight);                 // set the height to a new value
};
#endif