
// grep -i -w sed colorFile.txt > sed.txt

/* Please write your code for the lab exercise in this file */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 50

int main()
{   
    char* grepArgs[] = {(char*)"grep",
                        (char*)"-o", 
                        (char*)"-i", 
                        (char*)"pipe", 
                        (char*)"colorFile.txt", NULL};

    char* wcArgs[] = {(char*)"wc",
                      (char*)"-w", NULL};

    int fd_pair[2];
    
    if (pipe(fd_pair) == -1)
    {
        perror("Pipe");
        exit(2);
    }
    
    switch(fork())
    {
        case -1:
            perror("Fork");
            exit(3);
        case 0: // CHILD
            // printf("Child\n");

            dup2(fd_pair[1], fileno(stdout));

            close(fd_pair[0]);
            close(fd_pair[1]);

            execvp(grepArgs[0], grepArgs);

            break;
        default: // PARENT
            // printf("Parent\n");
            dup2(fd_pair[0], fileno(stdin));

            close(fd_pair[0]);
            close(fd_pair[1]);

            int outFile = creat("newFile.txt", S_IRUSR);
            if (outFile == -1)
            {
                perror("creating file");
            }

            dup2(outFile, fileno(stdout));
            execvp(wcArgs[0], wcArgs);
    }

}
