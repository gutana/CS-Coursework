
/* set CTRL-C and CTRL-\ to be trapped by a function called signal_catcher */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void yourSignalHandler(int signalNo, siginfo_t *info, void *context);

int main(void)
{
  int i;

  // Variables needed for signals
  sigset_t interruptMask;
  struct sigaction act;

  // Set the stage
  sigemptyset(&interruptMask);
  sigaddset(&interruptMask, SIGINT);
  sigaddset(&interruptMask, SIGQUIT);

  // Hook up the signalhandler to SIGINT and SIGQUIT
  act.sa_sigaction = &yourSignalHandler;
  act.sa_mask = interruptMask;
  act.sa_flags = SA_SIGINFO;

  if (sigaction(SIGINT, &act, NULL) == -1)
  {
    perror("Sigset cannot set SIGINT");
    exit(SIGINT);
  }
  if (sigaction(SIGQUIT, &act, NULL) == -1)
  {
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }
  for (i = 0;; ++i)
  {
    printf("%i\n", i);
    sleep(1);
  }
}
static void yourSignalHandler(int signalNo, siginfo_t *info, void *context)
{
  printf("\nSignal %d received. \n", signalNo);
  if (signalNo == SIGQUIT)
    exit(1);
}
