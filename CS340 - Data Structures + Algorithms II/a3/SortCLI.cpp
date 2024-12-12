#include "SortCLI.h"

#include "algorithms/SortingAlgorithms.h"

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <random>

template <typename T>
void SortCLI<T>::Run()
{
    std::cout << "\nWelcome to the interactive sorting experience!\n";

    printCommands();

    while (true)
    {
        std::cout << "sort$ ";

        std::string input;
        std::getline(std::cin, input);
        lowercaseString(input);

        try
        {
            handleCommands(input);
        }
        catch (...)
        {
            std::cout << "Unknown error occurred.";
        }
    }
}

template <typename T>
void SortCLI<T>::Demo()
{
    int MIN_SIZE = 3;
    int MAX_SIZE = 50000;
    int NUM_RUNS = 5;
    int k = 15;

    // the size at which we should give up trying to use insertion sort
    int INSERTION_SORT_LIMIT = 100;

    std::vector<int> timesInsertionSortWasFastest;
    std::vector<int> timesMergeSortWasFastest;
    std::vector<int> timesQuickSortWasFastest;
    std::vector<int> timesQuickInsertionSortWasFastest;

    for (int i = MIN_SIZE; i <= MAX_SIZE; i += 10)
    {
        // std::cout << "Testing arrays of size " << i << ".\n";
        std::vector<microseconds> quickSortTimes;
        std::vector<microseconds> insertionSortTimes;
        std::vector<microseconds> mergeSortTimes;
        std::vector<microseconds> quickInsertionSortTimes;

        for (int j = 0; j < NUM_RUNS; j++)
        {
            std::vector<T> *vec;

            // QUICK SORT
            vec = generateRandomVector(i);

            startTimer();
            QuickSort<T>::Sort(vec);
            auto time = stopTimer();

            quickSortTimes.push_back(time);
            delete vec;

            // INSERTION SORT
            // trying insertion sort above a certain number of elements is too slow and painful
            if (i < INSERTION_SORT_LIMIT)
            {
                vec = generateRandomVector(i);

                startTimer();
                InsertionSort<T>::Sort(vec);
                time = stopTimer();

                insertionSortTimes.push_back(time);
                delete vec;
            }

            // MERGE SORT
            vec = generateRandomVector(i);

            startTimer();
            MergeSort<T>::Sort(vec);
            time = stopTimer();

            mergeSortTimes.push_back(time);
            delete vec;

            // QUICK INSERTION SORT
            vec = generateRandomVector(i);

            startTimer();
            QuickInsertionSort<T>::Sort(vec, k);
            time = stopTimer();

            quickInsertionSortTimes.push_back(time);
            delete vec;
        }

        // std::cout << "Average sort times for array size " << i << ":\n";

        microseconds quickSortAverage(0); // note it won't actually be the average until we've divided by NUM_RUNS in a few lines
        for (auto time : quickSortTimes)
        {
            quickSortAverage += time;
        }
        quickSortAverage /= NUM_RUNS;

        microseconds mergeSortAverage(0); // note it won't actually be the average until we've divided by NUM_RUNS in a few lines
        for (auto time : mergeSortTimes)
        {
            mergeSortAverage += time;
        }
        mergeSortAverage /= NUM_RUNS;

        microseconds insertionSortAverage(0); // note it won't actually be the average until we've divided by NUM_RUNS in a few lines
        if (i < INSERTION_SORT_LIMIT)
        {
            for (auto time : insertionSortTimes)
            {
                insertionSortAverage += time;
            }
            insertionSortAverage /= insertionSortTimes.size(); // insertion sort will have less runs, because its too slow at high sizes
        }

        microseconds quickInsertionAverage(0); // note it won't actually be the average until we've divided by NUM_RUNS in a few lines
        for (auto time : quickInsertionSortTimes)
        {
            quickInsertionAverage += time;
        }
        quickInsertionAverage /= NUM_RUNS;

        // std::cout << "Insertion Sort:       " << insertionSortAverage.count() << " microseconds.\n";
        // std::cout << "Merge Sort:           " << mergeSortAverage.count() << " microseconds.\n";
        // std::cout << "Quick Sort:           " << quickSortAverage.count() << " microseconds.\n";
        // std::cout << "Quick Insertion Sort: " << quickInsertionAverage.count() << " microseconds.\n\n";

        if ((i < 100) && insertionSortAverage.count() < mergeSortAverage.count()) // is < ms
        {
            if (quickSortAverage.count() < quickInsertionAverage.count()) // qs < qi
            {
                if (insertionSortAverage.count() < quickSortAverage.count()) // is < qs < qi; is < ms
                {
                    timesInsertionSortWasFastest.push_back(i);
                }
                else // qs < is < ms; qs < qi
                {
                    timesQuickSortWasFastest.push_back(i);
                }
            }
            else // qi < qs
            {
                if (insertionSortAverage.count() < quickInsertionAverage.count()) // is < qi < qs; is < ms
                {
                    timesInsertionSortWasFastest.push_back(i);
                }
                else // qi < is < ms; qi < qs
                {
                    timesQuickInsertionSortWasFastest.push_back(i);
                }
            }
        }
        else // ms <= is
        {
            if (quickSortAverage.count() < quickInsertionAverage.count()) // qs < qi; ms <= is
            {
                if (quickSortAverage.count() < mergeSortAverage.count()) // qs < ms < is; qs < qi
                {
                    timesQuickSortWasFastest.push_back(i);
                }
                else // ms <= qs < qi; ms <= is
                {
                    timesMergeSortWasFastest.push_back(i);
                }
            }
            else // qi <= qs; ms <= is
            {
                if (quickInsertionAverage.count() < mergeSortAverage.count())
                {
                    timesQuickInsertionSortWasFastest.push_back(i);
                }
                else
                {
                    timesMergeSortWasFastest.push_back(i);
                }
            }
        }
    }

    std::cout << "\nArray sizes where insertion sort was best:\n";
    for (auto t : timesInsertionSortWasFastest)
    {
        std::cout << t << ' ';
    }
    std::cout << '\n';

    std::cout << "\nArray sizes where merge sort was best:\n";
    for (auto t : timesMergeSortWasFastest)
    {
        std::cout << t << ' ';
    }
    std::cout << '\n';

    std::cout << "\nArray sizes where quick sort was best:\n";
    for (auto t : timesQuickSortWasFastest)
    {
        std::cout << t << ' ';
    }
    std::cout << '\n';

    std::cout << "\nArray sizes where quick insertion sort was best:\n";
    for (auto t : timesQuickInsertionSortWasFastest)
    {
        std::cout << t << ' ';
    }
    std::cout << '\n';
}

