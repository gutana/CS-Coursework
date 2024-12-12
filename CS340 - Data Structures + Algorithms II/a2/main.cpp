// Mohammad Al-Bayati
// 200339106
// CS340

// INPUT: program will take input at runtime via interactive terminal

#include <iostream>

#include "MinMaxHeapCLI.h"

int main(int argc, char *argv[])
{
    try
    {
        MinMaxHeapCLI cli = MinMaxHeapCLI<float>();
        cli.Run();
    }
    catch (const char *msg)
    {
        std::cout << "Error: " << msg << '\n';
    }
}