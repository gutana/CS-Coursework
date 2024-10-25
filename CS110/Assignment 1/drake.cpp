/**********************************************************************************
 * Student Name: Mohammad Al-Bayati
 * Student Number: 200339106
 * Assignment Number: CS 110 Assignment #1
 * Program Name: drake.cpp
 * Last Modified: May 21st 2021
 *
 * ----------------------------- Problem Statement --------------------------------
 * This program implements the Drake equation (N = R∗ * fp * ne * f1 * fi *fc * L),
 * an equation to calculate the number of intelligent civilizations that may 
 * potentially communicate with us. 
 * 
 * The program assumes some values will be constant, and promps the user
 * for input on the remaining variables. The program will then return the calculated
 * number of intelligent civilizations that could communicate with us. 
 * 
 * ------------------------------ Input and Output --------------------------------
 * Input:
 *  - The fraction of stars that have planets (fp), stored as float starsWithPlants
 *  - The number of life-supporting planets per planet bearing star (ne), stored as 
 *    float lifeSupportingPlanetsPerStar
 *  - The fraction of civilizations that release detectable signals (fc), stored as 
 *    float CivsReleasingSignals. 
 *  - The number of years that a civilization releases detectable signals (L), 
 *    stored as float yearsSendingSignals. 
 * 
 * Output:
 *  - An introduction statement in the console. 
 *  - Prompts for the inputs above. 
 *  - The calculated number of intelligent civilizations in the galaxy. 
 *
 * -------------------------------- Main Algorithm ---------------------------------
 * 1- Prompt user and read in the variables from the console
 * 2- Compute the number of intelligent civilizations in the galaxy (Drake Equation)
 * 3- Display the number to the user

 * ---------------------------------- Assumptions ----------------------------------
 * a) Star formation rate is 1. (R∗)
 * b) Fraction of habitable planets that develop life is 1. (fl)
 * c) Fraction of planets with life that develop civilizations is 1. (fi) 
 *
 * ------------------------------- Major Variables ---------------------------------
 * communicatingCivs --> The main output, the calculation of the Drake equation. 
 * starsWithPlanets  --> The fraction of stars with planets, expected as input. 
 * lifeSupportingPlanetsPerStar --> The number of life supporting planets per planet-
 *     bearing star.
 * civsReleasingSignals --> Fraction of civilizations that release signals into space.  
 * yearsSendingSignals  --> The number of years that a civilization releases those signals. 
 * 
 * ----------------------------- Program Limitations -------------------------------
 * a) the program largely uses floats, limiting it to 6 digits
 *    of precision, which is acceptable given the approximate
 *    nature of the Drake equation. 
 * b) liberal estimates may come out as scientific notation.
 *    we can cast the output to (int) if we want to see all
 *    the 0's. 
***********************************************************************************/

#include <iostream>
using namespace std;

int main()
{
    // VARIABLES
    double communicatingCivs;           // Number of Intel civs in galaxy that could communicate with us
    float starsWithPlanets;             // Fraction of stars with planets
    float lifeSupportingPlanetsPerStar; // Life supporting planets per planet bearing star
    float civsReleasingSignals;         // those that release detectable signals into space
    float yearsSendingSignals;          // Years that an intelligent civ releases detectable signals into space.

    // CONSTANTS. They're all equal to 1, so we don't technically need to do anything with them to get the correct result, but used
    // and stored in separate variables for clarity and tweakability. Can be changed to float/double if decimals are needed.
    const int starFormationRate = 1;
    const int planetsThatDevLife = 1;
    const int planetsThatDevCivs = 1;

    // Printing a simple introductory message to the user.
    cout << "**************************************** Welcome to the magic Drake equation solver! ****************************************" << endl; // Stars may be misaligned if your console window is small.
    cout << "This program will calculate the number of intelligent civilizations in the galaxy that could potentially communicate with us." << endl
         << endl; // Another new line to leave a little space before we start prompting user. Just to make it nicer to look at.

    // Asking user for inputs for all the variables except the one we are calculating
    cout << "Please enter the fraction of stars that have planets: ";
    cin >> starsWithPlanets;

    cout << "Please enter the number of life supporting planets per planet-bearing star: ";
    cin >> lifeSupportingPlanetsPerStar;

    cout << "Please enter the fraction of civilizations that release detectable signals into space: ";
    cin >> civsReleasingSignals;

    cout << "Please enter the number of years that a civilization releases signals for: ";
    cin >> yearsSendingSignals;

    // MAIN FORMULA: The magic happens here (Drake equation is implemented)
    communicatingCivs = starFormationRate * starsWithPlanets * lifeSupportingPlanetsPerStar * planetsThatDevLife * planetsThatDevCivs * civsReleasingSignals * yearsSendingSignals;

    // Prints out the calculated number of intelligent civilizations.
    cout << endl // extra line break to make it less like a wall of text.
         << "The number of intelligent civiliations is: " << communicatingCivs << endl;

    return 0; // End program successfully
}