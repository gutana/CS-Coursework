// Mohammad Al-Bayati
// 200339106
// CS340

// INPUT: program will take input at runtime via interactive terminal

#include <iostream>
#include <cstdint>

#include "SortCLI.h"
#include "algorithms/SortingAlgorithms.h"

int main(int argc, char *argv[])
{
    try
    {
        SortCLI<uint32_t> cli = SortCLI<uint32_t>();
        cli.Run();
    }
    catch (const char *msg)
    {
        std::cout << "Error: " << msg << '\n';
    }
    catch (...)
    {
        std::cout << "Unexpected error. Exiting...\n";
    }
}
