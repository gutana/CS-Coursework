// Weather.cpp
//
// Mohammad Al-Bayati
// ID: 200339106
// CS115 Fall 2021

#include "Weather.h"

Weather::Weather() // Default constructor, set month string to unknown
{
    month = "Unknown"; // This shouldn't happen.
}

Weather::Weather(const std::string &m) // Constructor accepting month string
{
    month = m;
}

Weather::Weather(const Weather &other) // Copy constructor
{
    month = other.month;
    rain = other.rain;
    temp = other.temp;
}

void Weather::getWeather() // Get weather input from console
{
    rain.getRainfall(month);
    temp.getTemperature(month);
}

void Weather::printWeather() const // Prints weather
{
    std::cout << "| " << std::setw(9) << std::left << month << "|  "; // Output: "| [MONTH] |  "

    temp.print();
    std::cout << "|  ";
    rain.print();
}

void Weather::printHeader() // Prints the top of a table to console
{
    std::cout << "\n+----------+------TEMPERATURE------+--------RAINFALL-------+\n";
    std::cout << "| Month    |  min  |  max  |  avg  |  min  |  max  |  avg  |\n";
    std::cout << "+----------+-------+-------+-------+-------+-------+-------+\n";
}

void Weather::printWeatherAvgs(const std::vector<Weather> &wVec, int length) // Calculates the average of each variable in weather and prints
{
    double avgMinTemp, avgMaxTemp, avgAvgTemp, avgMinRain, avgMaxRain, avgAvgRain;

    // Init all to 0
    avgMinTemp = 0;
    avgMaxTemp = 0;
    avgAvgTemp = 0;
    avgMinRain = 0;
    avgMaxRain = 0;
    avgAvgRain = 0;

    for (int i = 0; i < length; i++) // Add them up
    {
        avgMinTemp += wVec[i].temp.getMinTemp();
        avgMaxTemp += wVec[i].temp.getMaxTemp();
        avgAvgTemp += wVec[i].temp.getAvgTemp();
        avgMinRain += wVec[i].rain.getMinRain();
        avgMaxRain += wVec[i].rain.getMaxRain();
        avgAvgRain += wVec[i].rain.getAvgRain();
    }
    avgMinTemp /= length; // Divide to get the average
    avgMaxTemp /= length;
    avgAvgTemp /= length;
    avgMinRain /= length;
    avgMaxRain /= length;
    avgAvgRain /= length;

    // Prints output that look like this:
    // +----------+-------+-------+-------+-------+-------+-------+
    // | Mean     |  -54.7|  48.1 |  2.05 |  2.5  |  19.9 |  10   |
    // +----------+-------+-------+-------+-------+-------+-------+
    std::cout << "+----------+-------+-------+-------+-------+-------+-------+\n";
    std::cout << "| Mean     |  " << std::setw(5) << avgMinTemp << "|  " << std::setw(5) << avgMaxTemp << "|  " << std::setw(5) << avgAvgTemp << "|  ";
    std::cout << std::setw(5) << avgMinRain << "|  " << std::setw(5) << avgMaxRain << "|  " << std::setw(5) << avgAvgRain << "|\n";
    std::cout << "+----------+-------+-------+-------+-------+-------+-------+\n";
}
