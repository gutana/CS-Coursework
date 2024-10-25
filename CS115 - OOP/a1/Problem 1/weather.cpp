// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Oct 1 2021
// Class: CS 115
//
// Weather.cpp

#include "weather.h"

// Will prompt user for all values of a weather, and return that struct
weather getWeatherInput(int month)
{
    rainfall tempRain;
    temperature tempTemp;

    do // Loop that will happen if inputs don't make sense (e.g. min > max)
    {
        tempRain = getRain(month); // get input
        if (!numbersMakeSense(tempRain))
        {
            // if input was bad
            std::cout << "Sorry, but your rainfall numbers for " << MONTH_TEXT_ARR[month] << " don't make sense. Try again.\n";
        }

    } while (!numbersMakeSense(tempRain));

    do // Loop that will happen if inputs don't make sense (e.g. min > max)
    {
        tempTemp = getTemp(month); // get input
        if (!numbersMakeSense(tempTemp))
        {
            // if input was bad
            std::cout << "Sorry, but your temperature numbers for " << MONTH_TEXT_ARR[month] << " don't make sense. Try again.\n";
        }
    } while (!numbersMakeSense(tempTemp));

    // create and return a weather.
    return weather{
        tempRain, tempTemp};
}

rainfall getRain(int month)
{
    // Construct a rainfall and return it.
    // For each value, we call getRainInput which will prompt user and return the #
    return rainfall{
        getRainInput("Maximum", month), getRainInput("Minimum", month), getRainInput("Average", month)};
}

temperature getTemp(int month)
{
    // Construct a temperature and return it.
    // For each value, we call getTempInput which will prompt user and return the #
    return temperature{
        getTempInput("Maximum", month), getTempInput("Minimum", month), getTempInput("Average", month)};
}

int getRainInput(std::string s, int month)
{
    // this function makes the full prompt string, and uses that with getIntInput
    // to return an integer.
    std::string prompString = "Please input the " + s + " Rainfall for ";
    prompString += MONTH_TEXT_ARR[month];
    prompString += ": ";
    return getIntInput(prompString);
}

double getTempInput(std::string s, int month)
{
    // this function makes the full prompt string, and uses that with getDoubleInput
    // to return a double.
    std::string prompString = "Please input the " + s + " Temperature for ";
    prompString += MONTH_TEXT_ARR[month];
    prompString += ": ";
    return getDoubleInput(prompString);
}

int getIntInput(std::string &s)
{
    int num = -1;

    do // Loops if bad/unchanged input
    {
        std::cout << s;
        std::cin >> num;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(256, '\n');
            num = -1;
        }
    } while (!isValidRainfall(num)); // range check

    return num;
}

double getDoubleInput(std::string &s)
{
    double d = -100.0f;

    do // loop if bad/unchanged input
    {
        std::cout << s;
        std::cin >> d;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(256, '\n');
            d = -100.0f;
        }
    } while (!isValidTemp(d)); // range check

    return d;
}

bool isValidRainfall(int r) // Range check
{
    if (r < MIN_RAIN || r > MAX_RAIN)
    {
        std::cout << "The rainfall entered is invalid.\n";
        return false;
    }
    return true;
}

bool isValidTemp(float t) // Range check
{
    if (t < MIN_TEMP || t > MAX_TEMP)
    {
        std::cout << "The temperature entered is invalid.\n";
        return false;
    }
    return true;
}

//// PRINTING FUNCTIONS ////
void printTableHeader()
{
    std::cout << "\n+----------+------TEMPERATURE------+--------RAINFALL-------+\n";
    std::cout << "| Month    |  min  |  max  |  avg  |  min  |  max  |  avg  |\n";
    std::cout << "+----------+-------+-------+-------+-------+-------+-------+\n";
}

void printWeather(weather wArray[], int i)
{
    std::cout << "| " << std::setw(9) << std::left << MONTH_TEXT_ARR[i] << "|  "; //
    printTemp(wArray[i].temp);
    std::cout << "|  ";
    printRain(wArray[i].rain);
}

void printRain(const rainfall &r)
{
    std::cout << std::setprecision(3);
    std::cout << std::setw(5) << r.minRainfall << "|  " << std::setw(5) << r.maxRainfall << "|  " << std::setw(5) << r.avgRainfall << "|\n";
}

void printTemp(const temperature &t)
{

    std::cout << std::setprecision(3); // one decimal place is enough for something like degrees
    std::cout << std::setw(5) << t.minTemperature << "|  " << std::setw(5) << t.maxTemperature << "|  " << std::setw(5) << t.avgTemperature;
}

// Calculates and prints averages. We don't use them after printing, so no point in storing anywhere
void printAverages(const weather wArray[], int length)
{
    float avgMinTemp = 0;
    float avgAvgTemp = 0;
    float avgMaxTemp = 0;
    float avgMinRain = 0;
    float avgAvgRain = 0;
    float avgMaxRain = 0;

    for (int i = 0; i < length; i++)
    {
        avgMinTemp += wArray[i].temp.minTemperature;
        avgAvgTemp += wArray[i].temp.avgTemperature;
        avgMaxTemp += wArray[i].temp.maxTemperature;
        avgMinRain += wArray[i].rain.minRainfall;
        avgAvgRain += wArray[i].rain.avgRainfall;
        avgMaxRain += wArray[i].rain.maxRainfall;
    }
    avgMinTemp /= length;
    avgAvgTemp /= length;
    avgMaxTemp /= length;
    avgMinRain /= length;
    avgAvgRain /= length;
    avgMaxRain /= length;

    std::cout << "+----------+-------+-------+-------+-------+-------+-------+\n";
    // Prints a line that looks like this:
    // | Mean     |  -54.7|  48.1 |  2.05 |  2.5  |  19.9 |  10   |
    std::cout << "| Mean     |  " << std::setw(5) << avgMinTemp << "|  " << std::setw(5) << avgMaxTemp << "|  " << std::setw(5) << avgAvgTemp << "|  ";
    std::cout << std::setw(5) << avgMinRain << "|  " << std::setw(5) << avgMaxRain << "|  " << std::setw(5) << avgAvgRain << "|\n";
    std::cout << "+----------+-------+-------+-------+-------+-------+-------+\n";
}

// Validity checks to ensure min doesn't exceed max, etc..
bool numbersMakeSense(rainfall &r)
{
    if (r.minRainfall <= r.avgRainfall && r.avgRainfall <= r.maxRainfall)
    {
        return true;
    }
    else
        return false;
}
bool numbersMakeSense(temperature &t)
{
    if (t.minTemperature <= t.avgTemperature && t.avgTemperature <= t.maxTemperature)
    {
        return true;
    }
    else
        return false;
}