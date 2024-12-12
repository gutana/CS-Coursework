// 1. term: SIGALRM
//    core: SIGBUS
//    stop: SIGTTIN
//  ignore: SIGCHLD

// 2. SIGKILL, SIGSTOP

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>

static void sigHandler(int signalNumber, siginfo_t *signalInfo, void *context)
{
    printf("Signal Receieved %d\n", signalNumber);
    if (signalNumber == 28)
    {
        printf("Child receieved signal\n");
    }
    else if (signalNumber == 17)
    {
        printf("Parent receieved signal\n");
    }
    else
    {
        printf("unknown signal.\n");
    }
    exit(0);
}

int main()
{
    pid_t childId;
    if (childId = fork()) // parent
    {
        // overwrite default action of SIGCHLD
        sigset_t interruptMask;
        struct sigaction signalAction;

        sigemptyset(&interruptMask);
        sigaddset(&interruptMask, SIGCHLD);

        signalAction.sa_sigaction = &sigHandler;
        signalAction.sa_mask = interruptMask;
        signalAction.sa_flags = SA_SIGINFO;

        if (sigaction(SIGCHLD, &signalAction, NULL) == -1)
        {
            perror("sigaction cannot set to SIGCHLD");
        }

        // nanosleep 500000ns
        nanosleep((const struct timespec[]){{0, 500000L}}, NULL);

        // send SIGWINCH to child.
        kill(childId, SIGWINCH);

        // loop infinitely, saying "parent waiting" (5000L nanosleep)
        while (true)
        {
            printf("parent waiting\n");
            nanosleep((const struct timespec[]){{0, 5000L}}, NULL);
        }
    }
    else // child
    {
        // overwrite default action of SIGWINCH
        sigset_t interruptMask;
        struct sigaction signalAction;

        sigemptyset(&interruptMask);
        sigaddset(&interruptMask, SIGWINCH);

        signalAction.sa_sigaction = &sigHandler;
        signalAction.sa_mask = interruptMask;
        signalAction.sa_flags = SA_SIGINFO;

        if (sigaction(SIGWINCH, &signalAction, NULL) == -1)
        {
            perror("sigaction cannot set to SIGWINCH");
        }

        // loop infinitely and print a message "Child waiting" (add nanosleep for 5000L)
        while (true)
        {
            printf("child waiting\n");
            nanosleep((const struct timespec[]){{0, 5000L}}, NULL);
        }
    }
}