// Temperature.h
//
// Mohammad Al-Bayati
// ID: 200339106
// CS115 Fall 2021

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#pragma once

#include <iostream> // For console
#include <iomanip>  // For console
#include <string>   // We need strings

class Temperature
{

private:
    double maxTemperature;
    double minTemperature;
    double avgTemperature;
    static constexpr double MIN_TEMP = -70; // Max and min allowable temperatures. used by isValidTemperature()
    static constexpr double MAX_TEMP = 60.0;

public:
    Temperature();                    // Default constructor
    Temperature(const Temperature &); // Copy Constructor

    // Getters
    float getMaxTemp() const;
    float getMinTemp() const;
    float getAvgTemp() const;

    void print() const;                            // Print the maxTemperature, minTemperature, and avgTemperature values of the object
    void getTemperature(const std::string &month); // get temperature from console

private:
    double getDouble(const std::string &s);                             // outputs prompt string and returns double from console
    bool isValidTemperature(double &n) const;                           // Checks that temperature is within ranges
    bool numbersMakeSense(double &min, double &max, double &avg) const; // Verifies that min < avg < max
};

#endif