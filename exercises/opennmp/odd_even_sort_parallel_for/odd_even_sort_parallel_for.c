#include <inttypes.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

void parallel_odd_even_sort(size_t n, double arr[n]) {
  for (uint64_t phase = 0; phase < n; phase++) {
    if (phase % 2 == 0) {
#pragma omp parallel for
      for (uint64_t i = 1; i < n; i += 2) {
        if (arr[i - 1] > arr[i]) {
          swap(&arr[i - 1], &arr[i]);
        }
      }
    } else {
#pragma omp parallel for
      for (uint64_t i = 1; i < n - 1; i += 2) {
        if (arr[i] > arr[i + 1]) {
          swap(&arr[i], &arr[i + 1]);
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Error, size of array and num of threads must be supplied\n");
    return 1;
  }

  size_t n = strtoul(argv[1], NULL, 10);
  int num_threads = atoi(argv[2]);
  double* arr = malloc(n * sizeof(double));

  if (!arr) {
    fprintf(stderr, "Failed to allocate memory for the array\n");
    return 1;
  }

  srand(time(NULL));
  for (uint64_t i = 0; i < n; ++i) {
    arr[i] = (double)rand();
  }

  omp_set_num_threads(num_threads);
  parallel_odd_even_sort(n, arr);

  /*for (uint64_t i = 0; i < n; ++i) {
    printf("%.2f \n", arr[i]);
  }
  printf("\n");*/

  free(arr);

  return 0;
}