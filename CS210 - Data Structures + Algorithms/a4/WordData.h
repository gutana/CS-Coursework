#ifndef WORDDATA
#define WORDDATA
#include <string>

/*
    Word Data - this is record information, where a key is the string
    and its data is the word data (the string and its frequency).
    We refer to the key as the string (word) and the data as the frequency.
    Author: Daniel Page
    Date: Winter 2022
*/

class WordData
{ // Word Data
private:
    std::string word; // the word
    int freq;         // frequency of a word
public:
    WordData();                           // default constructor
    WordData(std::string word);           // provide a string for the constructor
    WordData(std::string word, int freq); // storing the string and frequency
    ~WordData();                          // deconstructor (nothing interesting here)
    std::string &getKey();                // get the key
    int getValue();                       // get the value
    void setKey(std::string &key);        // set the key
    void setValue(int data);              // set the data
    std::string keyString();              // return key as string
    std::string valueString();            // return value as string
    std::string toString();               // return record as string
};
#endif