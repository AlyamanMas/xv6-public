#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_SIZE 100

void
printTextAndFloat(const char* text, float value)
{
  int fixedPointValue = (int)(value * 1000000); // 6 decimal places
  printf(1,
         "%s %d.%d\n",
         text,
         fixedPointValue / 1000000,
         fixedPointValue % 1000000);
}

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

float
custom_sqrt(float num)
{
  float x = num;
  float y = 1.0;
  float epsilon = 0.000001;

  while (x - y > epsilon) {
    x = (x + y) / 2;
    y = num / x;
  }

  return x;
}

float
calculate_mean(int arr[], int n)
{
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += arr[i];
  }
  return (float)sum / n;
}

float
calculate_standard_deviation(int arr[], int n, float mean)
{
  float sum_squared_diff = 0;
  for (int i = 0; i < n; i++) {
    sum_squared_diff += (arr[i] - mean) * (arr[i] - mean);
  }
  return custom_sqrt(sum_squared_diff / n);
}

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    printf(2, "Usage: printstats <number1> <number2> ...\n");
    exit();
  }

  int numbers[MAX_SIZE];
  int i, n = argc - 1;
  for (i = 0; i < n; i++) {
    numbers[i] = atoi(argv[i + 1]);
  }

  bubble_sort(numbers, n);

  float mean = calculate_mean(numbers, n);
  float standard_deviation = calculate_standard_deviation(numbers, n, mean);

  int median;
  if (n % 2 == 0) {
    median = (numbers[n / 2 - 1] + numbers[n / 2]) / 2;
  } else {
    median = numbers[n / 2];
  }

  int min = numbers[0];
  int max = numbers[n - 1];

  printTextAndFloat("Average: ", mean);
  printTextAndFloat("Standard Deviation: ", standard_deviation);
  printf(1, "Median: %d\n", median);
  printf(1, "Min: %d\n", min);
  printf(1, "Max: %d\n", max);

  exit();
}
