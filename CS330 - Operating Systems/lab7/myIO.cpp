#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>  
#include <sys/stat.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>


void Parent();
void Child();

std::string fileName;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Please include arguments\n");
        exit(1);
    }

    fileName = argv[1];

    if (fork())
    {
        Parent();
    }
    else 
    {
        Child();
    }
}

void Parent() // parent does read_write stuff. (parent code is in there)
{
    wait(NULL);

    // open the file for reading
    // "outFile" is slightly misleading: parent only reads it. but it was an output of the child.... 
    int outFile = open(fileName.c_str(), O_RDONLY);

    // check for errors on opening, use perror 
    if (outFile == -1)
    {
        perror("Unable to open file in parent process");
        exit(1);
    }

    // call fstat() and print info about file the child created
    struct stat fileStats;
    if (fstat(outFile, &fileStats) == 0)
    {
        struct passwd* passwdEntry = getpwuid(fileStats.st_uid);
        struct group* groupInfo = getgrgid(fileStats.st_gid);

        char* timeText = ctime(&fileStats.st_mtime);
        char* groupNameText = groupInfo->gr_name;

        printf("\nThis is the status of %s:\n", fileName.c_str());
        printf("%-12s %-10s %-10s %-10s\n", "File Size", "Owner", "Group ID", "Last Modified");
        printf("%-12li %-10s %-10s %-10s\n", fileStats.st_size, passwdEntry->pw_name, groupNameText, timeText);

    }
    else 
    {
        write(fileno(stdin), "Error getting file stats!\n", 26);
    }

    // write a message that you are in the parent
    printf("I am in the parent!\n");

    int n_char; 
    char* buffer[16];
    // read from the file (until eof)
    while ((n_char = read(outFile, buffer, 16)) != 0)
    {
        // Display the characters read
        n_char = write(fileno(stdout), buffer, n_char);
    }

    // close the file
    close(outFile);

}
void Child() // child does read_writeBad stuff (Child code is in there)
{
    // open the file for writing (use 'create' flag and permissions for user to read/write)
    int outFile = open(fileName.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 00666);

    if (outFile == -1)
    {
        perror("Unable to open file in child process");
        exit(1);
    }

    write(1, "Enter text and CTRL-D to end:\n", 30);

    // read from standard input until eof (ctrl + d)
    int n_char = 0;
    char* inputBuffer[16];
    while ((n_char = read(fileno(stdin), inputBuffer, 10)) != 0)
    {
        if (n_char == -1)
        {
            perror("Error trying to read keyboard input");
            exit(4);
        }
        // write to file
        n_char = write(outFile, inputBuffer, n_char);
    }

    // close the file
    close(outFile);
}