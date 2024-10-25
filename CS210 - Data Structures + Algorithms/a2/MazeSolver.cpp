#include "Tile.h"
#include "TileNode.h"
#include "StackADT.h"
#include "LinkedStack.h"
#include <iostream>
#include <fstream>
#include <string>

/*
    printMaze
    Input: 2D array mazeArray of Tile objects, an integer for the dimension of the maze of Tile objects.
    Output:  Prints to the console the maze, based on the tile types.
*/
void printMaze(Tile **mazeArray, int mazeDimension)
{

    system("CLS"); // If this line gives you issues, just comment it out!
    for (int k = 0; k < mazeDimension * 2 + 1; k++)
    {
        std::cout << "-";
    }
    std::cout << "\n";
    for (int i = 0; i < mazeDimension; i++)
    {
        // consider the next row!
        for (int j = 0; j < mazeDimension; j++)
        {
            Tile *currTile = &mazeArray[i][j];
            std::cout << "|";
            std::string outputSymbol = "";
            if (currTile->getTileType() == 0)
            { // empty chamber
                outputSymbol = " ";
            }
            else if (currTile->getTileType() == 1)
            { // solid chamber
                outputSymbol = "X";
            }
            else if (currTile->getTileType() == 2)
            { // start chamber
                outputSymbol = "S";
            }
            else if (currTile->getTileType() == 3)
            { // end chamber
                outputSymbol = "E";
            }
            else if (currTile->getTileType() == 4)
            { // this is a type we are using for recording the path
                outputSymbol = "*";
            }
            std::cout << outputSymbol; // print the symbol
            // std::cout<<"|";
        }
        std::cout << "|\n";
    }
    for (int k = 0; k < mazeDimension * 2 + 1; k++)
    {
        std::cout << "-";
    }
    std::cout << "\n";
}

