// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#include "Animal.h"
// constructor
Animal::Animal(std::string name, double weight) // Default: "Unknown", 0.0f
    : name(name), weight(weight)
{
    std::cout << "Animal Constructor" << std::endl;
};

// GETTERS
std::string Animal::getName() const
{
    return name;
}

double Animal::getWeight() const
{
    return weight;
}
