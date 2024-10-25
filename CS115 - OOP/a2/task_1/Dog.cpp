// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#include "Dog.h"

// Constructors
Dog::Dog()
    : Animal(),
      breed("Unknown")
{
}

Dog::Dog(std::string name, double weight)
    : Animal(name, weight),
      breed("Unknown")
{
}

Dog::Dog(std::string name, double weight, std::string breed)
    : Animal(name, weight),
      breed(breed)
{
}

// Getter
std::string Dog::getBreed() const
{
    return breed;
}

// Dork will bark to console. will do BIG bark if BIG dog
void Dog::bark() const
{
    std::cout << ((weight > 25) ? "BIG dog goes BARKK!" : "Dog goes bark!") << '\n';
}

// overloaded << so we can print dog to console
std::ostream &operator<<(std::ostream &o, const Dog &d)
{
    o << std::setprecision(2) << std::fixed << "Dog name: " << d.getName() << ", Weight: " << d.getWeight() << ", Breed: " << d.getBreed() << '\n';
    return o;
}