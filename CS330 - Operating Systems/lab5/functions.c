#include "functions.h"
/* FUNCTION: initialize
 * This function should initialize the array
 * to random values between 1 and 500
 *
 * The arguments are:
 *     array: pointer to an array of integer values
 *     length: size of array
 *
 * It returns:
 *     0: on success
 *     non-zero: on an error
 */
int initialize(int *array, int length)
{
  srand(getpid());
  int i;

  for (i = 0; i < length; i++)
  {
    array[i] = rand() % 500 + 1;
  }

  return 0;
}
/* FUNCTION: findMin
 * This function should find the smallest element in the array and
 * return it through the argument min.
 *
 * The arguments are:
 *     array: pointer to an array of integer values
 *     length: size of array
 *     min: set to the smallest value in array
 *
 * It returns:
 *     0: on success
 *     non-zero: on an error
 */
void* findMin(void* arrayPackage)
{
  struct ArrayPackage* arrPackage = arrayPackage;

  printf("Start:%i, End:%i\n", arrPackage->start, arrPackage->end);

  int i;
  
  int* array = arrPackage->array;

  if (pthread_mutex_init(&min_lock, NULL) != 0)
  {
    perror("Could not create mutex for min");
    return (void*)1; 
  }

  if (pthread_mutex_lock(&min_lock) != 0)
  {
    perror("Could not lock mutex for min");
    return (void*)1;
  }

  // MIN = array[0];
  for (i = arrPackage->start; i < arrPackage->end; i++)
  {
    if (MIN > array[i])
    {
      MIN = array[i];
    }
  }

  if (pthread_mutex_unlock(&min_lock) != 0)
  {
    perror("Could not unlock mutex for min");
    return (void*)1; 
  }

  // Do some memory cleanup
  free(arrPackage);

  return 0;
}

/* FUNCTION: printArray
 * This function should print all the elements of the array
 *
 * The arguments are:
 *     array: pointer to an array of integer values
 *     length: size of array
 *
 * It returns:
 *     0: on success
 *     non-zero: on an error
 */
int printArray(int *array, int length)
{
  int i;

  for (i = 0; i < length; i++)
  {
    printf("%d ", array[i]);
    // add a newline character for readability
    if ((i + 1) % 15 == 0)
    {
      printf("\n");
    }
  }
  printf("\n");
  return 0;
}
