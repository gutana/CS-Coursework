
// PART 1
// a. No. It only prints when there is an error, because we are using perror();
// b. The terminal that started it. 
// c. I would remove the wait() call. 
//
// PART 2
// Because although mynum is a global variable, it is not shared between two processes. 
// At the fork, both start with mynum='0'. The child updates its copy of the variable, but the parent doesn't. 

#include "HALmod.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int getCommand(string tokens[])
{
  string commandLine;
  bool commandEntered;
  int tokenCount;

  do
  {
    cout << "HALshell> ";
    while (1)
    {
      getline(cin, commandLine);
      commandEntered = checkForCommand();
      if (commandEntered)
      {
        break;
      }
    }
  } while (commandLine.length() == 0);

  tokenCount = tokenizeCommandLine(tokens, commandLine);

  return tokenCount;
}

int tokenizeCommandLine(string tokens[], string commandLine)
{
  char *token[MAX_COMMAND_LINE_ARGUMENTS];
  char *workCommandLine = new char[commandLine.length() + 1];
  int i;
  int tokenCount;

  for (i = 0; i < MAX_COMMAND_LINE_ARGUMENTS; i++)
  {
    tokens[i] = "";
  }
  strcpy(workCommandLine, commandLine.c_str());
  i = 0;
  if ((token[i] = strtok(workCommandLine, " ")) != NULL)
  {
    i++;
    while ((token[i] = strtok(NULL, " ")) != NULL)
    {
      i++;
    }
  }
  tokenCount = i;

  for (i = 0; i < tokenCount; i++)
  {
    tokens[i] = token[i];
  }

  delete[] workCommandLine;

  return tokenCount;
}

// Do not touch the below function
bool checkForCommand()
{
  if (cullProcess)
  {
    cullProcess = 0;
    cin.clear();
    cout << "\b\b  \b\b";
    return false;
  }

  return true;
}

// This is a very paired down version of Dr. Hilderman's function
int processCommand(string tokens[], int tokenCount)
{
  if (tokens[0] == "shutdown" || tokens[0] == "restart" || tokens[0] == "lo")
  {
    if (tokenCount > 1)
    {
      cout << "HALshell: " << tokens[0] << " does not require any arguments"
           << endl;
      return 1;
    }
    cout << endl;
    cout << "HALshell: terminating ..." << endl;

    return 0;
  }
  else
  {
    pid_t forkResult = fork(); 
    int forkStatus; 


    switch(forkResult)
    {
      case -1: // error
      {
        perror("Error forking...");
        break;
      }
      case 0: // child
      {
        // printf("Child!\n");
        // call ConvertToC, capture and return into a char** variable
        char** c_strs = convertToC(tokens, tokenCount);

        // call execvp, sending first elem of char** and the entire array
        // printf("Child executing %s\n", c_strs[0]);
        execvp(c_strs[0], c_strs);

        // print error message using perror
        perror("error executing command");

        exit(1);
        break;
      }

      default: // parent (we have process id)
      {
        // printf("Parent!\n");

        wait(&forkStatus);

        // printf("Waited for child.\n");

        // wait for child
        // print child's return value using WIFEXITEd and WEXITSTATUS
        if (WIFEXITED(forkStatus))
          printf("Child returned: %i\n", WEXITSTATUS(forkStatus));
        else 
          printf("Did not successfully get return value from child\n");

        return 1; 
        break; 
      }
    }
    return 1;
  }
}
char **convertToC(string tokens[], int tokenCount)
{

  // alloc an extra "word" pointer for NULL (hint: tokencount+1)
  char **words = (char **) malloc((sizeof(char*) * tokenCount) + (1 * sizeof(char*))); // room for null terminator

  for (int i = 0; i < tokenCount; i++)
  {
    words[i] = strdup(tokens[i].c_str());
  }

  // store a value of null into the extra "word"
  words[tokenCount] = NULL;

  return words;
}
