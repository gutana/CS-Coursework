// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#ifndef CAT_H
#define CAT_H

#include "Animal.h"
#include <iomanip>

class Cat : public Animal
{
public:
    // Constructors
    Cat();
    Cat(std::string name, double weight);
    Cat(std::string name, double weight, std::string temperament);
    Cat(const Cat &other);

    // Getter
    std::string getTemperament() const;

    // Will meow in console
    void meow() const;

private:
    std::string temperament;
};

// overloaded operator so we can print cat to console with <<
std::ostream &operator<<(std::ostream &o, const Cat &c);

#endif