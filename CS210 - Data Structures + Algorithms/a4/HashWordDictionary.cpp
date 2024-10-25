#include "HashWordDictionary.h"
#include "DuplicateKeyException.h"

/*
    Mohammad Al-Bayati
    200339106
    CS210 - Winter 2022
*/

HashWordDictionary::HashWordDictionary() // default constructor
    : numElem(0)                         // 0 elements to begin with
{
    table = new WordNode *[tableSize];

    for (int i = 0; i < tableSize; i++)
    {
        table[i] = nullptr; // Make the whole table nullptr to begin with
    }
}

HashWordDictionary::HashWordDictionary(int M) // size of the table
    : numElem(0), tableSize(M)                // 0 elements, M table size
{
    table = new WordNode *[tableSize];

    for (int i = 0; i < tableSize; i++)
    {
        table[i] = nullptr; // Make whole table nullptr to begin with
    }
}

HashWordDictionary::~HashWordDictionary() // destructor
{
    for (int i = 0; i < tableSize; i++) // Go through entire table
    {
        if (table[i] != nullptr) // If table entry isn't null
        {
            WordNode *nodeToDelete = table[i];            // Node we want to delete
            WordNode *nextNode = nodeToDelete->getNext(); // Store location of next node, if there is one
            while (nodeToDelete != nullptr)
            {
                delete nodeToDelete;
                nodeToDelete = nextNode; // Go next
                if (nextNode)            // Move nextNode to the one after, if it isn't already nullptr.
                    nextNode = nextNode->getNext();
            }
        }
    }
    delete[] table;
}

void HashWordDictionary::put(WordData *word) // put record (key, data) into dictionary, do not put in duplicates.
{
    int index = hashFunction(word->getKey()); // Get the index for the word

    if (index == -1) // Early return for empty string
        return;

    if (table[index] == nullptr) // If the table entry is empty, just add it and increment numElem
    {
        table[index] = new WordNode(word);
        numElem++;
        return;
    }

    // If there's already something at that table entry, we need to traverse the linked list
    WordNode *node = table[index]; // start at the first node at that index
    WordNode *prevNode = nullptr;  // To keep track of the previous node

    while (node != nullptr) // Loop until we get to the end of the linked list
    {
        if (node->getData()->getKey() == word->getKey()) // If the entry already exists in the linked list
        {
            int count = node->getData()->getValue(); // Get the count (freq) so we can increment it
            node->getData()->setValue(++count);      // set the incremented count
            throw DuplicateKeyException();           // early return if val already exists
        }
        prevNode = node;
        node = node->getNext(); // go next node
    }
    prevNode->setNext(new WordNode(word)); // We only get here if we're at the end of the list and no duplicates
    numElem++;
}

WordData *HashWordDictionary::get(std::string key) // Return the value of the record with key.
{
    int index = hashFunction(key); // Get key

    if (index == -1) // hash function returns -1 for an empty string
        return nullptr;

    WordNode *node = table[index]; // Start at that table entry

    while (node != nullptr) // Traverse the linked list
    {
        if (node->getData()->getKey() == key) // If we find what we're looking for, return it
        {
            return node->getData();
        }
        node = node->getNext(); // If not found, go to next elemenet
    }

    return nullptr; // if we didn't find the key
}

int HashWordDictionary::size() // how many elements are in the dictionary?
{
    return numElem;
}

int HashWordDictionary::M() // what is the table size?
{
    return tableSize;
}

bool HashWordDictionary::isEmpty() // is the dictionary empty?
{
    return (numElem > 0 ? false : true);
}

std::vector<WordData *> HashWordDictionary::listRecords() // returns a vector with all the records!
{
    std::vector<WordData *> v;

    for (int i = 0; i < tableSize; i++)
    {
        WordNode *node = table[i];
        while (node != nullptr)
        {
            // std::cout << "listRecords pushing " << node->getData() << " into vector.\n";
            v.push_back(node->getData());
            node = node->getNext();
        }
    }
    return v;
}

int HashWordDictionary::hashFunction(std::string input)
{
    if (input.empty())
        return -1;

    // Hash function, using Horner's method
    int val = (int)input[0]; // Get each char as an int

    for (int i = 1; i < input.size(); i++)
    {
        val = (val * 37 + (int)input[i]) % tableSize; // Fancy math stuff, % tableSize to stay in bounds.
    }
    return val; // Return the hashed index
}