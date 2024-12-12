#include "HALmod.h"
// 1. 
//     a. based on spaces (" ")
//     b. Can easily copy strings because of overloaded operator=, and 
//        memory is automatically cleaned up for us when string goes out of scope. 
            

int main()
{
  string tokens[MAX_COMMAND_LINE_ARGUMENTS];
  int tokenCount;

  do
  {
    tokenCount = getCommand(tokens);
    char ** cstr_tokens = convertToC(tokens, tokenCount);

    printReverse(cstr_tokens, tokenCount);

    cleanUpCArray(cstr_tokens, tokenCount);
    
  } while (processCommand(tokens, tokenCount));

  return 0;
}
