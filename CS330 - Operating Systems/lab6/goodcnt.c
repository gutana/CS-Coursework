#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NITER 1000000

int CNT = 0;
sem_t semCNT; 

void *counterFunct(void *a)
{
  int i, tmp;
  for (i = 0; i < NITER; i++)
  {
    sem_wait(&semCNT);

    tmp = CNT;     /* copy the global CNT locally */
    tmp = tmp + 1; /* increment the local copy */
    CNT = tmp;     /* store the local value into the global CNT */
    
    sem_post(&semCNT);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  pthread_t tId1, tId2;

  sem_init(&semCNT, 0, 1);

  if (pthread_create(&tId1, NULL, counterFunct, NULL))
  {
    perror("\n ERROR creating thread 1");
    exit(1);
  }

  if (pthread_create(&tId2, NULL, counterFunct, NULL))
  {
    perror("\n ERROR creating thread 2");
    exit(1);
  }

  if (pthread_join(tId1, NULL)) /* wait for the thread 1 to finish */
  {
    perror("\n ERROR joining thread");
    exit(1);
  }

  if (pthread_join(tId2, NULL)) /* wait for the thread 2 to finish */
  {
    perror("\n ERROR joining thread");
    exit(1);
  }

  // sem_getvalue(&semCNT, &CNT);

  if (CNT < 2 * NITER)
    printf("\n BOOM! CNT is [%d], should be %d\n", CNT, 2 * NITER);
  else
    printf("\n OK! CNT is [%d]\n", CNT);

  pthread_exit(NULL);
}
