#include "AVLTree.h"
#include "DuplicateKeyException.h"
#include "NoKeyException.h"

#include <iostream>

// Mohammad Al-Bayati
// 200339106
// CS210 - Daniel Page
//
// This file is the implementation of an AVLtree.
// Note: Rotate function is not working 100% properly, but works for many cases

AVLTree::AVLTree() // constructs an empty BST.
    : root(new BSTNode()), numRecords(0)
{
}

AVLTree::~AVLTree() // delete the tree
{
    deleteTree(root);
}

// =============
// BST functions
// =============

Product *AVLTree::get(int key)
{
    return get(root, key)->getData();
}

BSTNode *AVLTree::get(BSTNode *r, int key) // get the node that contains the key, if exists (nullptr otherwise)
{
    if (r->isLeaf()) //  If the node wasn't found, return the leaf where it would've been
        return r;

    if (key == r->getData()->getKey()) // If node found
        return r;

    if (key < r->getData()->getKey()) // If key we're looking for is left
        return get(r->getLeft(), key);

    return get(r->getRight(), key); // If key we're looking for is right
}

void AVLTree::put(Product *prod) // put for a BST (not necessarily AVL)
{
    bool success = put(root, prod);

    if (!success)
        throw DuplicateKeyException();
}

bool AVLTree::put(BSTNode *r, Product *prod) // put a record into the BST, throws DuplicateKeyException if duplicate key encountered.  Returns true if successful
{
    BSTNode *p = get(root, prod->getKey()); // Get location where we should put node

    if (p->isInternal()) // If there's already something there
        return false;

    // Set the node with the prod, give it children
    p->setData(prod);
    p->setLeft(new BSTNode(p));
    p->setRight(new BSTNode(p));

    numRecords++;
    return true;
}

void AVLTree::remove(int key) // remove for a BST
{
    void *success = remove(root, key);

    if (!success)
        throw NoKeyException();
}

BSTNode *AVLTree::remove(BSTNode *r, int key) // remove the node, throws the NoKeyException if the key is not found in the tree.
{
    BSTNode *p = get(r, key); // Get node where key should be

    if (p->isLeaf())    // If node is empty
        return nullptr; // the calling function will throw exception if we return nullptr

    if (p->getLeft()->isLeaf() || p->getRight()->isLeaf()) // If at least one of the children is a leaf
    {
        BSTNode *pParent = p->getParent();

        BSTNode *child = p->getLeft()->isLeaf() ? p->getRight() : p->getLeft(); // Child is whichever one is internal, or empty node.

        if (p == root)
        {
            root = child; // Set child to be the new root if the parent was.
            root->setParent(nullptr);
        }
        else
        {
            child->setParent(pParent);
            p->getLeft()->isLeaf() ? pParent->setRight(child) : pParent->setLeft(child); // Let the parent know they have a new child

            if (child->isLeaf())
            {
                (p == pParent->getRight()) ? pParent->setRight(child) : pParent->setLeft(child);
            }
            else
            {
                pParent->getData()->getKey() < child->getData()->getKey() ? pParent->setRight(child) : pParent->setLeft(child); // Let the parent know they have a new child
            }
        }
        delete p->getData();
        delete p;
        numRecords--;
        return child;
    }
    else // If both children are internal nodes
    {
        BSTNode *s = smallest(p->getRight()); // get successor

        p->getData()->setKey(s->getData()->getKey());
        p->getData()->setName(s->getData()->getName());
        p->getData()->setQuantity(s->getData()->getQuantity());

        remove(s, s->getData()->getKey());
    }
    return p;
}

Product *AVLTree::smallest() // returns the record with the smallest key
{
    BSTNode *node = smallest(root);

    if (node)
        return node->getData();

    return nullptr; // If node was empty, return nullptr
}

BSTNode *AVLTree::smallest(BSTNode *r) // returns the node containing the record with the smallest key (nullptr otherwise)
{
    if (r->isLeaf())
        return nullptr;

    BSTNode *p = r;

    while (p->getData()) // loop until we get to a leaf node
    {
        p = p->getLeft();
    }
    return p->getParent(); // return that leaf node's parent
}

