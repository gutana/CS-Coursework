#include <queue>
#include <cstring>
#include <map>
#include <math.h>
#include <algorithm>

#include <iostream>

#include "8PuzzleSearch.h"
#include "Stack.h"
#include "MinMaxHeap.h"

const char targetState[] = "1238 4765";

// helper function to check if map contains the targetState
bool mapContainsSolution(std::map<PuzzleState *, PuzzleState *> map)
{
    for (auto const &[key, val] : map)
    {
        if (strcmp(key->GetState(), targetState) == 0)
        {
            std::cout << "Target state found.\n";
            return true;
        }
    }
    // std::cout << "No target state found.\n";
    return false;
}

int EightPuzzleSearch::DepthFirstSearch(PuzzleState &initialState, std::vector<PuzzleState *> &out_path)
{
    Stack<PuzzleState *> stack;
    // to keep track of the parent of each explored node
    std::map<PuzzleState *, PuzzleState *> stateToParentMap;

    std::vector<PuzzleState *> generatedNodes;

    int count = 0; // count how many nodes we made

    stack.Push(&initialState);
    stateToParentMap.emplace(&initialState, (PuzzleState *)0);

    while (stack.Size() != 0 && !mapContainsSolution(stateToParentMap))
    {
        PuzzleState *current = stack.Pop();

        // std::cout << "Generating possibilities for state:\n";
        // current->printBoard();

        PuzzleState **possibleMoves = current->generateMoves();

        // check if each of these is a goal state or not
        for (int i = 0; i < 4; i++)
        {
            bool goodMove = true;
            if (possibleMoves[i] == nullptr)
                continue;

            stateToParentMap.emplace(possibleMoves[i], current);

            for (int j = 0; j < generatedNodes.size(); j++)
            {
                // if we already seen this config, we don't want to try it again
                if (strcmp(generatedNodes[j]->GetState(), possibleMoves[i]->GetState()) == 0)
                {
                    goodMove = false;
                    // std::cout << "Move already seen!\n";
                }
            }

            if (!goodMove)
                continue;

            count++;
            // check if this is valid solution
            if (strcmp(targetState, possibleMoves[i]->GetState()) == 0)
            {
                std::cout << "Found valid solution!\n";
                // Set the path vector and return true
                for (PuzzleState *p = possibleMoves[i]; p != nullptr; p = stateToParentMap.at(p))
                {
                    out_path.push_back(p);
                }
                // reverse the vector, since we're pushing to it end to start right now
                std::reverse(out_path.begin(), out_path.end());

                return count;
            }
            else if (goodMove)
            {
                stack.Push(possibleMoves[i]);
                generatedNodes.push_back(possibleMoves[i]);
            }
        }
    }

    return count;
}

int EightPuzzleSearch::BreadthFirstSearch(PuzzleState &initialState, std::vector<PuzzleState *> &out_path)
{
    std::queue<PuzzleState *> queue;

    // to keep track of the parent of each explored node
    std::map<PuzzleState *, PuzzleState *> stateToParentMap;

    std::vector<PuzzleState *> generatedNodes;

    int count = 0; // count how many nodes we made

    queue.push(&initialState);
    stateToParentMap.emplace(&initialState, (PuzzleState *)0);

    while (queue.size() != 0 && !mapContainsSolution(stateToParentMap))
    {
        PuzzleState *current = queue.front();
        queue.pop();

        // std::cout << "Generating possibilities for state:\n";
        // current->printBoard();

        PuzzleState **possibleMoves = current->generateMoves();

        // check if each of these is a goal state or not
        for (int i = 0; i < 4; i++)
        {
            bool goodMove = true;
            if (possibleMoves[i] == nullptr)
                continue;

            stateToParentMap.emplace(possibleMoves[i], current);

            for (int j = 0; j < generatedNodes.size(); j++)
            {
                // if we already seen this config, we don't want to try it again
                if (strcmp(generatedNodes[j]->GetState(), possibleMoves[i]->GetState()) == 0)
                {
                    goodMove = false;
                    // std::cout << "Move already seen!\n";
                }
            }

            if (!goodMove)
                continue;

            count++;
            // check if this is valid solution
            if (strcmp(targetState, possibleMoves[i]->GetState()) == 0)
            {
                std::cout << "Found valid solution!\n";
                // Set the path vector and return true
                for (PuzzleState *p = possibleMoves[i]; p != nullptr; p = stateToParentMap.at(p))
                {
                    out_path.push_back(p);
                }
                // reverse the vector, since we're pushing to it end to start right now
                std::reverse(out_path.begin(), out_path.end());

                return count;
            }
            else if (goodMove)
            {
                queue.push(possibleMoves[i]);
                generatedNodes.push_back(possibleMoves[i]);
            }
        }
    }

    return count;
}

int getNumTilesOutOfPlace(PuzzleState *state)
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (state->GetState()[i] != targetState[i])
        {
            count++;
        }
    }
    return count;
}

int getMinMovesToGoal(PuzzleState *state)
{
    // We use this 3x3 grid as target state to make life easier.
    char targetState[3][3] = {{'1', '2', '3'},
                              {'8', ' ', '4'},
                              {'7', '6', '5'}};

    // Define the current state as a 3x3 grid
    char currentState[3][3];
    int index = 0;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            currentState[row][col] = state->GetState()[index];
            index++;
        }
    }

    // Calculate total distance
    int totalDistance = 0;

    // Iterate through each cell of the grid
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            char currentTile = currentState[row][col];

            // Find the position of the current tile in the target state
            int targetRow, targetCol;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (targetState[i][j] == currentTile)
                    {
                        targetRow = i;
                        targetCol = j;
                        break;
                    }
                }
            }

            // Calculate dist between current position and target position
            totalDistance += std::abs(row - targetRow) + std::abs(col - targetCol);
        }
    }

    return totalDistance;
}

