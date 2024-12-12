#include <stdio.h>
//#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  int i;

  for (i = 0;; ++i)
  {
    printf("%i\n", i);
    sleep(1);
  }
}
