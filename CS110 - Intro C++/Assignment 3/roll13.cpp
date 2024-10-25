/*************************************************
 * Student Name:      Mohammad Al-Bayati
 * Student Number:    200339106
 * Assignment Number: CS 110 Assignment #3
 * Program Name:      roll13.cpp
 * Last Modified:     June 8th 2021
 *
 * ------ Problem Statement -----
 * This program replicates the functionality of 
 * the "BIG SHINY MEGACORP" game ROLL13, a game
 * where the user and dealer role dice and try to
 * get as close to 13 as they can without going over
 * by rolling dice. 
 * 
 * ------ Input and Output ------
 * Input:
 * - User will be promted to roll or stay (r/s) 
 *  
 * Output:
 * - An Introductory statement, which includes rules
 * - The results of a roll and an updated total for user and dealer
 * - Text output such as "You bust!" or "Dealer busts!"
 *
 * ------ Main Algorithm --------
 * 1 - Generates a random roll between 1-6 to add to the 
 *     user or dealer's total.
 * 2 - Various text outputs to the console to inform the player
 *     of the state of the game. 

 * -------- Assumptions ---------
 * a) The program should function as similarly as practically
 *    possible, while removing unintended bugs found in the 
 *    original ROLL13, such as instances where dealer would not
 *    bust despite being over 13 until user inputted 's'. 
 * 
 * ------ Major Variables -------
 * dealerTotal/userTotal --> The current total of the user or dealer. 
 * gameState --> Can be 0, 1, or 2 to help keep track of what point in the game we are. 
 *     - This was chosen because the first run needed to be slightly different:
 *          - 3 states: first roll (where dealer goes twice), main loop, and game over. 
 *     - i.e. dealer goes first and both roll twice on the first roll. 
 * 
 *----- Program Limitations -----
 * a) The program only accepts 'r' or 's' as input, and
 *    capital 'R' or 'S' will not work. This is the original limited functionality
 *    of the official ROLL13, but capital letters can be added easily. 
 * 
 *************************************************/

#include <iostream> // For console input/output
#include <cstdlib>  // We'll be using rand, from the std lib
#include <ctime>    // We'll be using time to seed the rand.

using namespace std; // Standard namespace, which we'll be using quite often.

void printIntro(const int); // Prints intro and rules. Bulky text so i made it it's own function to get it out of the way.
                            // Accepts an integer for the max so it can easily be changed to any # just by changing
                            // the constant int GOAL to get something like roll 21 or roll 2053794.
int rollRandom(const int);  // Returns random integer up to the value given as argument. Assumes min is 1.

int dealerRolls(int roll, int prevTotal); // Takes in the result of a roll and the previous total. Will print the results to console and return the new total.

int userRolls(int roll, int prevTotal); // Takes in the result of a roll and the previous total. Will print the results to console and return the new total.

