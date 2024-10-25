// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Oct 1 2021
// Class: CS 115
//
// main.cpp
//
// Problem Statement:
//  This program accepts inputs for the min/max/avg temp and rainfall for 12 months
//  and prints a formatted table consisting of those inputs and the yearly averages of
//  that inputted data.

#include "weather.h" // Weather/Rain/Temp structs are here, as well as relevant functions

int main()
{
    const int NUM_MONTHS = 12;

    // Declare our array of Weathers
    weather weatherArray[NUM_MONTHS];

    for (int i = 0; i < NUM_MONTHS; i++)
    {
        // getWeatherInput prompts user for input for a given month, and returns
        // a weather struct.
        weatherArray[i] = getWeatherInput(i); // i is the month, e.g. jan = 0;
    }

    // Print the top of our table
    printTableHeader();

    for (int i = 0; i < NUM_MONTHS; i++)
    {
        // Print the row for each month.
        printWeather(weatherArray, i); // i is the month, i.g. jan = 0
    }

    // Finally, print the yearly averages.
    printAverages(weatherArray, NUM_MONTHS);

    return 0;
}
