
#include <vector>
#include <iostream>
#include <string>
#include <chrono>

enum SortType
{
    Insertion_Sort,
    Merge_Sort,
    Quick_Sort,
    Quick_Insertion
};

using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::time_point;

template <typename T>
class SortCLI
{
public:
    void Run();
    void Demo();

private:
    void printCommands() const;
    void printVector(const std::vector<T> *) const;

    void handleCommands(std::string command);
    void runSortCommand(SortType);
    int getIntegerFromInput(const std::string) const;

    std::vector<T> *generateRandomVector(const int) const;
    void lowercaseString(std::string &) const;

private:
    time_point<high_resolution_clock> startTime;

    void startTimer();
    microseconds stopTimer();
    bool timerStarted = false;
};
