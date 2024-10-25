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

protected:
    // Member vars
    std::string name;
    double weight;
};

#endif