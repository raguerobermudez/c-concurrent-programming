

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(double* a, double* b) {
  uint64_t temp = *a;
  *a = *b;
  *b = temp;
}

void serial_odd_even_sort(size_t n, double arr[n]) {
  for (uint64_t phase = 0; phase < n; phase++) {
    if (phase % 2 == 0) {
      for (uint64_t i = 1; i < n; i += 2) {
        if (arr[i - 1] > arr[i]) {
          swap(&arr[i - 1], &arr[i]);
        }
      }
    } else {
      for (uint64_t i = 1; i < n - 1; i += 2) {
        if (arr[i] > arr[i + 1]) {
          swap(&arr[i], &arr[i + 1]);
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Error, size of array must be supplied\n");
    return 1;
  }

  size_t n = strtoul(argv[1], NULL, 10);
  double* arr = malloc(n * sizeof(double));

  if (!arr) {
    fprintf(stderr, "Failed to allocate memory for the array\n");
    return 1;
  }

  srand(time(NULL));
  for (uint64_t i = 0; i < n; ++i) {
    arr[i] = (double)rand();
  }

  serial_odd_even_sort(n, arr);

  /*for (uint64_t i = 0; i < n; ++i) {
    printf("%.2f \n", arr[i]);
  }
  printf("\n");*/

  free(arr);

  return 0;
}