int main()
{
    // Variables and constants
    const int MAX_ROLL = 6; // The maximum that can be rolled from a single die
    const int GOAL = 13;    // The number that the user is trying to get close to

    // Using these to keep track of where we are in the game loop, since first round is slightly different
    const int GAME_FIRST_ROLL = 0;
    const int GAME_LOOP = 1;
    const int GAME_OVER = 2;

    // Initialize the gamestate to first roll, set user/dealer totals to 0.
    int gameState = GAME_FIRST_ROLL;
    int dealerTotal = 0;
    int userTotal = 0;

    // Seed rng for dice rolling
    srand(time(0));

    // Print intro message and rules.
    printIntro(GOAL);

    // Main game loop
    do
    {
        // If it's first round, dealer goes first and both roll twice. Then set gameState to not first round.
        if (gameState == GAME_FIRST_ROLL)
        {
            cout << "--------------" << endl;
            cout << "Dealer rolls two dice..." << endl;
            dealerTotal = dealerRolls(rollRandom(MAX_ROLL), dealerTotal);
            cout << "--------------" << endl;
            dealerTotal = dealerRolls(rollRandom(MAX_ROLL), dealerTotal);

            cout << "--------------" << endl;
            cout << "You roll two dice..." << endl;
            userTotal = userRolls(rollRandom(MAX_ROLL), userTotal);
            cout << "--------------" << endl;
            userTotal = userRolls(rollRandom(MAX_ROLL), userTotal);
            gameState = GAME_LOOP;
        }

        else // If not first round, only roll once and user goes first, gets prompted to roll/stay.
        {
            char choice; // User's choice of stay or roll.
            cout << "--------------" << endl;
            do // This do loop only loops back if user inputs something incorrect.
            {
                // Print scores, ask user what they want to do.
                cout << "You: " << userTotal << " Dealer: " << dealerTotal << endl;
                cout << "Roll (r) or stay (s)?" << endl;
                cin >> choice;
                if (choice != 'r' && choice != 's') // Handling improper inputs
                {
                    cout << "That's not a valid entry, try again!" << endl;
                    // Clearing out the rest of the inputs and errors, in case user inputted many characters or invalid characters.
                    cin.ignore(10000, '\n');
                    cin.clear();
                }
                else if (choice == 'r') // if user selects r, both roll.
                {
                    userTotal = userRolls(rollRandom(MAX_ROLL), userTotal);
                    cout << "--------------" << endl;
                    dealerTotal = dealerRolls(rollRandom(MAX_ROLL), dealerTotal);
                }
                else if (choice == 's') // If they stay, we print user's total and change the gameState to stop loop. dealer will roll one last time
                {
                    cout << "--------------" << endl;
                    cout << "Your total is: " << userTotal << endl;
                    cout << "Dealer will roll one more time..." << endl;
                    dealerTotal = dealerRolls(rollRandom(MAX_ROLL), dealerTotal);
                    gameState = GAME_OVER;
                }
                // Checking if somebody busted, printing required message and ending game if so.
                if (userTotal > GOAL && dealerTotal > GOAL)
                {
                    cout << "You and dealer both bust!" << endl;
                    gameState = GAME_OVER;
                }
                else if (userTotal > GOAL)
                {
                    cout << "Oops... you bust!" << endl;
                    gameState = GAME_OVER;
                }
                else if (dealerTotal > GOAL)
                {
                    cout << "Dealer busts!" << endl;
                    gameState = GAME_OVER;
                }

            } while (choice != 'r' && choice != 's');
        }
    } while (gameState != GAME_OVER);

    // We get here once gamestate == GAME_OVER. Print final scores.
    cout << "--------------" << endl
         << "Final Score: You: " << userTotal << " Dealer: " << dealerTotal << endl;

    // Various if/else statements checking who busted/won and print it.
    if (userTotal == dealerTotal) // Tie
    {
        cout << "You tie! Nobody wins!" << endl;
    }
    else if (userTotal > GOAL && dealerTotal > GOAL) // Everybody Busts
    {
        cout << "You bust! Dealer busts! Nobody wins!" << endl;
    }
    else if (userTotal > GOAL && dealerTotal <= GOAL) // User busts
    {
        cout << "You bust! Dealer wins!" << endl;
    }
    else if (userTotal <= GOAL && userTotal > dealerTotal) // User has more
    {
        cout << "You win!" << endl;
    }
    else if (userTotal <= GOAL && dealerTotal > GOAL) // Dealer busts
    {
        cout << "Dealer busts! You win!" << endl;
    }
    else if (dealerTotal <= GOAL && dealerTotal > userTotal) // Dealer has more
    {
        cout << "Dealer wins!" << endl;
    }

    // Kindly thank the player for playing, and exit successfully.
    cout << "Thanks for playing!" << endl;
    return 0;
}

/********************************* FUNCTION DEFINITIONS ****************************************/

void printIntro(const int GOAL) // Prints intro statement with const GOAL, in case we wanted to make a ROLL21 or ROLL2342.
{
    cout << "+-------------------------+" << endl
         << "   WELCOME TO OPENROLL" << GOAL << endl
         << "+-------------------------+" << endl
         << endl;

    cout << "Rules:" << endl
         << "Get as close to " << GOAL << " as possible without going over." << endl
         << endl
         << "Both dealer and player start by rolling two die." << endl
         << "You may choose to roll or stay each turn." << endl
         << endl;

    cout << "If you go over, you lose and cannot roll again." << endl
         << "If the dealer goes over and you don't, you win!" << endl
         << "If you both go over, you both lose." << endl
         << "If you tie, nobody wins." << endl
         << endl;

    cout << "If you choose to stay, the dealer will roll one more time even if the dealer already bust." << endl
         << endl;
}

int rollRandom(const int MAX) // Returns random int up to the MAX provided.
{
    return rand() % MAX + 1;
}

int dealerRolls(int roll, int prevTotal) // Calculate total, print results, return total.
{
    int total = prevTotal + roll;
    cout << "Dealer rolls a " << roll << endl;
    cout << "Dealer total is " << total << endl;
    return total;
}

int userRolls(int roll, int prevTotal) // Calculate total, print results, return total.
{
    int total = prevTotal + roll;
    cout << "You roll a " << roll << endl;
    cout << "Your total is " << total << endl;
    return total;
}
