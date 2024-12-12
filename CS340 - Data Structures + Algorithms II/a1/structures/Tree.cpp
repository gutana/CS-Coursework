#include "Tree.h"
#include "Stack.h"

#include <vector>
#include <string>
#include <iostream>

template <typename T>
TreeNode<T> *TreeNode<T>::generateExpressionTree(std::vector<T> &postfixExpression)
{
    Stack<TreeNode<T> *> stack;

    bool isOperator(char);

    for (T s : postfixExpression)
    {
        if (isOperator(s[0]) && s.length() == 1)
        {
            TreeNode<T> *node = new TreeNode<T>();

            TreeNode<T> *rhs = stack.Pop();
            TreeNode<T> *lhs = stack.Pop();

            node->left = lhs;
            node->right = rhs;
            node->data = s;

            stack.Push(node);
        }
        else
        {
            TreeNode<T> *node = new TreeNode<T>();
            node->data = s;
            stack.Push(node);
        }
    }
    return stack.Pop();
}

template <typename T>
float TreeNode<T>::evaluateExpressionTree(TreeNode<T> *node)
{
    if (node->data == "*")
        return evaluateExpressionTree(node->left) * evaluateExpressionTree(node->right);
    else if (node->data == "/")
        return evaluateExpressionTree(node->left) / evaluateExpressionTree(node->right);
    else if (node->data == "+")
        return evaluateExpressionTree(node->left) + evaluateExpressionTree(node->right);
    else if (node->data == "-")
        return evaluateExpressionTree(node->left) - evaluateExpressionTree(node->right);
    else
    {
        try
        {
            return std::stof(node->data);
        }
        catch (std::invalid_argument err)
        {
            std::cerr << "Invalid operand. Expected number; recieved '" << node->data << "'.\n";
            exit(-1);
        }
    }
}

template <typename T>
void TreeNode<T>::printExpressionTree(TreeNode<T> *node, TraversalOrder order)
{
    // if one of our children is null, we know that this one has to be a numeric value
    // Just print the val and return
    // (to avoid excessive parentheses)
    if (node->left == nullptr)
    {
        std::cout << node->data << ' ';
        return;
    }

    if (order == TraversalOrder::Inorder)
    {
        std::cout << "( ";
        if (node->left != nullptr)
        {
            printExpressionTree(node->left, order);
        }

        std::cout << node->data << ' ';

        if (node->right != nullptr)
            printExpressionTree(node->right, order);

        std::cout << ") ";
    }
    else if (order == TraversalOrder::Postorder)
    {
        if (node->left != nullptr)
            printExpressionTree(node->left, order);

        if (node->right != nullptr)
            printExpressionTree(node->right, order);

        std::cout << node->data << ' ';
    }
    else if (order == TraversalOrder::Preorder)
    {
        std::cout << node->data << ' ';

        if (node->left != nullptr)
            printExpressionTree(node->left, order);

        if (node->right != nullptr)
            printExpressionTree(node->right, order);
    }
}

template <typename T>
void TreeNode<T>::freeTree(TreeNode<T> *node)
{
    if (node == nullptr)
        return;

    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

// Let the compiler know we will want a string version of this
template class TreeNode<std::string>;
