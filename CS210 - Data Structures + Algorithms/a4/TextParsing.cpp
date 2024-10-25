#include "WordNode.h"
#include "WordData.h"
#include "WordDictionary.h"
#include "DuplicateKeyException.h"
#include "HashWordDictionary.h"
#include "TextParser.cpp"
#include <iostream>
#include <vector>

/*
    Main text parsing file.  It reads from words.txt a dictionary of English words.
    We must provide one command line argument that is a text file.

    Author: Daniel Page

*/
int main(int argc, char *argv[])
{
    // ensure we give a text file to the program that contains text, it must be stressed that our File I/O is sensitive
    // to odd (special characters) symbols.
    if (argc <= 1)
    {
        std::cout << "ERROR: Please provide a text file." << std::endl;
        return 2;
    }

    TextParser myParser("words.txt", argv[1]); // build the text parser, file I/O!
    // reporting the information about the text file!
    std::cout << "---------------------------\nINFORMATION FROM TEXT FILE\n---------------------------" << std::endl;
    std::cout << "Word Count: " << myParser.wordCount() << std::endl;          // print the word count
    std::cout << "Load factor: " << myParser.computeLoadFactor() << std::endl; // REMOVE THIS
    std::cout << "Unique words in text file, ordered from most frequent to least frequent...(maximum 40)" << std::endl;
    std::vector<WordData *> wordVector = myParser.sortedTextWords(); // return a sorted vector!
    std::cout << "Sorted vector size: " << wordVector.size() << '\n';
    // do we have 40 top words or less?
    int topCount = wordVector.size();
    if (topCount > 40)
    {
        topCount = 40;
    }
    // get the top 40 most frequent words (or less than 40, if less unique words in text file)
    for (int i = 0; i < topCount; i++)
    {
        std::cout << i + 1 << ") " << wordVector[i]->keyString() << ": occurs " << wordVector[i]->valueString() << " times." << std::endl;
    }
    std::cout << "------------------\n"
              << "END OF PROCESSING" << std::endl;

    return 0;
}