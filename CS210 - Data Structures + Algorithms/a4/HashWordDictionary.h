#ifndef HASH_DICTIONARY
#define HASH_DICTIONARY

#include "WordDictionary.h"
#include "WordData.h"
#include "WordNode.h"
#include "DuplicateKeyException.h"

#include <vector>

class HashWordDictionary : public WordDictionary
{ // Dictionary ADT
private:
    WordNode **table;                    // array of linked lists
    int numElem;                         // number of elements
    int tableSize = 100;                 // size of the table, default is 100
    int hashFunction(std::string input); // compute the hash function

public:
    HashWordDictionary();                  // default constructor
    HashWordDictionary(int M);             // size of the table
    ~HashWordDictionary();                 // deconstructor
    void put(WordData *word);              // put record (key, data) into dictionary, do not put in duplicates.
    WordData *get(std::string key);        // Return the value of the record with key.
    int size();                            // how many elements are in the dictionary?
    int M();                               // what is the table size? (specifically for the hash table)
    bool isEmpty();                        // is the dictionary empty?
    std::vector<WordData *> listRecords(); // returns a vector with all the records!
};

#endif