template <typename T>
void SortCLI<T>::printCommands() const
{
    std::cout << "Press (h) for help, (q) to quit.\n";
    std::cout << "\nSorting types:\n";
    std::cout << "  (is)  Insertion Sort\n";
    std::cout << "  (ms)  Mergesort\n";
    std::cout << "  (qs)  Quicksort\n";
    std::cout << "  (qi)  Quick Insertion Sort\n\n";
}

// assumes input will be in lowercase, convert prior to calling
template <typename T>
void SortCLI<T>::handleCommands(std::string input)
{
    if (input == "is")
    {
        runSortCommand(SortType::Insertion_Sort);
    }
    else if (input == "ms")
    {
        runSortCommand(SortType::Merge_Sort);
    }
    else if (input == "qs")
    {
        runSortCommand(SortType::Quick_Sort);
    }
    else if (input == "qi")
    {
        runSortCommand(SortType::Quick_Insertion);
    }
    else if (input == "h")
    {
        printCommands();
    }
    else if (input == "q")
    {
        exit(0);
    }
    else
    {
        std::cout << "Error: Unknown command: " << input << '\n';
    }
}

const std::string arraySizePrompt = "Array Size: ";
const std::string kPrompt = "Enter K for Quick Insertion Sort: ";

template <typename T>
void SortCLI<T>::runSortCommand(SortType sortType)
{
    int arraySize = getIntegerFromInput(arraySizePrompt);

    int k;
    if (sortType == SortType::Quick_Insertion)
    {
        k = getIntegerFromInput(kPrompt);
    }

    std::vector<T> *vector = generateRandomVector(arraySize);

    std::cout << "\n--------- UNSORTED ARRAY --------\n";
    printVector(vector);

    std::string sortName;
    microseconds sortTime;

    switch (sortType)
    {
    case SortType::Insertion_Sort: // start timers at the last possible moment before sorting.

        sortName = "Insertion Sort";

        startTimer();
        InsertionSort<T>::Sort(vector);
        sortTime = stopTimer();

        break;

    case SortType::Quick_Insertion:

        sortName = "Quick Insertion Sort";

        startTimer();
        QuickInsertionSort<T>::Sort(vector, k);
        sortTime = stopTimer();

        break;

    case SortType::Quick_Sort:

        sortName = "Quick Sort";

        startTimer();
        QuickSort<T>::Sort(vector);
        sortTime = stopTimer();

        break;

    case SortType::Merge_Sort:

        sortName = "Merge Sort";

        startTimer();
        MergeSort<T>::Sort(vector);
        sortTime = stopTimer();

        break;
    }

    std::cout << "\n--------- SORTED ARRAY --------\n";
    printVector(vector);

    std::cout << '\n'
              << sortName << " took: " << sortTime.count() << " microseconds.\n";

    delete vector;
}

