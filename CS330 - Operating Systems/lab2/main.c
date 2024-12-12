#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

int main(int argc, char *argv[])
{
  int *array;
  int max = -9999;
  int size = 0;

  printf("How big is the array? ");
  scanf("%i", &size);

  array = malloc(sizeof(int) * size);


  if (initialize(array, size) != 0)
  {
    printf("initialization error\n");
    exit(1);
  }

  if (findMax(array, size, &max) != 0)
  {
    printf("someFunction error\n");
    exit(1);
  }
  printf("The max value in array is: %d\n", max);

  free(array);

  exit(0);
}
