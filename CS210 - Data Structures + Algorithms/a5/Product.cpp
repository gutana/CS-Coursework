#include "Product.h"

// constructor of an unnamed product, no assigned key with no cost
Product::Product()
{
    key = -1;
    prodName = "";
    quantity = 1;
}

// makes a product with a key, and no other data
Product::Product(int myKey)
{
    key = myKey;
    prodName = "";
    quantity = 1;
}

// provide a complete set of data for a record (product)
Product::Product(int myKey, std::string myName, int myQuantity)
{
    key = myKey;
    prodName = myName;
    quantity = myQuantity;
}

// compares two products by their keys, <0 if our product comes before, 0 is the same, and >0 if comes after.
int Product::compare(Product *otherProd)
{
    if (otherProd == nullptr)
    {
        return 0;
    }
    else
    { // otherwise we compare their keys
        if (key < otherProd->getKey())
        {
            return -1; // key comes before
        }
        else if (key == otherProd->getKey())
        {
            return 0; // same key
        }
        else
        {
            return 1; // key comes after
        }
    }
}

// get the key
int Product::getKey()
{
    return key;
}

// get the name
std::string Product::getName()
{
    return prodName;
}

// get the quantity
int Product::getQuantity()
{
    return quantity;
}

// update its key/id
void Product::setKey(int newKey)
{
    key = newKey;
}

// update its name
void Product::setName(std::string newName)
{
    prodName = newName;
}

// decrease the quantity by one
void Product::decreaseQuantity()
{
    quantity = quantity - 1;
}

// increase quantity by newStock
void Product::increaseQuantity(int newStock)
{
    quantity = quantity + newStock;
}

// set directly the quantity
void Product::setQuantity(int newQuantity)
{
    quantity = newQuantity;
}

// toString
std::string Product::toString()
{
    std::string myString = "[";
    myString = myString + std::to_string(getKey()) + ",";
    myString = myString + getName() + ",";
    myString = myString + std::to_string(getQuantity()) + "]";
    return myString;
}