// vector pointers returned by this function must be freed
template <typename T>
std::vector<T> *SortCLI<T>::generateRandomVector(int size) const
{
    std::vector<T> *vector = new std::vector<T>();
    vector->reserve(size);

    // random code from https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1000000);

    for (int i = 0; i < size; i++)
    {
        vector->push_back(dist(rng));
    }

    return vector;
}

template <typename T>
void SortCLI<T>::printVector(const std::vector<T> *vector) const
{
    int size = vector->size();
    if (vector->size() < 100)
    {
        for (int i = 0; i < size; i++)
        {
            std::cout << (*vector)[i] << ' ';
        }
        std::cout << '\n';
    }
    else
    {
        int numToPrint = 50;
        for (int i = 0; i < numToPrint; i++)
        {
            std::cout << (*vector)[i] << ' ';
        }
        std::cout << "\n . . . . . . . . . . .\n";
        for (int i = vector->size() - numToPrint; i < vector->size(); i++)
        {
            std::cout << (*vector)[i] << ' ';
        }
        std::cout << '\n';
        std::cout << "\nLarge array "
                  << "(size: " << vector->size() << "): only the first and last " << 50 << " elements were printed.\n";
    }
}

template <typename T>
int SortCLI<T>::getIntegerFromInput(std::string prompt) const
{
    while (true)
    {
        std::cout << prompt;

        std::string input;
        std::getline(std::cin, input);

        try
        {
            int number = std::stoi(input);
            return number;
        }
        catch (const std::invalid_argument &exception)
        {
            std::cout << "Unable to read input as number... ";
            std::cout << exception.what() << '\n';
        }
        catch (const std::out_of_range &exception)
        {
            std::cout << "Unable to read input as number... ";
            std::cout << exception.what() << '\n';
        }
        catch (...)
        {
            std::cout << "Error reading input as number...\n";
        }
    }
}

template <typename T>
void SortCLI<T>::lowercaseString(std::string &s) const
{
    int length = s.length();
    for (int i = 0; i < length; i++)
    {
        s[i] = tolower(s[i]);
    }
}

template <typename T>
void SortCLI<T>::startTimer()
{
    if (timerStarted)
        throw "SortCLI startTimer was called but existing timer was already running.";

    timerStarted = true;
    startTime = high_resolution_clock::now();
}

template <typename T>
microseconds SortCLI<T>::stopTimer()
{
    time_point<high_resolution_clock> stopTime = high_resolution_clock::now();

    if (!timerStarted)
        throw "SortCLI timer was stopped but has never been started.";

    timerStarted = false;

    auto duration = std::chrono::duration_cast<microseconds>(stopTime - startTime);
    return duration;
}

template class SortCLI<uint32_t>;