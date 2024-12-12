#include <fstream>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>

using namespace std;

// The following two lines come from HALglobals.h
const int MAX_COMMAND_LINE_ARGUMENTS = 8;
const int SLEEP_DELAY = 100000;

int getCommand(string tokens[]);
int tokenizeCommandLine(string tokens[], string commandLine);
bool checkForCommand();
int processCommand(string tokens[], int tokenCount);

char ** convertToC (string tokens [], int tokenCount);
void cleanUpCArray (char ** cTokens, int tokenCount);
void printReverse (char ** cTokens, int tokenCount);


static volatile sig_atomic_t cullProcess = 0;