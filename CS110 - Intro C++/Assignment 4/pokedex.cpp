/*************************************************
 * Student Name:      Mohammad Al-Bayati
 * Student Number:    200339106
 * Assignment Number: CS 110 Assignment #4
 * Program Name:      pokedex.cpp
 * Last Modified:     June 17th 2021
 *
 * ------ Problem Statement -----
 * This program replicates the functionality
 * of a Pokedex. The user will be able to search
 * for a pokemon by name or number and be given
 * information about the pokemon, such as type. 
 * 
 * ------ Input and Output ------
 * // Input
 * User will be prompted to input text to
 * indicate whether they want to search by
 * name or number, and then they will input
 * that name or number. They will also be asked
 * if they want to search again. 
 *  
 * // Output
 * Prompts for the above mentioned inputs.
 * Output of pokemon name, number, type, etc. 
 *
 * ------ Main Algorithm --------
 * The program uses a filestream to iterate 
 * through a CSV one row at a time. It splits
 * these rows by commas and adds the resulting
 * elements into a 2D array. 
 * 
 * When searching by number, the program will simply
 * call that row of the array and print it. 
 * When searching by name, the program will first search
 * the array[name] column for a matching name. It will then
 * return that column number to the printing function, which
 * will look up all the columns for that index, and print. 
 *  note: Search will compare lower-cased copies of the strings,
 *  so search will not be case sensitive. 
 * 
 * -------- Assumptions ---------
 * a) There are only 801 Pokemon. 
 * b) If type 2 is "none", it's not necessary to print it. 
 * 
 * ------ Major Variables -------
 * pokedexArray: where the csv is loaded into. 
 * 
 *----- Program Limitations -----
 * a) Data is not stored in 4 separate arrays, but instead
 *    in an array of arrays. 
 * 
 *************************************************/

#include <iostream> // For console input/output
#include <fstream>  // For file stream, to open the csv
#include <string>   // For string class and substr operations

using namespace std; // We'll be using a lot of std lib stuff

// Some functions to make the main file neater
void printPokeInfo(int); // Prints information about pokemon, accepts int pokemon number.

// These functions will prompt user for input depending on what kind of search.
// Both return an int so printPokeInfo can work with both, and so looking up the
// number from the name can be dealt with within the print function, and not in
// main or another func.
int getPokemonNumByName();
int getPokemonNum();

// This function returns a lowercase version of the string inputted.
// Used it to make the search work without being case sensitive.
string lowercase(string);

// Function to prompt user if they want to search again, and return true if so.
bool userSearchAgain();

// Global variables that multiple functions might need to access.
// Put them in their own namespace for safety.
namespace pokedex
{
    // To make it easier to change if we get a new spreadsheet or more pokemon, etc.
    const int ROWS_IN_CSV = 802;
    const int COLUMNS_IN_CSV = 5;
    const string FILE_NAME = "pokemon.csv";

    // Array indices in the CSV. Didn't want these global, but also didn't want them hardcoded.
    // be sure to change these if you edit printPokeInfo or if the csv file changes.
    const int POKE_NUM = 0;
    const int POKE_NAME = 1;
    const int POKE_CLASS = 2;
    const int POKE_TYPE1 = 3;
    const int POKE_TYPE2 = 4;

    // The array we'll store the pokemon data in.
    string pokedexArray[ROWS_IN_CSV][COLUMNS_IN_CSV];
}

int main()
{
    // start the filestream and open it with file_name.
    ifstream pokedexData(pokedex::FILE_NAME);
    string searchOption; // To store user's search choice
    bool running = true; // To know whether or not to loop.

    // Checking if we couldn't load the file. If we couldn't, return 0 to end program early.
    if (!pokedexData.good())
    {
        cout << "Could not load Pokedex data!" << endl;
        return 0;
    }

    // This loop loads the contents of the filestream into pokedexArray
    // First, it gets an entire row, then it iterates through the row and
    // if it sees a comma or gets to the end, it splits string and adds the
    // chars since the last comma to the array.
    for (int i = 0; i < pokedex::ROWS_IN_CSV; i++)
    {
        // Declare a string to store the row of the csv, then get row using getline.
        string wholeRow;
        getline(pokedexData, wholeRow);

        // I used these to keep track of where we are and where we should write to.
        int index = 0;          // The index in the array to write to
        int commaLocation = -1; // Where we are in the current string
        int substrLength = 0;   // The length to cut the substring.

        // Iterate through the whole row
        for (int j = 0; j < wholeRow.length(); j++)
        {
            // If we see a comma, get a substring and place it in the array.
            // Reset the commaLocation to here (to let the next loop know where to start cutting the string)
            // Also reset substring length to zero (cause we're starting a new element) and increase
            // int index to so that we write in the next element in the array the next time.
            if (wholeRow[j] == ',')
            {
                pokedex::pokedexArray[i][index] = wholeRow.substr(commaLocation + 1, substrLength);
                commaLocation = j;
                substrLength = 0;
                index++;
            }
            // If we are at the end of the row, we basically do the same thing as the above loop
            // EXCEPT: we want to cut a substring that's 1 longer, because it would ignore last
            // character otherwise.
            else if (j == (wholeRow.length() - 1))
            {
                pokedex::pokedexArray[i][index] = wholeRow.substr(commaLocation + 1, substrLength + 1);
            }
            // If this char wasn't a ',' or the end, add to the length of the string we'll eventually cut
            else
            {
                substrLength++;
            }
        }
    }

    cout << "Welcome to the Pokedex. ";

    // Main program loop: asks user how they want to search, and searches it.
    while (running)
    {
        cout << "Would you like to search by name or by number? (q to quit)" << endl;
        cin >> searchOption;
        if (searchOption == "name")
        {
            // If user wants to search by name, we call getPokemonNumberByName, which prompts user,
            // looks up the name in the array and  returns the int index to printPokeInfo, which gets
            // the whole "row" in the array and prints it.
            printPokeInfo(getPokemonNumByName());
            // Ask user if they want to search again. if false, the while loop exits.
            running = userSearchAgain();
        }
        else if (searchOption == "number")
        {
            // getPokemonNum simply prompts user for an integer, which it returns
            // to printPokeInfo.
            printPokeInfo(getPokemonNum());
            // Ask user if they want to search again. if false, the while loop exits.
            running = userSearchAgain();
        }
        else if (searchOption == "q")
        {
            // if q entered, stop the loop
            running = false;
        }
        else
        {
            cout << "Invalid selection." << endl;
        }
    }

    // if user entered q, we thank the user and close.
    cout << "Thank you for using your Pokedex!" << endl;
    return 0;
}