void AVLTree::deleteTree(BSTNode *r) // delete the BST nodes using a post-order traversal.
{
    // Delete children, then ourself
    if (r->isInternal())
    {
        deleteTree(r->getRight());
        deleteTree(r->getLeft());
        delete r->getData();
    }
    delete r;
}

// =============
// AVL tree functions
// =============

void AVLTree::putAVL(Product *prod) // put for an AVL tree
{
    try
    {
        put(prod);
    }
    catch (DuplicateKeyException)
    {
        throw DuplicateKeyException();
    }

    BSTNode *node = get(root, prod->getKey()); // Get node where we put the prod so we can rebalance around it
    rebalanceAVL(node);
}

void AVLTree::removeAVL(int key) // remove for an AVL tree
{
    BSTNode *node = remove(root, key);
    rebalanceAVL(node);
}

void AVLTree::recomputeHeight(BSTNode *p) // update the height of node p.
{
    int leftHeight = 0;
    int rightHeight = 0;
    if (p && p->getLeft())
    {
        leftHeight = p->getLeft()->getHeight();
    }
    if (p && p->getRight())
    {
        rightHeight = p->getRight()->getHeight();
    }
    if (p)
        p->setHeight(leftHeight > rightHeight ? ++leftHeight : ++rightHeight); // choose max of left/right height and add one to it
}

BSTNode *AVLTree::taller(BSTNode *x, bool onLeft) // return the taller of its children.  On tie, it picks the same side as from its parent.
{
    if (x->getLeft()->getHeight() > x->getRight()->getHeight())
    {
        return x->getLeft();
    }
    if (x->getLeft()->getHeight() < x->getRight()->getHeight())
    {
        return x->getRight();
    }
    return (onLeft ? x->getLeft() : x->getRight()); // If there's a tie
}

BSTNode *AVLTree::rotate(BSTNode *x) // perform rotate(x), returns the root of the subtree.
{
    // I have no idea what I'm doing

    bool parentWasRoot = x->getParent() == root;
    // if we're on the left side of our parent
    if (x->getData()->getKey() < x->getParent()->getData()->getKey())
    {
        BSTNode *temp = x->getRight();
        x->setRight(x->getParent());

        if (x->getParent() != root)
        {
            if (x->getParent()->getParent())
                x->getParent()->getParent()->setRight(x);

            x->getParent()->setRight(temp);
            x->setParent(x->getParent()->getParent());
        }
        x->getRight()->setLeft(new BSTNode);

        if (parentWasRoot)
            root = x;
    }
    else // if we're on the right side of our parent
    {
        BSTNode *temp = x->getLeft();
        x->setLeft(x->getParent());
        if (x->getParent() != root)
        {
            if (x->getParent()->getParent())
                x->getParent()->getParent()->setLeft(x);
            x->getParent()->setLeft(temp);
            x->setParent(x->getParent()->getParent());
        }
        x->getLeft()->setRight(new BSTNode);

        if (parentWasRoot)
            root = x;
    }
    recomputeHeight(x->getLeft());
    recomputeHeight(x->getRight());
    recomputeHeight(x);
    if (parentWasRoot)
        x->setParent(nullptr);
    return x;
}

BSTNode *AVLTree::rotation(BSTNode *z, BSTNode *y, BSTNode *x) // performs the rotation, returns the root of the subtree.
{
    if (y->getData()->getKey() < z->getData()->getKey()) // if y is left child of z (L)
    {
        if (x->getData()->getKey() < y->getData()->getKey()) // if x is left child of y (LL)
        {
            return rotate(y); // LL Rotation
        }
        // if x isn't the left child, then it must be the right child (LR). double rotation
        rotate(x);
        return rotate(x);
    }
    // we get here if y is the right child (R)
    if (x->getData()->getKey() > y->getData()->getKey()) // is x the right child of y?
    {
        return rotate(y); // RR Rotation
    }
    rotate(x);
    return rotate(x);
}

