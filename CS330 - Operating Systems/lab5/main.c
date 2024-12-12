#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

int MIN = 9999; /*Used when you implement the thread function */

int main(int argc, char *argv[])
{
  int *array;
  struct ArrayPackage onePackage;
  int size = 0;
  int num_threads;
  int increments;
  int min = 9999;
  pthread_t *thread_ids;
  void *p_status;

  if (pthread_mutex_init(&min_lock, NULL) != 0)
  {
    perror("Could not create mutex for MIN: ");
    return 1;
  }
  printf("How many threads? ");
  scanf("%d", &num_threads);
  thread_ids = (pthread_t *)malloc(sizeof(pthread_t *) * num_threads);

  do
  {
    printf("How big is the array (100 elements or more)? ");
    scanf("%i", &size);
  } while (size < 100);
  array = (int *)malloc(sizeof(int) * size);

  increments = size / num_threads;

  if (initialize(array, size) != 0)
  {
    printf("initialization error\n");
    exit(1);
  }

  printArray(array, size);

  for (int i = 0; i < num_threads; i++)
  {
    struct ArrayPackage *onePack =  (struct ArrayPackage *)malloc(sizeof(struct ArrayPackage));
    /*-------------------------------------------------------------------*/
    /* Store the array, the start, and the end into the onePack instance */


    onePack->array = array;
    onePack->start = i * increments;
    onePack->end = onePack->start + increments;

    if (i == num_threads - 1)
    {
      onePack->end = size; 
    }

    /*---------------------------------------------------------------------*/
    /* Call the pthread_create                                             */

    if (pthread_create(&thread_ids[i], NULL, findMin, onePack) > 0)
    {
      perror("creating thread:");
      return 2;
    }
  }

  for (int i = 0; i < num_threads; i++)
  {
    void* retValue;
    if(pthread_join(thread_ids[i], &retValue) != 0)
    {
      perror("Error joining threads: ");
    }
    /*---------------------------------------------------------------------*/
    /*Call the join                                                        */
  }
  /*-----------------------------------------------------------------------*/
  /* The following call to findMin will be taken out when you implement the thread function*/
  /*-----------------------------------------------------------------------*/


  /* The following line will have to be changed                            */
  printf("min value in array is: %d\n", MIN);
  free(array);
  free(thread_ids);
  return 0;
}
