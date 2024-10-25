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
    Animal(std::string name = "Unknown", double weight = 0.0);

public:
    // Getters
    std::string getName() const;
    double getWeight() const;

    // Task 3:
    // Template to let us copy subclasses. e.g. cat1 = cat2
    // NOTE: This does NOT WORK. operator= is not inherited and
    // the default assignment operator will be called
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