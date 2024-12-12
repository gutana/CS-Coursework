#pragma once

#include <iostream>
#include <string>

#include "algorithms/8PuzzleSearch.h"

class SearchCLI
{
public:
    void Run();

private:
    void printCommands() const;

    void handleCommand(char command);

    char *getBoardFromCommandLine();
    Heuristic getHeuristicFromCommandLine();

    Heuristic convertCharToHeuristic(char c);
    void printStateSequence(std::vector<PuzzleState *> &seq);
};
