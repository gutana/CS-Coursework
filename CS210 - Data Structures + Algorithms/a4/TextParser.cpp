#include "WordData.h"
#include "HashWordDictionary.h"
#include "DuplicateKeyException.h"
#include <iostream>
#include <fstream>
#include <vector>

/*
    TextParser - used to parse text, and obtain data involving it.
    Partially written by Daniel Page

    Mohammad Al-Bayati
    200339106
    CS210 - Winter 2022

*/

class TextParser
{
private:
    HashWordDictionary *words;     // a hash table for the words of the English dictionary
    HashWordDictionary *textWords; // a hash table for the words of the text file read

    template <typename T>
    void swap(T &x, T &y) // swap function to make our lives a little easier during partition
    {
        T temp = x;
        x = y;
        y = temp;
    }
    /*
        partition
        Used for QS
    */
    // Partitions wordVector, returns position of pivot
    int partition(std::vector<WordData *> &wordVector, int start, int end)
    {
        WordData *x = wordVector[end]; // pivot
        int i = start - 1;

        for (int j = start; j < end; j++)
        {
            if (wordVector[j]->getValue() >= x->getValue())
            {
                i++;
                swap(wordVector[i], wordVector[j]);
            }
        }
        // Swap elements, put pivot into place between the two groups of elements
        swap(wordVector[i + 1], wordVector[end]);
        return i + 1;
    }

    /*
        Quick Sort
        Input: An array of WordData object, integers that are start and end positions of subarray
        Output: Sorts wordArray[start...end] from largest to smallest frequencies.
    */
    void QS(std::vector<WordData *> &wordVector, int start, int end)
    {

        // you are responsible for implementing partition.  See how simple this is!!!
        if (start < end)
        {
            int pos = partition(wordVector, start, end);
            QS(wordVector, start, pos - 1);
            QS(wordVector, pos + 1, end);
        }
    }

    /*
        Quick Sort driver function
        Sorts WordData objects from largest to smallest frequencies.
    */
    void quickSort(std::vector<WordData *> &wordVector, int n)
    {
        QS(wordVector, 0, n - 1); // begins Quick Sort
    }

public:
    /*
        Constructor
    */
    TextParser(std::string wordFile, std::string textFile)
    {

        /*
            FOR YOU TO DO SPECIFY THE SIZE FOR THE words DICTIONARY!
            RIGHT NOW IT IS SET TO 100, pick an appropriate value.
        */
        words = new HashWordDictionary(77479); // 0.749997 Load Factor
        /*
             END OF PART TO CHANGE
        */
        textWords = new HashWordDictionary(20047); // we just picked this value, we are not going to be concerned about this!

        std::ifstream fileWords; // for reading the file with words
        std::ifstream fileTexts; // for reading the file with the text

        fileWords.open(wordFile);
        std::string input; // read line.
        int duplicateRecords = 0;

        while (std::getline(fileWords, input))
        {
            WordData *newWord = new WordData(input);
            try
            {
                words->put(newWord);
            }
            catch (DuplicateKeyException)
            {
                duplicateRecords++;
            }
        }

        if (duplicateRecords > 0)
        { // report to the user the number of duplicates.
            std::cout << "Warning: Upon recording " << wordFile << "... there were " << duplicateRecords << " records encountered." << std::endl;
        }

        fileWords.close();
        // next, we read in words from the text file!
        fileTexts.open(textFile);
        std::string inputText;
        // fileTexts>>inputText;

        while (!(fileTexts >> std::ws).eof())
        { // read the file until we have nothing left
            fileTexts >> inputText;
            for (int i = 0; i < inputText.size(); i++)
            { // make the input letters all uppercase
                inputText.at(i) = toupper(inputText.at(i));
            }
            std::string longest = findLongestWord(inputText);
            try
            {
                if (longest.size() > 0)
                { // don't put empty strings in
                    textWords->put(new WordData(longest));
                }
            }
            catch (DuplicateKeyException)
            {
                // do nothing
            }
        }
        fileTexts.close();
    }

    // deconstructor, you do not need to do anything here.
    ~TextParser()
    {
        delete words;
        delete textWords;
    }

    /*
        Find the longest string that provided is in the word dictionary, return it!
    */
    std::string findLongestWord(std::string word)
    {

        std::string longest = ""; // empty string to begin with

        for (int i = 0; i < word.length(); i++)
        {
            for (int j = 1; j < word.length() + 1; j++)
            {
                std::string subString = word.substr(i, j);
                if (words->get(subString) && (subString.length() > longest.length()))
                // If word exists and is longer than current longest, replace it
                {
                    longest = subString;
                }
            }
        }
        return longest;
    }

    // Return a sorted array (vector), sorted most frequent word to least.
    std::vector<WordData *> sortedTextWords()
    {

        // obtain all the records
        std::vector<WordData *> wordVector = textWords->listRecords();
        // sort the vector (do not modify this code)
        quickSort(wordVector, textWords->size());

        return wordVector;
    }

    // Return the word count of the text file (total count of words)
    int wordCount()
    {
        int numWords = 0;
        // obtain all the records
        std::vector<WordData *> wordVector = textWords->listRecords();
        // Use the vector count up how many words there are (by counting the frequencies of words)
        for (int i = 0; i < wordVector.size(); i++)
        {
            numWords += wordVector[i]->getValue(); // add frequency of each word to numWords
        }

        return numWords;
    }

    // compute load factor
    double computeLoadFactor()
    {
        return ((double)words->size()) / ((double)words->M());
    }
};