void AVLTree::rebalanceAVL(BSTNode *v)
{
    if (v && !v->isLeaf())
        recomputeHeight(v);

    while (v && v != root) // Go up tree until we're at root
    {
        v = v->getParent();

        int difference = 0;
        if (v)
            difference = v->getLeft()->getHeight() - v->getRight()->getHeight();

        if (difference > 1 || difference < -1) // If absolute of difference between heights of the subtrees is above 1
        {
            bool vOnLeft = (v->getParent() && v->getParent()->getData()->getKey() > v->getData()->getKey());
            BSTNode *y = taller(v, vOnLeft);

            bool yOnLeft = (y->getParent() && y->getParent()->getData()->getKey() > y->getData()->getKey());
            BSTNode *x = taller(y, yOnLeft);

            v = rotation(v, y, x); // rotate
        }
        recomputeHeight(v);
    }
}

// ==============
// misc functions
// ==============

int AVLTree::size() // getting the number of records.
{
    return numRecords;
}

bool AVLTree::isEmpty() // is the tree empty?
{
    return (numRecords == 0);
}

std::vector<Product *> AVLTree::treeSort() // return a vector containing pointers to every Product object, sorted.
{
    std::vector<Product *> v; // Create the vector, pass it to the sort recursive function, then return it
    sortRec(root, v);
    return v;
}

void AVLTree::sortRec(BSTNode *r, std::vector<Product *> &sortedList) // inorder traversal of the BST.
{
    if (r->getData())
    {
        sortRec(r->getLeft(), sortedList);
        sortedList.push_back(r->getData());
        sortRec(r->getRight(), sortedList);
    }
}

void AVLTree::updateProductName(int key, std::string newName) // update the name of the product with key, if it exists
{
    Product *product = get(key);
    if (product)
        product->setName(newName);
}

void AVLTree::printLevelOrderKeys()
{
    std::queue<BSTNode *> treeQueue;
    treeQueue.push(root);
    std::cout << "[";

    while (!treeQueue.empty())
    {
        BSTNode *node = treeQueue.front(); // get the element at the front of queue
        if (node->getData() == nullptr)
        {
            std ::cout << "[null]";
        }
        else
        {
            std ::cout << "[" << std::to_string(node->getData()->getKey()) << "]";
        }
        treeQueue.pop(); // removes first element (in a queue)
        if (node->getLeft() != nullptr)
        {
            treeQueue.push(node->getLeft()); // enqueue left
        }
        if (node->getRight() != nullptr)
        {
            treeQueue.push(node->getRight()); // enqueue right
        }
    } // while there is still a node
    std ::cout
        << "]"
        << std::endl;
}

Product *AVLTree::highestQuantity() // returns a pointer to the product with the highest quantity.
{
    return highestQuantityRec(root);
}

Product *AVLTree::highestQuantityRec(BSTNode *r) // postorder traversal to find the product with the highest quantity
{
    if (!r->getData())
        return nullptr;

    Product *highestLeft = highestQuantityRec(r->getLeft());
    Product *highestRight = highestQuantityRec(r->getRight());

    if (!highestLeft && !highestRight) // If both children empty, then we just return ourselves
        return r->getData();

    if (highestLeft && !highestRight) // If right is empty, return highest of this node and the left
        return (highestLeft->getQuantity() > r->getData()->getQuantity() ? highestLeft : r->getData());

    if (!highestLeft && highestRight) // If left is empty, return highest of this node and the right
        return (highestRight->getQuantity() > r->getData()->getQuantity() ? highestRight : r->getData());

    if (highestLeft->getQuantity() > highestRight->getQuantity()) // If left higher than right, return the highest of this or left
    {
        return (highestLeft->getQuantity() > r->getData()->getQuantity() ? highestLeft : r->getData());
    }
    else // Return the highest of the right and this
    {
        return (highestRight->getQuantity() > r->getData()->getQuantity() ? highestRight : r->getData());
    }
}