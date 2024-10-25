// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include <iostream>

class Animal
{
protected: // Accessible by Animal and classes that inherit it
    // Constructor
    Animal(std::string name = "Unknown", double weight = 0.0f);

public:
    // Getters
    std::string getName() const;
    double getWeight() const;

    // Template function so we can copy subclasses. e.g. Cat2 = Cat1
    template <typename T>
    T operator=(const T &other)
    {
        if (this != &other)
        {
            name = other.name;
            weight = other.weight;
        }
        return *this;
    }

protected:
    // Member vars
    std::string name;
    double weight;
};

#endif