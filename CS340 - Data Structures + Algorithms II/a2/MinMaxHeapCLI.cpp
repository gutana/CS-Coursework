#include "MinMaxHeapCLI.h"

#include "structures/MinMaxHeap.h"

#include <string>

template <typename T>
MinMaxHeapCLI<T>::MinMaxHeapCLI()
{
}

template <typename T>
void MinMaxHeapCLI<T>::Run()
{
    std::cout << "\nWelcome to the interactive MinMaxHeap interface!\n";
    std::cout << "Enter 'h' or 'help' at any time to see list of commands.\n\n";

    printCommands();

    while (true)
    {
        std::cout << "MinMaxHeap$ ";

        std::string input;
        std::getline(std::cin, input);

        if (input.size() == 0)
            continue;

        std::vector<std::string> splitInput = splitString(input, ' ');

        processCommands(splitInput);
    }
}

template <typename T>
void MinMaxHeapCLI<T>::processCommands(std::vector<std::string> input)
{
    CommandType command = getCommandType(input[0]);

    switch (command)
    {
    case CommandType::Help:
    {
        printCommands();
        break;
    }
    case CommandType::Print:
    {
        mm.printArray();
        break;
    }
    case CommandType::Insert:
    {
        for (int i = 1; i < input.size(); i++)
        {
            mm.insertHeap(stof(input[i]));
            std::cout << stof(input[i]) << " ";
        }
        std::cout << "inserted\n";
        break;
    }
    case CommandType::Clear:
    {
        mm.clear();
        break;
    }
    case CommandType::Min:
    {
        std::cout << mm.findMin() << '\n';
        break;
    }
    case CommandType::Max:
    {
        std::cout << mm.findMax() << '\n';
        break;
    }
    case CommandType::DeleteMin:
    {
        std::cout << mm.deleteMin() << " removed.\n";
        break;
    }
    case CommandType::DeleteMax:
    {
        std::cout << mm.deleteMax() << " removed\n";
        break;
    }
    case CommandType::Build:
    {
        std::vector<T> items;
        for (int i = 1; i < input.size(); i++)
        {
            items.push_back(stof(input[i]));
        }
        mm = MinMaxHeap<T>::buildHeap(items);
        std::cout << "Built new heap with " << items.size() << " items.\n";
        break;
    }
    case CommandType::Quit:
    {
        std::cout << "Exiting. Have a good day!\n";
        exit(0);
        break;
    }

    default:
        break;
    }
}

template <typename T>
CommandType MinMaxHeapCLI<T>::getCommandType(std::string s) const
{
    if (s == "min")
    {
        return CommandType::Min;
    }
    if (s == "max")
    {
        return CommandType::Max;
    }
    if (s == "build")
    {
        return CommandType::Build;
    }
    if (s == "deletemin")
    {
        return CommandType::DeleteMin;
    }
    if (s == "deletemax")
    {
        return CommandType::DeleteMax;
    }

    char firstChar = tolower(s[0]);

    switch (firstChar)
    {
    case 'h':
    {
        return CommandType::Help;
    }
    case 'i':
    {
        return CommandType::Insert;
    }
    case 'p':
    {
        return CommandType::Print;
    }
    case 'c':
    {
        return CommandType::Clear;
    }
    case 'q':
    {
        return CommandType::Quit;
    }
    case 'b':
    {
        return CommandType::Build;
    }
    default:
    {
        std::cout << "Error: Unknown command. 'h' or 'help' for help.\n";
        return CommandType::Unknown;
    }
    }
}

template <typename T>
void MinMaxHeapCLI<T>::printCommands() const
{
    std::cout << "-------- COMMANDS --------\n";
    std::cout << "help      (h): Displays this message.\n";
    std::cout << "insert    (i): Inserts all elements following the command into the heap.\n";
    std::cout << "               Example: 'insert 5 3 10'\n";
    std::cout << "build     (b): Builds a new heap from all elements following the command.\n";
    std::cout << "print     (p): Prints the array representation of the MinMax Heap.\n";
    std::cout << "clear     (c): Clears the heap, deleting all elements.\n";
    std::cout << "quit      (q): Quits this program.\n";
    std::cout << "min:           Outputs the minimum element.\n";
    std::cout << "deletemin:     Deletes and outputs the minimum element.\n";

    std::cout << "max:           Outputs the maximum element.\n";
    std::cout << "deletemax:     Deletes and outputs the maximum element.\n";

    std::cout << "\n";
}

template <typename T>
std::vector<std::string> MinMaxHeapCLI<T>::splitString(std::string stringToSplit, char charToSplitBy)
{
    // the final vector of strings we will return
    std::vector<std::string> splitInput;

    // temp to hold our characters as we iterate until we reach a space or end of string
    std::string tempStr;
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

template class MinMaxHeapCLI<float>;
