
#include "structures/MinMaxHeap.h"

#include <vector>
#include <iostream>
#include <string>

enum CommandType
{
    Quit,
    Unknown,
    Help,
    Insert,
    Print,
    Build,
    Clear,
    Min,
    Max,
    DeleteMin,
    DeleteMax
};

template <typename T>
class MinMaxHeapCLI
{
private:
    MinMaxHeap<T> mm;

public:
    MinMaxHeapCLI();
    void Run();

private:
    void processCommands(std::vector<std::string> input);
    CommandType getCommandType(std::string) const;
    void printCommands() const;

    std::vector<std::string> splitString(std::string, char);
};
