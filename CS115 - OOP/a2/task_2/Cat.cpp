// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#include "Cat.h"

// CONSTRUCTORS
Cat::Cat()
    : Animal(),
      temperament("Unknown")
{
}

Cat::Cat(std::string name, double weight)
    : Animal(name, weight),
      temperament("Unknown")
{
}

Cat::Cat(std::string name, double weight, std::string temperament)
    : Animal(name, weight),
      temperament(temperament)
{
}

Cat::Cat(const Cat &other)
    : Animal(other.name, other.weight),
      temperament(other.temperament)
{
}

// operator= so we can do Cat1 = Cat2, etc.
Cat Cat::operator=(const Cat &other)
{
    if (&other != this)
    {
        name = other.name;
        weight = other.weight;
        temperament = other.temperament;
    }
    return *this;
}

// getter
std::string Cat::getTemperament() const
{
    return temperament;
}

// meow to console
void Cat::meow() const
{
    std::cout << name << " goes meow!\n";
}

// overloaded << operator allows us to print cat to console
std::ostream &operator<<(std::ostream &o, const Cat &c)
{
    o << std::setprecision(2) << std::fixed << "Cat name: " << c.getName() << ", Weight: " << c.getWeight() << ", Temperament: " << c.getTemperament() << '\n';
    return o;
}