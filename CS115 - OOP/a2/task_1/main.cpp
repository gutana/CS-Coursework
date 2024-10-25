// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115
//
// main.cpp
//
// Problem Statement:
//  This program will test inheritance, using a base class Animal and two
//  derived classes, Cat and Dog.

#include "Cat.h"
#include "Dog.h"

int main()
{
    // Initialize some cats
    Cat c1;
    Cat c2("Sniffles", 20);
    Cat c3("Mitsy", 50, "Friendly");

    // Initialize some dogs
    Dog d1;
    Dog d2("Gerald", 20);
    Dog d3("Boxer", 50, "German Shepherd");

    // print cats and dogs
    std::cout << c1 << c2 << c3;
    std::cout << d1 << d2 << d3;

    // Let the animals speak
    c1.meow();
    c2.meow();
    c3.meow();

    d1.bark();
    d2.bark();
    d3.bark();

    // end of program
    return 0;
}