// our maze solving program!
int main(int argc, char *argv[])
{

    // ensure we give a text file to the program, formatted exactly as requested in the assignment
    if (argc <= 1)
    {
        std::cout << "ERROR: Please provide a maze (text file) in the manner formatted." << std::endl;
        return 2;
    }

    int mazeDimension = 0;
    std::ifstream readMaze;
    readMaze.open(argv[1]);    // open the maze file!
    readMaze >> mazeDimension; // get the dimension of the maze

    Tile *startTile = nullptr; // this will be the starting tile, we assume there is only one such tile.
    Tile *endTile;             // it is the end tile, assumed only one exists.

    // create an empty 2D array for our maze.
    Tile **mazeArray = new Tile *[mazeDimension];
    for (int i = 0; i < mazeDimension; i++)
    {
        mazeArray[i] = new Tile[mazeDimension];
    }
    // we are going, for the purposes of this assignment, assume the format of the file is correct...
    int tileID = 0;
    for (int i = 0; i < mazeDimension; i++)
    { // first, we will get all the information about the tile types!

        std::string rowInput = "";
        readMaze >> rowInput;
        for (int j = 0; j < mazeDimension; j++)
        {
            char symbolAt = rowInput[j];
            if (symbolAt == 'O')
            {
                mazeArray[i][j].setTileType(0); // open tile to walk through.
            }
            else if (symbolAt == 'X')
            {
                mazeArray[i][j].setTileType(1); // solid or wall
            }
            else if (symbolAt == 'S')
            {
                mazeArray[i][j].setTileType(2); // start tile
                startTile = &mazeArray[i][j];   // hang onto the start tile
            }
            else
            {
                mazeArray[i][j].setTileType(3); // otherwise, assume it is an end tile.
                endTile = &mazeArray[i][j];     // hang onto the end tile (if we need it)
            }
            mazeArray[i][j].setID(tileID);
            tileID++;
        }
    }

    // next, we go through all the tiles again, meshing together the neighbours, wherever they exist...
    for (int i = 0; i < mazeDimension; i++)
    {
        for (int j = 0; j < mazeDimension; j++)
        {
            // now assign the neighbours, but avoid being "out of bounds".
            if (i - 1 >= 0)
            {
                mazeArray[i][j].setNeighbour(0, &mazeArray[i - 1][j]); // Up
            }
            if (j + 1 < mazeDimension)
            {
                mazeArray[i][j].setNeighbour(1, &mazeArray[i][j + 1]); // Right
            }
            if (i + 1 < mazeDimension)
            {
                mazeArray[i][j].setNeighbour(2, &mazeArray[i + 1][j]); // Down
            }
            if (j > 0)
            {
                mazeArray[i][j].setNeighbour(3, &mazeArray[i][j - 1]); // Left
            }
        }
    }
    // we are done reading the file!
    readMaze.close(); // close the file

    int numMoves = 0; // Number of moves, let us count these!

    // now we attempt to solve the maze... using the stack!
    StackADT *mazeStack = new LinkedStack(); // make an empty stack
    mazeStack->push(startTile);              // push the beginning of the maze!
    startTile->markTile();                   // mark it, so we don't come to the beginning all over again.
    bool foundEnd = false;                   // set this to be true, if we find the end!

    // TIME TO EXPLORE THE  MAZE
    while (!mazeStack->isEmpty() && !foundEnd)
    {                                          // MAIN-LOOP while we have not yet found the end and the stack is not empty!
        Tile *currentTile = mazeStack->peek(); // examine the top of the stack
        if (currentTile->getTileType() == 3)
        { // found the end tile?
            foundEnd = true;
            break;
        }
        numMoves++;
        bool foundNeighbour = false;
        int countNeighbour = 0;
        while (foundNeighbour == false && countNeighbour <= 3)
        { // look through the neighbours
            Tile *neighbour = currentTile->getNeighbour(countNeighbour);
            if (neighbour != nullptr)
            { // is this an actual tile?
                if (neighbour->getTileType() != 1 && !neighbour->isMarked())
                {
                    foundNeighbour = true;
                    neighbour->markTile();      // mark it!
                    mazeStack->push(neighbour); // push the tile
                }
            }

            countNeighbour++; // consider the next neighbour
        }
        if (!foundNeighbour)
        {
            mazeStack->pop(); // No more we can do with this tile/chamber, pop it!
        }
    }

    // did we find the end of the maze?
    if (foundEnd)
    {
        int numPath = mazeStack->size(); // let us compute the number of tiles/chambers moved!
        while (!mazeStack->isEmpty())
        { // the stack will contain the path (end to start)!
            Tile *currTile = mazeStack->pop();
            if (currTile->getTileType() != 2 && currTile->getTileType() != 3)
            {
                currTile->setTileType(4); // we will set the path to be type 4...
                // printMaze(mazeArray,mazeDimension);//uncomment this line if you want to watch the maze animate!
            }
        }
        // now let us print out what the maze looks like, with the path!
        printMaze(mazeArray, mazeDimension);
        std::cout << "MAZE END FOUND: A path to the end visits " << numPath - 1 << " chamber(s) from the start, " << numMoves << " steps were made to solve this maze." << std::endl;
    }
    else
    {
        printMaze(mazeArray, mazeDimension);
        std::cout << "MAZE END NOT FOUND: No solution..., " << numMoves << " many steps have occurred." << std::endl;
    }

    // some cleanup now!
    delete mazeStack;
    // deleting the array of tiles
    for (int i = 0; i < mazeDimension; i++)
    {
        delete[] mazeArray[i];
    }
    delete mazeArray; // delete the array of pointers to tiles

    // Some test code, if you would like to use it...
    /*LinkedStack *newStack = new LinkedStack();
    std::cout << "Pushing tiles with IDs 0,1,2,...,9 onto stack" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        Tile *newTile = new Tile();
        newTile->setID(i); // a tile has an ID, you can use this!
        newStack->push(newTile);
        std::cout << "Pushed a tile with ID: " << newStack->peek()->getID() << ", number of tiles in stack:" << newStack->size() << std::endl;
    }
    std::cout << "Removing all the tiles from the stack" << std::endl;
    while (!newStack->isEmpty())
    {
        Tile *returnedEntry = newStack->pop();
        std::cout << "Popped a tile, its ID:" << returnedEntry->getID() << ", number of tiles in stack:" << newStack->size() << std::endl;
        delete returnedEntry;
    }
    delete newStack;*/
    return 0;
}