// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#include "Animal.h"
// Constructor
Animal::Animal(std::string name, double weight) // Default
    : name(name), weight(weight)
{
    std::cout << "Animal Constructor" << std::endl;
};

// Getters
std::string Animal::getName() const
{
    return name;
}

double Animal::getWeight() const
{
    return weight;
}