int calculateSequenceScore(PuzzleState *state)
{
    // 0 1 2
    // 3 4 5
    // 6 7 8
    std::vector<int> clockwiseTraversal = {1, 2, 5, 8, 7, 6, 3, 0}; // (if you are 5, your successor is 8, etc)

    int sum = 0;
    for (int i = 0; i < 9; i++)
    {
        if (i == 4) // If in centre, tile is worth 1
        {
            sum++;
            continue;
        }

        // If followed by proper successor (clockwise), worth 0
        char currentChar = state->GetState()[i];

        for (int j = 0; j < clockwiseTraversal.size(); j++)
        {
            if (clockwiseTraversal[i] == currentChar)
            {
                j++;
                // If not followed by proper successor, worth 2
                if (j < i)
                    sum += 2;
                break;
            }
        }
    }
    return sum;
}

int getDistAndSequenceScore(PuzzleState *state)
{
    int totalDistance = getMinMovesToGoal(state);
    int sequenceScore = calculateSequenceScore(state);
    return totalDistance + 3 * sequenceScore;
}

int calculateDepth(PuzzleState *state, std::map<PuzzleState *, PuzzleState *> childToParentMap)
{
    int count = 0;
    for (PuzzleState *p = state; p != nullptr; p = childToParentMap.at(p))
    {
        count++;
    }
    return count;

    return 1;
}

int EightPuzzleSearch::BestFirstSearch(PuzzleState *initialState, std::vector<PuzzleState *> &out_path, Heuristic heuristicType)
{
    MinMaxHeap<PuzzleStateWithPriority *> prioQueue;

    // to keep track of the parent of each explored node
    std::map<PuzzleState *, PuzzleState *> stateToParentMap;

    std::vector<PuzzleState *> generatedNodes;

    int count = 0; // count how many nodes we made

    // calculate priority
    int priority = 1;

    PuzzleStateWithPriority *stateWithPrio = new PuzzleStateWithPriority();
    stateWithPrio->priority = priority;
    stateWithPrio->SetState(initialState);

    prioQueue.insertHeap(stateWithPrio);
    stateToParentMap.emplace(initialState, nullptr);

    while (prioQueue.size() != 0 && !mapContainsSolution(stateToParentMap))
    {
        PuzzleStateWithPriority *current = prioQueue.deleteMin();
        // std::cout << "Generating possibilities for state:\n";
        // current->printBoard();

        PuzzleState **possibleMoves = current->state->generateMoves();

        // check if each of these is a goal state or not
        for (int i = 0; i < 4; i++)
        {
            bool goodMove = true;
            if (possibleMoves[i] == nullptr)
                continue;

            stateToParentMap.emplace(possibleMoves[i], current->state);

            for (int j = 0; j < generatedNodes.size(); j++)
            {
                // if we already seen this config, we don't want to try it again
                if (strcmp(generatedNodes[j]->GetState(), possibleMoves[i]->GetState()) == 0)
                {
                    goodMove = false;
                    // std::cout << "Move already seen!\n";
                }
            }

            if (!goodMove)
                continue;

            count++;
            // check if this is valid solution
            if (strcmp(targetState, possibleMoves[i]->GetState()) == 0)
            {
                std::cout << "Found valid solution! Final state:\n\n";
                // Set the path vector and return true

                // std::cout << "\n Printing the whole sequnce for Best-First search is disabled. ";
                // std::cout << "\nI was unable to get this part working for some reason (i'm sorry).\n";

                // Commented out because throws std::out_of_range exception for unknown reason
                // out of range here???
                for (PuzzleState *p = possibleMoves[i]; p != nullptr; p = stateToParentMap.at(p))
                {
                    out_path.push_back(p);
                }

                // reverse the vector, since we're pushing to it end to start right now
                std::reverse(out_path.begin(), out_path.end());

                return count;
            }
            else if (goodMove)
            {
                switch (heuristicType)
                {
                case Heuristic::NumTilesOutOfPlace:
                    priority = getNumTilesOutOfPlace(possibleMoves[i]) + calculateDepth(possibleMoves[i], stateToParentMap);
                    break;
                case Heuristic::MinimumMovesToReachGoal:
                    priority = getMinMovesToGoal(possibleMoves[i]) + calculateDepth(possibleMoves[i], stateToParentMap);
                    break;
                case Heuristic::DistanceAndSequenceScore:
                    priority = getDistAndSequenceScore(possibleMoves[i]) + calculateDepth(possibleMoves[i], stateToParentMap);
                    break;
                }
                PuzzleStateWithPriority *prioritizedPuzzle = new PuzzleStateWithPriority();
                prioritizedPuzzle->priority = priority;
                prioritizedPuzzle->state = new PuzzleState();
                prioritizedPuzzle->SetState(possibleMoves[i]);
                prioQueue.insertHeap(prioritizedPuzzle);
                generatedNodes.push_back(possibleMoves[i]);
            }
        }
    }

    return count;
}
