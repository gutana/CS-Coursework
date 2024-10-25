// Temperature.cpp
//
// Mohammad Al-Bayati
// ID: 200339106
// CS115 Fall 2021

#include "Temperature.h"

Temperature::Temperature() // Default constructor
{
}

Temperature::Temperature(const Temperature &other) // Copy Constructor
{
    maxTemperature = other.maxTemperature;
    minTemperature = other.minTemperature;
    avgTemperature = other.avgTemperature;
}

// GETTERS

float Temperature::getMaxTemp() const
{
    return maxTemperature;
}

float Temperature::getMinTemp() const
{
    return minTemperature;
}

float Temperature::getAvgTemp() const
{
    return avgTemperature;
}

void Temperature::print() const // Print the maxTemperature, minTemperature, and avgTemperature values of the object
{
    std::cout << std::setprecision(3); // one decimal place is enough for something like degrees
    std::cout << std::setw(5) << minTemperature << "|  " << std::setw(5) << maxTemperature << "|  " << std::setw(5) << avgTemperature;

    // Output: "  -65  |  54   |  -18  "
}

void Temperature::getTemperature(const std::string &month) // Get temperature from console
{
    do
    {
        maxTemperature = getDouble("Please enter the Maximum temperature for " + month + ": ");
        minTemperature = getDouble("Please enter the Minimum temperature for " + month + ": ");
        avgTemperature = getDouble("Please enter the Average temperature for " + month + ": ");

    } while (!numbersMakeSense(minTemperature, maxTemperature, avgTemperature)); // Loop to ensure numbers are reasonable
}

double Temperature::getDouble(const std::string &s) // Print string and get double from console
{
    double num = -100; // Initial value that must be changed to return
    do
    {
        std::cout << s;      // Print s
        std::cin >> num;     // get Num
        if (std::cin.fail()) // If errors (weird input, wrong type, etc), clear errors and reset the value to ensure loop.
        {
            std::cin.clear();
            std::cin.ignore(256, '\n');
            num = -100;
        }
    } while (!isValidTemperature(num)); // Ensures temperature is within max/min range
    return num;
}

bool Temperature::isValidTemperature(double &t) const
{
    if (t < MIN_TEMP || t > MAX_TEMP)
    {
        std::cout << "The temperature entered is invalid.\n";
        return false;
    }
    return true;
}

bool Temperature::numbersMakeSense(double &min, double &max, double &avg) const // To make sure min <= avg <= max
{
    if (min <= avg && avg <= max)
    {
        return true;
    }
    std::cout << "Sorry, your numbers don't make sense. Try again.\n";
    return false;
}