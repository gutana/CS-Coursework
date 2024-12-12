#include "HALmod.h"

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
    return 1;
}

char ** convertToC (string tokens [], int tokenCount)
{
    char** cstr_tokens = new char*[tokenCount];
    for (int i = 0; i < tokenCount; i++)
    {
        char* cstr_token = new char[tokens[i].length() + 1];
        strcpy(cstr_token, tokens[i].c_str());
        cstr_tokens[i] = cstr_token;
    }
    return cstr_tokens;
}

void cleanUpCArray (char ** cTokens, int tokenCount)
{
    for (int i = 0; i < tokenCount; i++)
    {
        delete[] cTokens[i];
    }
    delete[] cTokens;
}

void printReverse (char ** cTokens, int tokenCount)
{
    printf("The words in reverse are: ");
    for (int i = tokenCount - 1; i >= 0; i--)
    {
        printf("%s ", cTokens[i]);
    }
    printf("\n");
}