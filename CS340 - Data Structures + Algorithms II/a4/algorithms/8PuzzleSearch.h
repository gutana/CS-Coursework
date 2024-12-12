#pragma once

#include <vector>

#include "PuzzleState.h"

enum SearchType
{
    DepthFirst,
    BreadthFirst,
    BestFirstSearch
};

enum Heuristic
{
    NumTilesOutOfPlace,
    MinimumMovesToReachGoal,
    DistanceAndSequenceScore
};

class EightPuzzleSearch
{
public:
    static int DepthFirstSearch(PuzzleState &initialState,
                                std::vector<PuzzleState *> &out_path);

    static int BreadthFirstSearch(PuzzleState &initialState,
                                  std::vector<PuzzleState *> &out_path);

    static int BestFirstSearch(PuzzleState *initialState,
                               std::vector<PuzzleState *> &out_path,
                               Heuristic heuristicType);
};

struct PuzzleStateWithPriority
{
    PuzzleState *state;
    int priority;

    PuzzleStateWithPriority(){};
    PuzzleStateWithPriority(PuzzleState *state, int priority)
        : state(state), priority(priority){};

    bool operator<(const PuzzleStateWithPriority &other) const
    {
        return priority < other.priority;
    }
    void SetState(PuzzleState *p) { state = p; }
};