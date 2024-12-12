// Mohammad Al-Bayati
// 200339106
// CS340

// INPUT: program will take input at runtime via interactive terminal

#include <iostream>
#include <cstdint>

#include "SearchCLI.h"

int main(int argc, char *argv[])
{
    try
    {
        SearchCLI cli;
        cli.Run();
    }
    catch (const char *errorMsg)
    {
        std::cout << "Error: " << errorMsg << '\n';
    }
}
