// main.cpp
//
// Mohammad Al-Bayati
// ID: 200339106
// CS115 Fall 2021
//
// Problem Statement:
//     The program will accept, as console input, the maximum/minimum/average rainfall and
//     temperature for each month of a year. It will then display these numbers in a
//     formatted table and display the means of each of min/max/avg rain/temp for the entire
//     year.

#include <vector> // to store Weather objects
#include "Weather.h"

const int NUM_MONTHS = 12; // This program will work with any number of months.
                           // Months after December will loop back around (e.g. month 13 will be "January" )

const std::string monthArray[] = {
    // An array of strings representing months, to make printing convenient.
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
};

int main()
{
    std::vector<Weather> weatherVector; // Declare weather vector
    weatherVector.reserve(NUM_MONTHS);  // Reserve data we need to minimize reallocations

    // Initialize each month, giving each its respective string e.g. 0 = "January"
    for (int i = 0; i < NUM_MONTHS; i++)
    {
        // construct weathers and put them into the array. We give them their "name"
        weatherVector[i] = Weather(monthArray[i % 12]);
    }

    // Get weather input from console
    for (int i = 0; i < NUM_MONTHS; i++)
    {
        weatherVector[i].getWeather();
    }

    // Print the top of the output table
    Weather::printHeader();
    // Print the table of weathers
    for (int i = 0; i < NUM_MONTHS; i++)
    {
        weatherVector[i].printWeather();
    }

    // Calculate the averages, print it out at the end of the table.
    Weather::printWeatherAvgs(weatherVector, NUM_MONTHS);

    return 0;
}