// Rainfall.h
//
// Mohammad Al-Bayati
// ID: 200339106
// CS115 Fall 2021

#ifndef RAINFALL_H
#define RAINFALL_H

#pragma once

#include <iostream> // For console
#include <iomanip>  // For console
#include <string>   // Need to use strings

class Rainfall
{
private:
    int maxRainfall = 0; // default value of 0, which is not valid. this will guarantee defaults must be overwritten
    int minRainfall = 0;
    int avgRainfall = 0;

    // Max/min allowable rainfall values. used by isValidRainfall()
    static const int MIN_RAIN = 1;
    static const int MAX_RAIN = 25;

public:
    Rainfall();                             // Default Constructor
    Rainfall(int &max, int &min, int &avg); // Constructor directly taking all values
    Rainfall(const Rainfall &other);        // Copy Constructor

    // Getters
    int getMinRain() const;
    int getMaxRain() const;
    int getAvgRain() const;

    void print() const;                         // Print the maxRainfall, minRainfall, and avgRainfall values of the object
    void getRainfall(const std::string &month); // Get rainfall from console

private:
    bool isValidRainfall(int r) const;                      // Verifies rainfall within allowable ranges
    int getInt(std::string s);                              // Output string and return int from console
    bool numbersMakeSense(int min, int max, int avg) const; // Verifies min <= avg <= max
};

#endif