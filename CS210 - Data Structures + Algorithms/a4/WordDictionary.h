#ifndef DICTIONARY_ADT
#define DICTIONARY_ADT

#include "WordData.h"
#include <vector>

class WordDictionary
{ // Dictionary ADT
public:
    virtual void put(WordData *word) = 0;              // put record (key, data) into dictionary, do not put in duplicates.
    virtual WordData *get(std::string key) = 0;        // Return the word, given a key (if exists).
    virtual int size() = 0;                            // how many elements are in the dictionary?
    virtual bool isEmpty() = 0;                        // is the dictionary empty?
    virtual std::vector<WordData *> listRecords() = 0; // returns a list of the records.
};

#endif