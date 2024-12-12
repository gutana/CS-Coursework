#include <iostream>
#include <cstring>
#include <algorithm>

#include "PuzzleState.h"

PuzzleState::PuzzleState(const char *s)
{
    board = new char[9];

    bool holeFound = false;

    // To keep track of the numbers we've seen, so we can ensure
    // theres only 1 of each.
    int numbersSeen[9] = {0};

    // loop to copy the string, but also do some validation while we're looping
    for (int i = 0; i < 9; i++)
    {
        board[i] = s[i];

        if (board[i] == 'X' || board[i] == 'x' || board[i] == ' ') // convert the 'X' to an empty space
        {
            if (holeFound)
                throw "Invalid board: Multiple holes/empty spaces found";

            board[i] = ' ';
            holeLocation = i;
            holeFound = true;
        }

        // Validate board configuration
        if ((board[i] != ' ') && (board[i] < '1' || board[i] > '8'))
        {
            std::cout << "Unexpected piece: " << board[i] << '\n';
            throw "Invalid Board: unexpected valid (0-8 expected)";
        }

        if (board[i] != ' ')
        {
            int num = (board[i] - '0'); // subtract the '0' to convert from char to int
            numbersSeen[num]++;
        }
    }

    if (!holeFound)
    {
        throw "Invalid board: No empty space found.";
    }

    for (int i = 1; i < 9; i++)
    {
        if (numbersSeen[i] != 1)
        {
            std::cout << "Error: " << i << " was seen " << numbersSeen[i] << " times.\n";
            throw "Invalid board: all numbers from 1-8 must appear once and only once.";
        }
    }
}

PuzzleState::PuzzleState()
{
}

PuzzleState::~PuzzleState()
{
    if (board != nullptr)
    {
        delete[] board;
        board = nullptr;
    }
}

void PuzzleState::SetState(char *s)
{

    board = s;

    bool holeFound = false;

    // To keep track of the numbers we've seen, so we can ensure
    // theres only 1 of each.
    int numbersSeen[9] = {0};

    // loop to copy the string, but also do some validation while we're looping
    for (int i = 0; i < 9; i++)
    {
        board[i] = s[i];

        if (board[i] == 'X' || board[i] == 'x' || board[i] == ' ') // convert the 'X' to an empty space
        {
            if (holeFound)
                throw "Invalid board: Multiple holes/empty spaces found";

            board[i] = ' ';
            holeLocation = i;
            holeFound = true;
        }

        // Validate board configuration
        if ((board[i] != ' ') && (board[i] < '1' || board[i] > '8'))
        {
            std::cout << "Unexpected piece: " << board[i] << '\n';
            throw "Invalid Board: unexpected valid (0-8 expected)";
        }

        if (board[i] != ' ')
        {
            int num = (board[i] - '0'); // subtract the '0' to convert from char to int
            numbersSeen[num]++;
        }
    }

    if (!holeFound)
    {
        std::cout << "No hole found in board: " << board << '\n';
        throw "Invalid board: No empty space found.";
    }

    for (int i = 1; i < 9; i++)
    {
        if (numbersSeen[i] != 1)
        {
            std::cout << "Error: " << i << " was seen " << numbersSeen[i] << " times.\n";
            throw "Invalid board: all numbers from 1-8 must appear once and only once.";
        }
    }
}

char *PuzzleState::GetState()
{
    return board;
}

// Example output:
// |1|2|3|
// |4|5|6|
// |7|8| |
void PuzzleState::printBoard()
{
    std::cout << '|' << board[0] << '|' << board[1] << '|' << board[2] << "|\n";
    std::cout << '|' << board[3] << '|' << board[4] << '|' << board[5] << "|\n";
    std::cout << '|' << board[6] << '|' << board[7] << '|' << board[8] << "|\n";
}

int PuzzleState::getHoleLocation() const
{
    return holeLocation;
}

// expects a pointer to an array of 4 pointers, which should eventually be freed
PuzzleState **PuzzleState::generateMoves()
{
    PuzzleState **array = new PuzzleState *[4];
    array[0] = Up();
    array[1] = Down();
    array[2] = Left();
    array[3] = Right();
    return array;
}

// 0 1 2
// 3   5
// 6 7 8
// Allocates new memory. Be sure to eventually free the returned pointer.
PuzzleState *PuzzleState::Up()
{
    if (holeLocation > 5)
        return nullptr;

    char *newStateStr = new char[9];
    memcpy(newStateStr, board, 9);

    std::swap(newStateStr[holeLocation], newStateStr[holeLocation + 3]);

    PuzzleState *newState = new PuzzleState(newStateStr);

    delete[] newStateStr;
    return newState;
}
// Allocates new memory. Be sure to eventually free the returned pointer.
PuzzleState *PuzzleState::Down()
{
    if (holeLocation < 3)
        return nullptr;

    char *newStateStr = new char[9];
    memcpy(newStateStr, board, 9);

    std::swap(newStateStr[holeLocation], newStateStr[holeLocation - 3]);

    PuzzleState *newState = new PuzzleState(newStateStr);

    delete[] newStateStr;
    return newState;
}
// Allocates new memory. Be sure to eventually free the returned pointer.
PuzzleState *PuzzleState::Left()
{
    if (holeLocation % 3 == 2)
        return nullptr;

    char *newStateStr = new char[9];
    memcpy(newStateStr, board, 9);

    std::swap(newStateStr[holeLocation], newStateStr[holeLocation + 1]);

    PuzzleState *newState = new PuzzleState(newStateStr);

    delete[] newStateStr;
    return newState;
}
// Allocates new memory. Be sure to eventually free the returned pointer.
PuzzleState *PuzzleState::Right()
{
    if (holeLocation % 3 == 0)
        return nullptr;

    char *newStateStr = new char[9];
    memcpy(newStateStr, board, 9);

    std::swap(newStateStr[holeLocation], newStateStr[holeLocation - 1]);

    PuzzleState *newState = new PuzzleState(newStateStr);

    delete[] newStateStr;
    return newState;
}
