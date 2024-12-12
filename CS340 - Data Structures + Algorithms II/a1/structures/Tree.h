#pragma once

#include <vector>

enum TraversalOrder
{
    Preorder,
    Inorder,
    Postorder
};

template <typename T>
class TreeNode
{
public:
    T data;
    TreeNode<T> *left;
    TreeNode<T> *right;

    static TreeNode<T> *generateExpressionTree(std::vector<T> &);
    static float evaluateExpressionTree(TreeNode<T> *);
    static void printExpressionTree(TreeNode<T> *, TraversalOrder);
    static void freeTree(TreeNode<T> *);

};