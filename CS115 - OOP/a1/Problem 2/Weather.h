// Weather.h
//
// Mohammad Al-Bayati
// ID: 200339106
// CS115 Fall 2021

#ifndef WEATHER_H
#define WEATHER_H

#pragma once

#include <iostream> // For console stuff
#include <iomanip>  // Also console stuff
#include <vector>   // The printWeatherAvgs function needs to know what a vector is

#include "Temperature.h"
#include "Rainfall.h"

class Weather
{
public:
    Rainfall rain;     // contains min/max/avg rainfall for one month
    Temperature temp;  // contains min/max/avg temperature for one month
    std::string month; // string for this month

public:
    Weather();                         // Default constructor
    Weather(const std::string &month); // Constructor accepting string for the month name
    Weather(const Weather &other);     // Copy constructor

    void getWeather();         // Get data for weather object from console
    void printWeather() const; // Print the weather data for this month.
                               // Prints a single row in the final output table.

    // These will print the table. Don't need to be associated with any instance,
    // so they're static.
    static void printHeader();
    static void printWeatherAvgs(const std::vector<Weather> &wVec, int length);
};

#endif