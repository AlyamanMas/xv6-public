#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_SIZE 100

void
bubble_sort(int arr[], int n)
{
  int i, j, temp;
  for (i = 0; i < n - 1; i++) {
    for (j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    printf(2, "Usage: sort <number1> <number2> ...\n");
    exit();
  }

  int numbers[MAX_SIZE];
  int i, n = argc - 1;
  for (i = 0; i < n; i++) {
    numbers[i] = atoi(argv[i + 1]);
  }

  bubble_sort(numbers, n);

  printf(1, "Sorted numbers:");
  for (i = 0; i < n; i++) {
    printf(1, " %d", numbers[i]);
  }
  printf(1, "\n");

  exit();
}
