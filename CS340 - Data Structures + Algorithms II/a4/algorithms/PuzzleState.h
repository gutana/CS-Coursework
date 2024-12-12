#pragma once

#include <string>

class PuzzleState
{
public:
    PuzzleState(const char *);
    PuzzleState();
    ~PuzzleState();

    void SetState(char *);
    char *GetState();

    void printBoard();
    int getHoleLocation() const;

    PuzzleState **generateMoves();

    PuzzleState *Up();
    PuzzleState *Down();
    PuzzleState *Left();
    PuzzleState *Right();

private:
    char *board = nullptr;
    int holeLocation;
};