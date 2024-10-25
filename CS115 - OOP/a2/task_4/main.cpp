// Name:  Mohammad Al-Bayati
// ID:    200339106
// Date:  Nov 19 2021
// Class: CS 115

#include "Cat.h"
#include "Dog.h"

int main()
{
    // Initialize some cat and dog pointers
    Cat *catPtr = new Cat("Mittens", 20.0f, "Lazy");
    Cat *catPtr2 = new Cat("Tom", 15.0f, "Energetic");
    Dog *dogPtr = new Dog("Rufus", 35.0f, "Husky");
    Dog *dogPtr2 = new Dog("Gerald", 50.0f, "Golden Retriever");

    // call some methods
    catPtr->meow();
    dogPtr->bark();

    // Copy cats and dogs
    *catPtr2 = *catPtr;
    *dogPtr = *dogPtr2;

    // Test the cats and dogs after copy
    std::cout << "New cat name: " << catPtr->getName() << '\n';
    std::cout << "New cat2 name: " << catPtr2->getName() << '\n';
    std::cout << "New dog name: " << dogPtr->getName() << '\n';
    std::cout << "New dog2 name: " << dogPtr2->getName() << '\n';

    return 0;
}