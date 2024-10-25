#ifndef AVL_TREE
#define AVL_TREE
#include "BSTNode.h"
#include <vector>
#include <queue>

class AVLTree
{
private:
    BSTNode *root;  // the root node
    int numRecords; // number of records

    // BST specific functions
    bool put(BSTNode *r, Product *prod);                          // put a record into the BST, throws DuplicateKeyException if duplicate key encountered.  Returns true if successful
    BSTNode *remove(BSTNode *r, int key);                         // remove the node, throws the NoKeyException if the key is not found in the tree.
    BSTNode *get(BSTNode *r, int key);                            // get the node that contains the key, if exists (nullptr otherwise)
    BSTNode *smallest(BSTNode *r);                                // returns the node containing the record with the smallest key (nullptr otherwise)
    void sortRec(BSTNode *r, std::vector<Product *> &sortedList); // inorder traversal of the BST.
    Product *highestQuantityRec(BSTNode *r);                      // postorder traversal to find the product with the highest quantity
    void deleteTree(BSTNode *r);                                  // delete the BST nodes using a post-order traversal.

    // AVL tree specific functions
    void recomputeHeight(BSTNode *p);                      // update the height of node p.
    BSTNode *taller(BSTNode *x, bool onLeft);              // return the taller of its children.  On tie, it picks the same side as from its parent.
    BSTNode *rotate(BSTNode *x);                           // perform rotate(x), returns the root of the subtree.
    BSTNode *rotation(BSTNode *z, BSTNode *y, BSTNode *x); // performs the rotation, returns the root of the subtree.
    void rebalanceAVL(BSTNode *v);                         // rebalances the tree after insert/removal.
public:
    AVLTree();  // constructs an empty BST.
    ~AVLTree(); // will call deleteTree

    // BST functions
    Product *get(int key);
    void put(Product *prod); // put for a BST (not necessarily AVL)
    void remove(int key);    // remove for a BST (not necessarily AVL)
    Product *smallest();     // returns the record with the smallest key
    // AVL tree functions
    void putAVL(Product *prod); // put for an AVL tree
    void removeAVL(int key);    // remove for an AVL tree

    // misc functions
    int size();                                           // getting the number of records.
    bool isEmpty();                                       // is the tree empty?
    std::vector<Product *> treeSort();                    // return a vector containing pointers to every Product object, sorted.
    Product *highestQuantity();                           // returns a pointer to the product with the highest quantity.
    void updateProductName(int key, std::string newName); // update the name of the product with key, if it exists
    void printLevelOrderKeys();                           // prints the tree in level order (uses a queue from std, make sure to include<queue>).
};
#endif