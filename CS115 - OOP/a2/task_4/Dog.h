// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#ifndef DOG_H
#define DOG_H

#include "Animal.h"
#include <iomanip>

class Dog : public Animal
{
public:
    // Constructors
    Dog();
    Dog(std::string name, double weight);
    Dog(std::string name, double weight, std::string breed);

    // Getter
    std::string getBreed() const;

    // Bark to console. BIG bark if big dog
    void bark() const;

private:
    std::string breed;
};

// overloaded << so we can print dog to console.
std::ostream &operator<<(std::ostream &o, const Dog &c);

#endif