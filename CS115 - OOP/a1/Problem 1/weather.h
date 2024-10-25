// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Oct 1 2021
// Class: CS 115
//
// Weather.h

#ifndef WEATHER_H
#define WEATHER_H

#include <string>
#include <iostream>
#include <iomanip>

struct temperature
{
    // Default invalid temperatures, must be overwritten
    double maxTemperature = -100;
    double minTemperature = -100;
    double avgTemperature = -100;
};

struct rainfall
{
    // Default invalid rainfalls, must be overwritten
    int maxRainfall = -1;
    int minRainfall = -1;
    int avgRainfall = -1;
};

struct weather
{
    rainfall rain;
    temperature temp;
};

// A simple array of string for each month. Makes printing easier.
const std::string MONTH_TEXT_ARR[12] = {
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
    "December"};

// Maximum and minimum values for our valid range
const double MAX_TEMP = 60.0f;
const double MIN_TEMP = -70.0f;
const int MIN_RAIN = 1;
const int MAX_RAIN = 25;

// Gets the information for a weather/rain/temperature
// struct from console and returns it.
weather getWeatherInput(int month);
rainfall getRain(int month);
temperature getTemp(int month);

// Get input for console. s is month name, int is month number.
int getRainInput(std::string s, int month);
double getTempInput(std::string s, int month);

// Prints a string to console, returns inputted value
int getIntInput(std::string &s);
double getDoubleInput(std::string &f);

// Validity checks to ensure value is within appropriate range
bool isValidRainfall(int r);
bool isValidTemp(float t);

// Printing functions
void printTableHeader();
void printWeather(weather w[], int month);
void printRain(const rainfall &r);
void printTemp(const temperature &t);
void printAverages(const weather[], int length);

// Simple functions to ensure minimum <= average <= maximum
bool numbersMakeSense(rainfall &r);
bool numbersMakeSense(temperature &t);

#endif