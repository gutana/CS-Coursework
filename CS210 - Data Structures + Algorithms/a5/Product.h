#ifndef PRODUCT
#define PRODUCT
#include <string>
class Product
{
private:
    int key;              // key, every location has a unique name
    std::string prodName; // product name
    int quantity;         // quantity of the product
public:
    Product();                                              // constructor of an unnamed product, no assigned key with no cost
    Product(int myKey);                                     // makes a product with a key, and no other data
    Product(int myKey, std::string myName, int myQuantity); // provide a complete set of data for a record (product)
    int compare(Product *otherProd);                        // compares two products by their keys, <0 if our product comes before, 0 is the same, and >0 if comes after.
    int getKey();                                           // get the key
    std::string getName();                                  // get the name
    int getQuantity();                                      // get the quantity
    void setKey(int newKey);                                // update its key/id
    void setName(std::string newName);                      // update the name
    void decreaseQuantity();                                // decrease the quantity by one
    void increaseQuantity(int newStock);                    // increase quantity by newStock.
    void setQuantity(int newQuantity);                      // set directly the quantity
    std::string toString();                                 // output the data of the product
};
#endif