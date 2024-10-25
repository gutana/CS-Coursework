// Rainfall.h
//
// Mohammad Al-Bayati
// ID: 200339106
// CS115 Fall 2021

#include "Rainfall.h"

Rainfall::Rainfall()
{
}

Rainfall::Rainfall(int &max, int &min, int &avg) // Directly construct with values.
{
    maxRainfall = max;
    minRainfall = min;
    avgRainfall = avg;
}

Rainfall::Rainfall(const Rainfall &other) // Copy Constructor
{
    maxRainfall = other.maxRainfall;
    minRainfall = other.minRainfall;
    avgRainfall = other.avgRainfall;
}

// GETTERS

int Rainfall::getMaxRain() const
{
    return maxRainfall;
}

int Rainfall::getMinRain() const
{
    return minRainfall;
}

int Rainfall::getAvgRain() const
{
    return avgRainfall;
}

void Rainfall::print() const // Print the maxRainfall, minRainfall, and avgRainfall values of the object
{
    std::cout << std::setprecision(3);
    std::cout << std::setw(5) << minRainfall << "|  " << std::setw(5) << maxRainfall << "|  " << std::setw(5) << avgRainfall << "|\n";
    // Output: "|  3    |  20   |  11  |"
}

void Rainfall::getRainfall(const std::string &month) // Gets all the data for a rainfall from console
{
    do
    {
        maxRainfall = getInt("Please enter the Maximum rainfall for " + month + ": ");
        minRainfall = getInt("Please enter the Minimum rainfall for " + month + ": ");
        avgRainfall = getInt("Please enter the Average rainfall for " + month + ": ");
    } while (!numbersMakeSense(minRainfall, maxRainfall, avgRainfall)); // Sanity check
}

int Rainfall::getInt(std::string s) // Print string and get int from console
{
    int num = -1;

    do
    {
        std::cout << s;
        std::cin >> num;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(256, '\n');
            num = -1;
        }
    } while (!isValidRainfall(num));

    return num;
}

bool Rainfall::isValidRainfall(int r) const // Ensure rainfall is within valid range
{
    if (r < MIN_RAIN || r > MAX_RAIN)
    {
        std::cout << "The rainfall entered is invalid.\n";
        return false;
    }
    return true;
}

bool Rainfall::numbersMakeSense(int min, int max, int avg) const // Ensure min <= avg <= max
{
    if (min <= avg && avg <= max)
    {
        return true;
    }

    std::cout << "Sorry, your numbers don't make sense. Try again.\n";
    return false;
}