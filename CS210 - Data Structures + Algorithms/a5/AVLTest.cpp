#include <string>
#include "Product.h"
#include "BSTNode.h"
#include "DuplicateKeyException.h"
#include "NoKeyException.h"
#include "AVLTree.h"
#include <iostream>
#include <vector>
#include <exception>

/*
    A basic test program for the BST/AVL functions as a dictionary.
    Author: Daniel Page
    Date: Winter 2022
    Updated V2 - Includes tests
*/

// test 1 - create an empty BST, it must have a leaf node only in it, then destroys it.
bool testOne()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 1..." << std::endl;
        AVLTree *myBST = new AVLTree();
        if (myBST->size() != 0)
        { // no records
            testResult = false;
            std::cout << "T1 failed, the size should be 0." << std::endl;
            return false;
        }
        std::cout << "T1 Expected:\n"
                  << "[[null]]" << std::endl;
        std::cout << "T1 Output:" << std::endl;
        myBST->printLevelOrderKeys();
        std::cout << "T1 passes if the output matches what is expected..." << std::endl;
        delete myBST; // call the deconstructor/destructor!
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "T1 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 2, BST - 3 put operations happen, sees the keys are entered correctly (and size is updated)
bool testTwo()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 2..." << std::endl;
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(3); // put with key 3
        myBST->put(myProd);
        myProd = new Product(2); // put with key 2
        myBST->put(myProd);
        myProd = new Product(4); // put with key 4
        myBST->put(myProd);
        if (myBST->size() != 3)
        { // must have 3 records
            testResult = false;
            std::cout << "T2 failed, the size should be 3." << std::endl;
            return false;
        }
        std::cout << "T2 Expected:\n"
                  << "[[3][2][4][null][null][null][null]]" << std::endl;
        std::cout << "T2 Output:" << std::endl;
        myBST->printLevelOrderKeys();
        std::cout << "T2 passes if the output matches what is expected..." << std::endl;
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "T2 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 3, BST - 5 put operations happen, then computes smallest...
bool testThree()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 3...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(6); // put with key 6
        myBST->put(myProd);
        myProd = new Product(3); // put with key 3
        myBST->put(myProd);
        myProd = new Product(2); // put with key 2
        myBST->put(myProd);
        myProd = new Product(1); // put with key 1
        myBST->put(myProd);
        myProd = new Product(4); // put with key 4
        myBST->put(myProd);
        if (myBST->size() != 5)
        { // must have 3 records
            testResult = false;
            std::cout << "T3 failed, the size should be 5." << std::endl;
            return false;
        }
        if (myBST->smallest()->getKey() != 1)
        {
            testResult = false;
            std::cout << "T3 failed, smallest should 1." << std::endl;
        }
        testResult = true;
        std::cout << "PASSES" << std::endl;
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "T3 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 4, BST - some put operations happen, one of them should throw the DuplicateKeyException, then a remove is attempted where a NoKeyException should be thrown after that...
bool testFour()
{
    bool testResult = false;
    AVLTree *myBST = new AVLTree();
    Product *myProd;
    try
    {
        std::cout << "--Test 4...";
        myProd = new Product(6); // put with key 6
        myBST->put(myProd);
        myProd = new Product(7); // put with key 7
        myBST->put(myProd);
        myProd = new Product(5); // put with key 5
        myBST->put(myProd);
        myProd = new Product(4); // put with key 4
        myBST->put(myProd);
        myProd = new Product(6); // put with key 6 -DUPLICATE KEY EXCEPTION
        myBST->put(myProd);
        testResult = false;
        std::cout << "FAILS... DuplicateKeyException should be thrown" << std::endl;
        return true;
    }
    catch (DuplicateKeyException)
    {
        // it should go here!
    }
    catch (std::exception &e)
    {
        std::cout << "T4 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    try
    {
        myBST->remove(100); // no record exists with this!
    }
    catch (NoKeyException)
    {
        // it should go here!
        std::cout << "PASSES" << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "T4 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 5, BST -  builds a basic BST, then does 2 get operations (one should return nullptr)
bool testFive()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 5...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(3); // put with key 3
        myBST->put(myProd);
        myProd = new Product(6); // put with key 6
        myBST->put(myProd);
        myProd = new Product(9); // put with key 9
        myBST->put(myProd);
        myProd = new Product(2); // put with key 2
        myBST->put(myProd);
        Product *prodNotFound = myBST->get(8); // should fail to find
        Product *prodFound = myBST->get(9);    // should find
        if (prodNotFound == nullptr && prodFound != nullptr && prodFound->getKey() == 9)
        {
            std::cout << "PASSES" << std::endl;
            testResult = true;
            return true;
        }
        else
        {
            testResult = false;
            std::cout << "FAILS...a get operation failed to work as expected" << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T5 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 6, BST -  build a tree, performs treeSort.
bool testSix()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 6...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(5); // put with key 5
        myBST->put(myProd);
        myProd = new Product(8); // put with key 8
        myBST->put(myProd);
        myProd = new Product(6); // put with key 6
        myBST->put(myProd);
        myProd = new Product(7); // put with key 7
        myBST->put(myProd);
        myProd = new Product(1); // put with key 1
        myBST->put(myProd);
        myProd = new Product(2); // put with key 2
        myBST->put(myProd);
        myProd = new Product(4); // put with key 4
        myBST->put(myProd);
        myProd = new Product(3); // put with key 3
        myBST->put(myProd);
        std::vector<Product *> inorderProds = myBST->treeSort();
        // let us check that we read off 1, 2, 3, 4, ..., 8
        for (int i = 0; i <= 7; i++)
        {
            int myKey = inorderProds[i]->getKey(); // one higher than i
            if (myKey != (i + 1))
            {
                std::cout << "FAILS, check your inorder traversal possibly..." << std::endl;
                return false;
            }
        }
        std::cout << "PASSES" << std::endl;
        testResult = true;
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "T6 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 7, BST - size and isEmpty test
bool testSeven()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 7...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        if (!myBST->isEmpty())
        {
            std::cout << "FAILS...T7, no insertions happened yet, should be empty" << std::endl;
            return false;
        }
        for (int i = 0; i < 1000; i++)
        {                            // do a bunch of insertions (1000)
            myProd = new Product(i); // put with key i
            myBST->put(myProd);
        }
        if (myBST->size() == 1000 && !myBST->isEmpty())
        { // size is 1000 and is not empty
            std::cout << "PASSES" << std::endl;
            testResult = true;
            return true;
        }
        else
        {
            testResult = false;
            std::cout << "FAILS...either isEmpty result is false or size is wrong (should be 1000)" << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T7 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 8, BST - highestQuantity test, we insert some records, then must pick the one with the highest quantity
bool testEight()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 8...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(5, "", 10); // put with key 5
        myBST->put(myProd);
        myProd = new Product(1, "", 5); // put with key 1
        myBST->put(myProd);
        myProd = new Product(3, "", 4); // put with key 3
        myBST->put(myProd);
        myProd = new Product(2, "", 6); // put with key 2
        myBST->put(myProd);
        myProd = new Product(10, "", 20); // put with key 10
        myBST->put(myProd);
        myProd = new Product(4, "", 50); // put in key 4, has max quantity
        myBST->put(myProd);
        Product *high = myBST->highestQuantity();
        if (high != nullptr && high->getKey() == 4 && high->getQuantity() == 50)
        {
            std::cout << "PASSES" << std::endl;
            testResult = true;
            return true;
        }
        else
        {
            testResult = false;
            std::cout << "FAILS...highest quantity should be 50 (key 4)." << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T8 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 9, BST - 5 put operations happen, then update the name of a record, then check if it did get updated!
bool testNine()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 9...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(6); // put with key 6
        myBST->put(myProd);
        myProd = new Product(3); // put with key 3
        myBST->put(myProd);
        myProd = new Product(2); // put with key 2
        myBST->put(myProd);
        myProd = new Product(1); // put with key 1
        myBST->put(myProd);
        myProd = new Product(4); // put with key 4
        myBST->put(myProd);
        myBST->updateProductName(2, "name1"); // successful update
        myBST->updateProductName(8, "name1"); // your program should not update this name, no record exists with this key!
        if (myBST->get(2)->getKey() == 2 && myBST->get(2)->getName() == "name1")
        { // must update name, if exists!
            testResult = true;
            std::cout << "PASSES" << std::endl;
            return true;
        }
        else
        {
            testResult = false;
            std::cout << "FAILS.. the key is either wrong or the name of this record was not updated properly." << std::endl;
            return false;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T9 failed, something unexpected happened (perhaps it attempted to update a name of a product that didn't exist)..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 10, BST - build a BST, then remove node 4 (causes case 1)!
bool testTen()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 10...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(3); // put with key 3
        myBST->put(myProd);
        myProd = new Product(4); // put with key 4
        myBST->put(myProd);
        myProd = new Product(5); // put with key 5
        myBST->put(myProd);
        myBST->remove(4); // remove 4
        if (myBST->size() == 2)
        {
            // now check the level-order traversal!
            std::cout << "\nT10 Expected:\n"
                      << "[[3][null][5][null][null]]" << std::endl;
            std::cout << "T10 Output:" << std::endl;
            myBST->printLevelOrderKeys();
            std::cout << "T10 passes if the output matches what is expected..." << std::endl;
            return true;
        }
        else
        {
            std::cout << "FAILS, make sure the num of records is updated!" << std::endl;
            testResult = false;
            return false;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T10 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 11, BST - build a BST, then remove the root, put a new record in, then remove in a way that causes case 2!
bool testEleven()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 11...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(4); // put with key 4
        myBST->put(myProd);
        myProd = new Product(3); // put with key 3
        myBST->put(myProd);
        myProd = new Product(2); // put with key 2
        myBST->put(myProd);
        myProd = new Product(1); // put with key 1
        myBST->put(myProd);
        myBST->remove(4); // remove 4, the root...
        // now, let us put back in 4.  If removed correctly, no duplicate shall happen and the tree will not be all erased...
        myProd = new Product(4);
        myBST->put(myProd);
        // now let us remove 3, the new root
        myBST->remove(3); // causes case 2!
        // 4 should be the root after this again!
        if (myBST->size() == 3)
        {
            // now check the level-order traversal!
            std::cout << "\nT11 Expected:\n"
                      << "[[4][2][null][1][null][null][null]]" << std::endl;
            std::cout << "T11 Output:" << std::endl;
            myBST->printLevelOrderKeys();
            std::cout << "T11 passes if the output matches what is expected..." << std::endl;
            return true;
        }
        else
        {
            std::cout << "FAILS, make sure the num of records is updated!" << std::endl;
            testResult = false;
            return false;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T11 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 12, AVL - build a basic AVL tree, it does an RR rotation on 3.
bool testTwelve()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 12...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(1); // put with key 1
        myBST->putAVL(myProd);
        myProd = new Product(2); // put with key 2
        myBST->putAVL(myProd);
        myProd = new Product(3); // put with key 3
        myBST->putAVL(myProd);   // RR rotation happens here.
        if (myBST->size() == 3)
        {
            // now check the level-order traversal!
            std::cout << "\nT12 Expected:\n"
                      << "[[2][1][3][null][null][null][null]]" << std::endl;
            std::cout << "T12 Output:" << std::endl;
            myBST->printLevelOrderKeys();
            std::cout << "T12 passes if the output matches what is expected..." << std::endl;
            return true;
        }
        else
        {
            std::cout << "FAILS, make sure the num of records is updated!" << std::endl;
            testResult = false;
            return false;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T12 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 13, AVL - build a basic AVL tree, it does an LR rotation on inserting 7.
bool testThirteen()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 13...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(8); // put with key 8
        myBST->putAVL(myProd);
        myProd = new Product(5); // put with key 5
        myBST->putAVL(myProd);
        myProd = new Product(7); // put with key 7
        myBST->putAVL(myProd);   // LR rotation happens here.
        if (myBST->size() == 3)
        {
            // now check the level-order traversal!
            std::cout << "\nT13 Expected:\n"
                      << "[[7][5][8][null][null][null][null]]" << std::endl;
            std::cout << "T13 Output:" << std::endl;
            myBST->printLevelOrderKeys();
            std::cout << "T13 passes if the output matches what is expected..." << std::endl;
            return true;
        }
        else
        {
            std::cout << "FAILS, make sure the num of records is updated!" << std::endl;
            testResult = false;
            return false;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T13 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 14, AVL - putAVL then removeAVL; it does several rotations, RL-rotation and LL-rotations...
bool testFourteen()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 14...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(8); // put with key 8
        myBST->putAVL(myProd);
        myProd = new Product(10); // put with key 10
        myBST->putAVL(myProd);
        myProd = new Product(9);  // put with key 9
        myBST->putAVL(myProd);    // RL rotation happens
        myProd = new Product(12); // put with key 12
        myBST->putAVL(myProd);
        myProd = new Product(7); // put with key 7
        myBST->putAVL(myProd);
        myProd = new Product(6); // put with key 6
        myBST->putAVL(myProd);   // LL rotation will happen!
        myProd = new Product(5); // put with key 5
        myBST->putAVL(myProd);
        myBST->removeAVL(12); // LL rotation happens here!

        if (myBST->size() == 6)
        {
            // now check the level-order traversal!
            std::cout << "\nT14 Expected:\n"
                      << "[[7][6][9][5][null][8][10][null][null][null][null][null][null]]" << std::endl;
            std::cout << "T14 Output:" << std::endl;
            myBST->printLevelOrderKeys();
            std::cout << "T14 passes if the output matches what is expected..." << std::endl;
            return true;
        }
        else
        {
            std::cout << "FAILS, make sure the num of records is updated!" << std::endl;
            testResult = false;
            return false;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "T14 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

// test 15, AVL - the example of applying removeAVL from class!
bool testFifteen()
{
    bool testResult = false;
    try
    {
        std::cout << "--Test 15...";
        AVLTree *myBST = new AVLTree();
        Product *myProd;
        myProd = new Product(30); // put with key 30
        myBST->putAVL(myProd);
        myProd = new Product(20); // put with key 20
        myBST->putAVL(myProd);
        myProd = new Product(55); // put with key 55
        myBST->putAVL(myProd);
        myProd = new Product(10); // put with key 10
        myBST->putAVL(myProd);
        myProd = new Product(45); // put with key 45
        myBST->putAVL(myProd);
        myProd = new Product(25); // put with key 25
        myBST->putAVL(myProd);
        myProd = new Product(60); // put with key 60
        myBST->putAVL(myProd);
        myProd = new Product(15); // put with key 15
        myBST->putAVL(myProd);
        myProd = new Product(40); // put with key 40
        myBST->putAVL(myProd);
        myProd = new Product(50); // put with key 50
        myBST->putAVL(myProd);
        myProd = new Product(65); // put with key 65
        myBST->putAVL(myProd);
        myProd = new Product(35); // put with key 35
        myBST->putAVL(myProd);
        myBST->removeAVL(20); // LR rotation happens, then RL rotation happens...

        // now check the level-order traversal!
        std::cout << "\nT15 Expected:\n"
                  << "[[45][30][55][15][40][50][60][10][25][35][null][null][null][null][65][null][null][null][null][null][null][null][null]]" << std::endl;
        std::cout << "T15 Output:" << std::endl;
        myBST->printLevelOrderKeys();
        std::cout << "T15 passes if the output matches what is expected..." << std::endl;
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << "T15 failed, something unexpected happened..." << e.what() << std::endl;
        return false;
    }
    return testResult;
}

/*
    Run the tests!
*/
void runTests(int testStart)
{
    // we will total up the tests passed, wherever not stated it is worth 1 mark.
    if (testStart <= 1)
    {              // run test 1
        testOne(); // worth 0.5 marks
    }
    if (testStart <= 2)
    { // run test 2
        testTwo();
    }
    if (testStart <= 3)
    { // run test 3
        testThree();
    }
    if (testStart <= 4)
    { // run test 4
        testFour();
    }
    if (testStart <= 5)
    { // run test 5
        testFive();
    }
    if (testStart <= 6)
    { // run test 6
        testSix();
    }
    if (testStart <= 7)
    {                // run test 7
        testSeven(); // 0.5 marks
    }
    if (testStart <= 8)
    {                // run test 8
        testEight(); // 0.5 marks
    }
    if (testStart <= 9)
    {               // run test 9
        testNine(); // 0.5  marks
    }
    if (testStart <= 10)
    {              // run test 10
        testTen(); // 0.5 marks
    }
    if (testStart <= 11)
    {                 // run test 11
        testEleven(); // 0.5 marks
    }
    if (testStart <= 12)
    {                 // run test 12
        testTwelve(); // 0.5 marks
    }
    if (testStart <= 13)
    {                   // run test 13
        testThirteen(); // 0.5 marks
    }
    if (testStart <= 14)
    {                   // run test 14
        testFourteen(); // worth 0.5 marks
    }
    if (testStart <= 15)
    {                  // run test 15
        testFifteen(); // worth 0.5 marks
    }
}

int main(int argc, char *argv[])
{

    // you are encouraged to make your own AVLTree object and manipulate it (see code below for examples)

    // there is one command-line argument,
    // it decides which test to start at (enter number to test start at [optional])...
    int testStart = 0;
    if (argc > 1)
    {
        testStart = std::atoi(argv[1]);
    }

    /*
        Some tests, examine each test above to learn more...
    */
    runTests(testStart);

    // create a new BST (not AVL)
    AVLTree *myBST = new AVLTree();
    // building the following BST (ignoring the leaf nodes)
    /*
                4
               / \
              3   9
                 /
                8
               /
              5
               \
                6
                 \
                  7
    */
    std::cout << "Let's play with a simple BST!" << std::endl;
    Product *myProd;
    myProd = new Product(4, "HEMMED BARBER TOWEL", 3);
    myBST->put(myProd);
    myProd = new Product(9, "FRINGE END BARBER TOWEL", 5);
    myBST->put(myProd);
    myProd = new Product(8, "EXTRA GOOD QUALITY HEMMED BARBER TOWELS", 8);
    myBST->put(myProd);
    myProd = new Product(5, "EXTRA SIZE BARBER TOWELS", 10);
    myBST->put(myProd);
    myProd = new Product(6, "HONEYCOMB DISH TOWELS", 2);
    myBST->put(myProd);
    myProd = new Product(7, "GLASS TOWEL PURE WHITE", 12);
    myBST->put(myProd);
    myProd = new Product(3, "TEA TOWEL", 5);
    myBST->put(myProd);
    myBST->put(new Product(11, "DILDOS", 69));
    // let's print out some information!
    std::cout << "The BST contains: " << myBST->size() << " products." << std::endl;
    Product *smallest = myBST->smallest();
    std::cout << "The product with the smallest key " << smallest->getKey() << " is named..." << smallest->getName() << std::endl;
    std::cout << "The highest quantity product is: " << myBST->highestQuantity()->getName() << std::endl;
    Product *failedSearch = myBST->get(200); // this should fail
    if (failedSearch == nullptr)
    {
        std::cout << "The dictionary successfully failed to find a record with key 200" << std::endl;
    }
    else
    {
        std::cout << "The dictionary failed to determine that a record with key 200 was not in the dictionary" << std::endl;
    }
    std::cout << "Tree sorted by keys (inorder): " << std::endl;
    std::vector<Product *> sortedList = myBST->treeSort();
    for (int i = 0; i < sortedList.size(); i++)
    {
        std::cout << sortedList[i]->toString() << std::endl;
    }
    std::cout << "Level-order traversal of BST (with leaf nodes):" << std::endl;
    myBST->printLevelOrderKeys();
    // remove a record
    myBST->remove(4);   // performs case 2
    sortedList.clear(); // reset the vector
    sortedList = myBST->treeSort();
    std::cout << "Tree sort (after delete):" << std::endl;
    for (int i = 0; i < sortedList.size(); i++)
    {
        std::cout << sortedList[i]->toString() << std::endl;
    }
    std::cout << "Level-order traversal of BST (with leaf nodes), after delete:" << std::endl;
    myBST->printLevelOrderKeys();
    // remove another record
    myBST->remove(3);   // performs case 1 only
    sortedList.clear(); // reset the vector
    sortedList = myBST->treeSort();
    std::cout << "Tree sort (after delete):" << std::endl;
    for (int i = 0; i < sortedList.size(); i++)
    {
        std::cout << sortedList[i]->toString() << std::endl;
    }
    std::cout << "Level-order traversal of BST (with leaf nodes), after delete:" << std::endl;
    myBST->printLevelOrderKeys();
    // remove another record
    myBST->remove(5);   // performs case 1 only (but at the root)
    sortedList.clear(); // reset the vector
    sortedList = myBST->treeSort();
    std::cout << "Tree sort (after delete):" << std::endl;
    for (int i = 0; i < sortedList.size(); i++)
    {
        std::cout << sortedList[i]->toString() << std::endl;
    }
    std::cout << "Level-order traversal of BST (with leaf nodes), after delete:" << std::endl;
    myBST->printLevelOrderKeys();

    // now delete the BST
    delete myBST;

    std::cout << "My BST was deleted...\n"
              << std::endl;

    /*
        AVL Tree example
    */
    std::cout << "Let's play with a simple AVL tree!" << std::endl;
    AVLTree *myAVL = new AVLTree(); // empty AVL tree
    myProd = new Product(1, "BLEACHED BATH TOWEL", 10);
    myAVL->putAVL(myProd);
    myProd = new Product(2, "FRINGED TERRY WASH CLOTH", 3);
    myAVL->putAVL(myProd);
    myProd = new Product(3, "OUR CHOICEST AND FINEST HEMSTITCHED LINEN HUCK TOWEL", 50);
    myAVL->putAVL(myProd); // RR rotation happens here
    sortedList.clear();    // reset the vector
    sortedList = myBST->treeSort();
    std::cout << "Tree sort (after delete):" << std::endl;
    for (int i = 0; i < sortedList.size(); i++)
    {
        std::cout << sortedList[i]->toString() << std::endl;
    }
    std::cout << "Level-order traversal of BST (with leaf nodes), after delete:" << std::endl;
    myBST->printLevelOrderKeys();
    std::cout << "The AVL Tree contains: " << myAVL->size() << " products." << std::endl;
    smallest = myBST->smallest();
    std::cout << "The product with the smallest key " << smallest->getKey() << " is named..." << smallest->getName() << std::endl;
    std::cout << "The highest quantity product is: " << myAVL->highestQuantity()->getName() << std::endl;

    // you are encouraged to test more, try to make more situations!
    // try to apply removeAVL when you know rotations need to happen!

    delete myAVL; // delete the AVL tree
    std::cout << "My AVL Tree was deleted... done!" << std::endl;

    return 0;
}