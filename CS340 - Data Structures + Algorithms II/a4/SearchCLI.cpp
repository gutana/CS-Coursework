#include "SearchCLI.h"
#include "algorithms/PuzzleState.h"
#include "algorithms/8PuzzleSearch.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include <cstring>

void SearchCLI::Run()
{
    std::cout << "\nWelcome to the Interactive 8-Puzzle Search Algorithm Experience!\n";

    printCommands();
    while (true)
    {
        std::cout << "search$ ";

        char input;
        std::cin >> input;

        handleCommand(tolower(input)); // lowercase the input char to slightly simplify logic
    }
}

void SearchCLI::printCommands() const
{
    std::cout << "This program will take an initial configuration of an 8-Puzzle and demonstrate the usage of various search algorithms.\n";
    std::cout << "Press (?) for help, (q) to quit.\n";
    std::cout << "\nSearch types:\n";
    std::cout << "  (d)  Depth First Search\n";
    std::cout << "  (b)  Breadth First Search\n";
    std::cout << "  (h)  Best First Search with Heuristic\n\n";
}

// assumes input is lowercase
void SearchCLI::handleCommand(char command)
{
    const std::string validCommandChars = "?hqdb";
    // check if the char we received isn't a valid char
    if (validCommandChars.find(command) == std::string::npos)
    {
        std::cout << "Unknown command.\n";

        // clear input in cases where user inputted multi-char sequence
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return;
    }

    char *boardInput;
    PuzzleState p;

    switch (command)
    {
    case '?': // Help
    {
        printCommands();
        break;
    }
    case 'q': // Quit
    {
        std::cout << "Have a nice day!\n";
        exit(0);
    }
    case 'd': // Depth First Search
    {
        boardInput = getBoardFromCommandLine();
        p.SetState(boardInput);

        std::vector<PuzzleState *> pathFound;
        int numNodes = EightPuzzleSearch::DepthFirstSearch(p, pathFound);

        if (numNodes == -1)
        {
            std::cout << "Unable to find solution.\n";
            break;
        }

        printStateSequence(pathFound);

        std::cout << numNodes << " nodes/states were generated.\n";
        break;
    }
    case 'b': // Breadth First Search
    {
        boardInput = getBoardFromCommandLine();
        p.SetState(boardInput);

        std::vector<PuzzleState *> pathFound;
        int numNodes = EightPuzzleSearch::BreadthFirstSearch(p, pathFound);

        if (numNodes == -1)
        {
            std::cout << "Unable to find solution.\n";
            break;
        }

        printStateSequence(pathFound);

        std::cout << numNodes << " nodes/states were generated.\n";
        break;
    }
    case 'h': // Best first search w/ Heuristics
    {
        Heuristic chosenHeuristic = getHeuristicFromCommandLine();

        boardInput = getBoardFromCommandLine();
        p.SetState(boardInput);

        std::vector<PuzzleState *> pathFound;
        int numNodes = EightPuzzleSearch::BestFirstSearch(&p, pathFound, chosenHeuristic);

        if (numNodes == -1)
        {
            std::cout << "Unable to find solution.\n";
            break;
        }

        printStateSequence(pathFound);

        std::cout << numNodes << " nodes/states were generated.\n";
        break;
    }
    }
}

char *SearchCLI::getBoardFromCommandLine()
{
    const std::string validBoardChars = "X12345678x";
    while (true)
    {
        std::cout << "Please enter starting configuration for the 8 puzzle.\n";
        std::cout << "    Example: \"54321X687\" will produce the board: \n";
        std::cout << "        |5|4|3|\n";
        std::cout << "        |2|1| |\n";
        std::cout << "        |6|8|7|\n\n";
        std::cout << "    Note: use 'X' to indicate hole/empty space.\n\n";

        std::cout << "Starting Configuration: ";

        std::string input;

        std::cin >> input;
        while (size_t spaceLocation = input.find(' ') != std::string::npos)
        {
            input.erase(spaceLocation, 1); // clear any leading/trailing spaces;
        }

        bool inputGood = true;

        if (input.length() != 9)
        {
            std::cout << "Error with board. Expected input of length 9.";
            inputGood = false;
        }

        if (!inputGood)
            continue;

        for (int i = 0; i < 9; i++)
        {
            if (validBoardChars.find(input[i]) == std::string::npos)
            {
                std::cout << "Unexpected input in board configuration: " << input[i] << '\n';
                inputGood = false;
            }
        }

        if (inputGood)
        {
            char *outText = new char[9];
            memcpy(outText, input.c_str(), 9);
            return outText;
        }
    }
}

Heuristic SearchCLI::getHeuristicFromCommandLine()
{
    std::string validChars = "tmd";
    char input;

    std::cout << "\nBest first search with Heuristic Chosen.\n";
    std::cout << "Heuristics available:\n";
    std::cout << "     (t) Number of tiles out of place.\n";
    std::cout << "     (m) Minimum number of moves to reach goal.\n";
    std::cout << "     (d) Distance + Sequence Score.\n";
    std::cout << "\n";

    while (true) // this loop just gets char input to choose heuristic, breaking when valid input is given
    {
        std::cout << "heuristic: ";
        std::cin >> input;

        input = tolower(input);

        if (validChars.find(input) == std::string::npos)
        {
            std::cout << "Unknown input.\n";
        }
        else
        {
            break;
        }
    }

    Heuristic heuristic = convertCharToHeuristic(input);
    return heuristic;
}

Heuristic SearchCLI::convertCharToHeuristic(char c)
{
    switch (c)
    {
    case 't':
        return Heuristic::NumTilesOutOfPlace;
    case 'm':
        return Heuristic::MinimumMovesToReachGoal;
    case 'd':
        return Heuristic::DistanceAndSequenceScore;
    default:
        throw "Unexpected character being converted to heuristic enum";
    }
}

void SearchCLI::printStateSequence(std::vector<PuzzleState *> &seq)
{
    std::cout << '\n';
    for (int i = 0; i < seq.size(); i++)
    {
        seq[i]->printBoard();
        // don't print an arrow after if this is the last state
        if (i != seq.size() - 1)
        {
            std::cout << "   |\n";
            std::cout << "   V\n";
        }
    }
    std::cout << '\n';
}
