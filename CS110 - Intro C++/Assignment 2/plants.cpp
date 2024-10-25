/*************************************************
 * Student Name:   Mohammad Al-Bayati
 * Student Number: 200339106
 * Assignment Number: CS 110 Assignment #2
 * Program Name: plants.cpp
 * Last Modified: June 1st 2021
 *
 * ------ Problem Statement -----
 * This program should prompt the user to select 
 * a plant from a list of options, take a temp
 * from the user, and tell the user whether or 
 * not the temperature is suitable for the plant. 
 * 
 * ------ Input and Output ------
 * Input:
 * - A number (1-3) representing the choice of plant (stored as int usersPlant)
 * - The user's temperature (stored as float usersTemp)
 *  
 * Output:
 * - Prompts for plant selection and temperature.
 *
 * ------ Main Algorithm --------
 * 1- Display list of plants and take selection from user
 * 2- Take the temperature from the user
 * 3- Advise the user regarding the suitability of that temperature
 *    for that plant by comparing it to known optimal temperatures. 

 * -------- Assumptions ---------
 * a) Program only needs to be run once.
 *    - If program needs to loop, could be done with minor modifications 
 *      and a do while loop where the user can enter -1 as plant to exit 
 *      the program.  
 * 
 * ------ Major Variables -------
 * usersPlant --> The users choice of plant. 
 * usersTemp ---> The users temperature. 
 * Constants ---> Plant choices and temperature ranges
 * 
 *----- Program Limitations -----
 *
 * a) the program doesn't expect decimal inputs
 *    e.g. input 1.5 will be truncated and read as Fern. 
 * b) the program doesn't expect weird values for temperature,
 *    such as strings. Result will be too cold. 
 *************************************************/

#include <iostream>  // For outputting and getting input from console
using namespace std; // Save us some typing as we'll be using std functions (iostream)

int main()
{
    // Constants for plant selection
    const int indoorFern = 1;
    const int pothos = 2;
    const int kale = 3;

    // Constants for temperature ranges
    const int fernMin = 60;
    const int fernMax = 75;

    const int pothMin = 70;
    const int pothMax = 90;

    const int kaleMin = 55;
    const int kaleMax = 75;

    // User inputted variables
    int usersPlant;
    float usersTemp; // Used float to accomodate decimals

    // Print intro statement
    cout << "*************** Welcome to the Optimal Growing Temperature checker  ***************" << endl
         << "This program will help you ensure your plant's temperature supports optimal growth." << endl
         << endl;

    // Prompt user for plant selection
    cout << indoorFern << ". Indoor Fern" << endl
         << pothos << ". Pothos" << endl
         << kale << ". Kale" << endl
         << endl
         << "Please Select Your Plant: ";

    // Get user's plant type
    // (expecting an integer whose value is the same as indoorFern, pothos, or kale)
    cin >> usersPlant;

    // If user selected a valid option, get temperature (F) from user
    // Probably could've just done two checks: if usersPlant < 1 and if usersPlant > 3
    // but this way is more human readible, and this application is not performance critical.
    if (usersPlant == indoorFern || usersPlant == pothos || usersPlant == kale)
    {
        cout << "Please input your growing temperature (in degrees F): ";
        cin >> usersTemp;
        cout << endl; // Extra line break to make output more readible
    }
    // Decide if the temperature fits the plant and tell the user
    // if it's just right, too cold, or too warm.
    // Will print "Invalid plant selection" if user puts in something other than
    // the expected variables.
    switch (usersPlant) // We switch based on the plant, then use if/else logic based on temp
    {
    case indoorFern:
        if (usersTemp > fernMax)
        {
            cout << "That's too warm!" << endl;
        }
        else if (usersTemp >= fernMin)
        {
            cout << "That's just right!" << endl;
        }
        else
        {
            cout << "That's too cold!" << endl;
        }
        break;
    case pothos:
        if (usersTemp > pothMax)
        {
            cout << "That's too warm!" << endl;
        }
        else if (usersTemp >= pothMin)
        {
            cout << "That's just right!" << endl;
        }
        else
        {
            cout << "That's too cold!" << endl;
        }
        break;
    case kale:
        if (usersTemp > kaleMax)
        {
            cout << "That's too warm!" << endl;
        }
        else if (usersTemp >= kaleMin)
        {
            cout << "That's just right!" << endl;
        }
        else
        {
            cout << "That's too cold!" << endl;
        }
        break;
    default: // for when user puts in an invalid choice, such as 37654 or pdsfgkjsl.
        cout << "Invalid plant selection!" << endl;
        break;
    }
    return 0;
}