int getPokemonNum() // Prompts user for valid pokemon number and returns it as int
{
    int num = 0;
    while (num < 1 || num > 801) // loops if invalid pokemon number entered
    {
        cout << "Please enter the number of the Pokemon:" << endl;
        cin >> num;

        if (num < 1 || num > 801)
        {
            cout << "I'm sorry, but there is no known Pokemon by that number." << endl;

            // clear out the stream in case we loop because an invalid number was entered
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    return num;
}

int getPokemonNumByName() // Promts user for pokemon name, finds the index, and returns int index
{
    // variables to store name and pokemon number, if we find it.
    int pokemonNumber = 0;
    string inputtedName;

    // Will loop if it doesn't find a pokemon
    while (pokemonNumber < 1 || pokemonNumber > 801)
    {
        cout << "Please enter the name of the Pokemon:" << endl;
        cin >> inputtedName;

        // loop through the name column until we find the one we're looking for
        for (int i = 1; i < pokedex::ROWS_IN_CSV; i++)
        {
            // we check the lower-cased strings so the search isn't case sensitive
            if ((lowercase(pokedex::pokedexArray[i][pokedex::POKE_NAME]) == lowercase(inputtedName)))
            {
                // this index is the pokemon number, so we'll save it and return it.
                pokemonNumber = i;
                return pokemonNumber;
            }
        }
        // We get here if we never didn't find a pokemon, so let the user know.
        // Then the while loop should loop back.
        cout << "I'm sorry, but there is no known Pokemon by that name." << endl;
    }
    // This return should never really execute.
    return pokemonNumber;
}

void printPokeInfo(int pokeNumber) // Looks up pokemon number from array and prints associated info
{
    using namespace pokedex;

    // Safety check, but isn't strictly necessary since the input functions check it too.
    if (pokeNumber > 0 && pokeNumber < 802)
    {
        cout << pokedexArray[pokeNumber][POKE_NAME]
             << ", Pokedex Number " << pokedexArray[pokeNumber][POKE_NUM]
             << ", " << pokedexArray[pokeNumber][POKE_CLASS]
             << ", type 1: " << pokedexArray[pokeNumber][POKE_TYPE1];
        // If there's a type2, we print it. otherwise no need.
        if (pokedexArray[pokeNumber][POKE_TYPE2] != "none")
        {
            cout << ", type 2: " << pokedexArray[pokeNumber][POKE_TYPE2];
        }
        cout << endl;
    }
    else
    {
        cout << "Pokemon not found." << endl;
    }
}

// Converts any strings passed to only lower-case
string lowercase(string s)
{
    // storage for the converted string
    string newStr;

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] > 64 && s[i] < 91)
        {
            newStr.push_back((s[i] + 32));
        }
        else
        {
            newStr.push_back(s[i]);
        }
    }
    return newStr;
}

// Function that asks user if they want to search again
// returns true if y, false if n.
bool userSearchAgain()
{
    string input;
    while (true)
    {
        cout << "Would you like to search for another Pokemon (y/n)?" << endl;
        cin >> input;
        if (lowercase(input) == "y" || lowercase(input) == "yes")
        {
            return true;
        }
        else if (lowercase(input) == "n" || lowercase(input) == "no")
        {
            return false;
        }
        else
        {
            cout << "Invalid selection." << endl;
            // If something unexpected was inputted,
            // clear the console and errors just in case.
            cin.ignore(1000, '\n');
            cin.clear();
        }
    }
}