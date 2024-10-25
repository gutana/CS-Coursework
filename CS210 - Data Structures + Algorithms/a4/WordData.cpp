#include "WordData.h"
#include <string>
/*
    Word Data - this is record information, where a key is the string
    and its data is the word data (the string and its frequency).
    We refer to the key as the string (word) and the data as the frequency (freq).
    Author: Daniel Page
    Date: Winter 2022
*/

// default constructor
WordData::WordData()
{
    word = nullptr;
    freq = 1; // the word itself is one.
}

// constructor with a word, but no frequency information
WordData::WordData(std::string word)
{
    this->word = word;
    freq = 1;
}

// constructor for including defined key and value pair (in our case the word, and the frequency).
WordData::WordData(std::string word, int freq)
{
    this->word = word;
    this->freq = freq;
}

// deconstructor
WordData::~WordData()
{
    // just going to clear out the string.
    this->word.clear();
}

// get the key
std::string &WordData::getKey()
{
    return this->word;
}

// get the value/data
int WordData::getValue()
{
    return this->freq;
}

// set the key
void WordData::setKey(std::string &word)
{
    this->word = word; // update the key
}

// set the value/data
void WordData::setValue(int freq)
{
    this->freq = freq;
}

// return the key as a string
std::string WordData::keyString()
{
    return this->word;
}

// return the data/value as a string
std::string WordData::valueString()
{
    return std::to_string(this->freq);
}

// return the record data
std::string WordData::toString()
{
    return "[" + this->word + "," + std::to_string(this->freq) + "]";
}