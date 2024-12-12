// Mohammad Al-Bayati
// 200339106
// CS340

// INPUT: program will take input at runtime via terminal

#include <iostream>
#include <string>

#include "structures/Stack.h"
#include "structures/Tree.h"

using std::string;
using std::vector;

enum NotationType
{
    Prefix,
    Infix,
    Postfix
};

enum MathematicalSymbol // Assume + and - have equal precedence, same with * and /
{
    Plus = 0,
    Minus = 0,
    Multiply = 1,
    Divide = 1,

    RightParenthesis = 2,
    LeftParenthesis = 3
};

// Will split a string into a vector of strings based on the char passed in.
// e.g. passing in ' ' will split a string by spaces
vector<string> splitString(string, char);

// Converts a given array of strings (in infix notation) to postix
void convertInfixToPostfix(vector<string> &);

// Converts a given char (+ - / *) into an enum MathematicalSymbol
MathematicalSymbol convertCharToMathSymbol(char);

// Returns true if char is + - / * or ( )
bool isOperator(char);
bool isParenthesis(char);

// Figures out which notation type our list of elements is using
NotationType getNotationType(vector<string> &);

int main(int argc, char *argv[])
{
    std::cout << "Hello! Welcome to the magic expression evaluator.\n";
    try
    {
        const char CHAR_TO_SPLIT_BY = ' ';

        while (true)
        {
            string inputExpression;
            std::cout << "Please enter your expression (q to quit): ";
            std::getline(std::cin, inputExpression);

            if (inputExpression == "q" || inputExpression == "Q")
            {
                std::cout << "Have a nice day!\n";
                exit(0);
            }

            vector<string> splitInput = splitString(inputExpression, CHAR_TO_SPLIT_BY);

            NotationType nt = getNotationType(splitInput);

            if (nt == Infix)
            {
                convertInfixToPostfix(splitInput);
            }
            else if (nt == Prefix)
            {
                throw("Prefix expression detected. Prefix expressions are not currently supported.");
            }

            TreeNode<string> *tree = TreeNode<string>::generateExpressionTree(splitInput);

            std::cout << "\nInfix: ";
            TreeNode<string>::printExpressionTree(tree, TraversalOrder::Inorder);
            std::cout << '\n';

            std::cout << "Prefix: ";
            TreeNode<string>::printExpressionTree(tree, TraversalOrder::Preorder);
            std::cout << '\n';

            std::cout << "Postfix: ";
            TreeNode<string>::printExpressionTree(tree, TraversalOrder::Postorder);
            std::cout << "\n\n";

            float result = TreeNode<string>::evaluateExpressionTree(tree);
            std::cout << "Result: " << result << "\n\n";

            TreeNode<string>::freeTree(tree);
        }
    }
    catch (const char *msg)
    {
        std::cout << "Error: " << msg << '\n';
    }
}

vector<string> splitString(string stringToSplit, char charToSplitBy)
{
    // the final vector of strings we will return
    vector<string> splitInput;

    // temp to hold our characters as we iterate until we reach a space or end of string
    string tempStr;
    for (int i = 0; i < stringToSplit.length(); i++)
    {
        char currentChar = stringToSplit[i];
        if (currentChar == charToSplitBy)
        {
            splitInput.push_back(tempStr);
            tempStr.clear();
        }
        // if we're at the end of the string (not a space, but str still needs to be added to vector)
        else if (i == stringToSplit.length() - 1)
        {
            tempStr.push_back(currentChar);
            splitInput.push_back(tempStr);
            tempStr.clear();
        }
        else
        {
            tempStr.push_back(currentChar);
        }
    }
    return splitInput;
}

bool isOperator(char c)
{
    if (c == '+' || c == '-' || c == '/' || c == '*')
    {
        return true;
    }
    return false;
}

bool isParenthesis(char c)
{
    if (c == '(' || c == ')')
    {
        return true;
    }
    return false;
}

NotationType getNotationType(vector<string> &elements)
{
    // Check for postfix first
    // Basically check last element, if it's 1 char long and an operator, it's postfix
    string element = elements.back();
    if (element.length() == 1 && isOperator(element[0]))
        return NotationType::Postfix;

    // Check first element to see if it's prefix
    element = elements.front();
    if (element.length() == 1 && isOperator(element[0]))
        return NotationType::Prefix;

    // if it's not prefix or postfix, it's infix
    return NotationType::Infix;
}

void convertInfixToPostfix(vector<string> &v)
{
    vector<string> output;
    Stack<string> operatorStack;

    for (auto str : v)
    {
        bool isOp = (str.length() == 1) && isOperator(str[0]);
        bool isParen = isParenthesis(str[0]);

        // if operand, immediately place it into output
        if (!isOp && !isParen)
        {
            output.push_back(str);
            continue;
        }

        if (isParen)
        {
            if (convertCharToMathSymbol(str[0]) == MathematicalSymbol::LeftParenthesis)
            {
                operatorStack.Push(str);
                continue;
            }
            else // if right paren
            {
                // if we see right paren, pop the stack and write symbols until we encounter a left paren
                // which is popped and not output
                while (operatorStack.Size() > 0)
                {
                    string poppedOperator = operatorStack.Pop();

                    if (poppedOperator[0] == '(')
                        break;

                    output.push_back(poppedOperator);
                }
                continue;
            }
        }

        // if operator and not output, place onto stack
        if (isOp)
        {
            MathematicalSymbol operatorOnTopStack;

            if (operatorStack.Size() > 0)
                operatorOnTopStack = convertCharToMathSymbol(operatorStack.Top()[0]);

            MathematicalSymbol currentOperator = convertCharToMathSymbol(str[0]);

            // if stack.top has lower precedence, or if stack is empty output nothing and push onto stack
            if (((operatorStack.Size() > 0) && (operatorOnTopStack <= currentOperator)) || (operatorStack.Size() == 0))
            {
                operatorStack.Push(str);
            }
            else // if there's something on stack and it's higher priority than current
            {
                // pop entries from stack until we find an entry of lower priority.
                // never remove a '('
                if (operatorStack.Top() == "(")
                {
                    operatorStack.Push(str);
                    continue;
                }

                string popped;
                while (operatorStack.Size() != 0)
                {
                    // NEVER remove a '('
                    if (operatorStack.Top() == "(")
                    {
                        break;
                    }

                    popped = operatorStack.Pop();
                    output.push_back(popped);

                    MathematicalSymbol poppedOperator = convertCharToMathSymbol(popped[0]);

                    // if we found operator of lower priority, we're done
                    if (poppedOperator <= currentOperator)
                        break;
                };
                // When done popping, push operator onto stack
                operatorStack.Push(str);
            }
            continue;
        }
    }

    // if end of input, pop until it is empty, writing symbols onto output
    while (operatorStack.Size() > 0)
    {
        string poppedOperator = operatorStack.Pop();
        output.push_back(poppedOperator);
    }
    v = output;
}

MathematicalSymbol convertCharToMathSymbol(char c)
{
    if (c == '+')
        return MathematicalSymbol::Plus;
    if (c == '-')
        return MathematicalSymbol::Minus;
    if (c == '*')
        return MathematicalSymbol::Multiply;
    if (c == '/')
        return MathematicalSymbol::Divide;
    if (c == '(')
        return MathematicalSymbol::LeftParenthesis;
    if (c == ')')
        return MathematicalSymbol::RightParenthesis;

    std::cout << "Error: Unexpected symbol " << c << '\n';
    throw("Unexpected symbol: " + c);
}
