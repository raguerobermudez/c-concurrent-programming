#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
// fcntl and sys/stat is for named semaphores
#include <unistd.h>
/*
 * Examples taken from:
 * https://eric-lo.gitbook.io/synchronization/semaphore-in-c*/
typedef struct shared_data {
  sem_t* can_greet;
  uint64_t thread_count;
} shared_data_t;

typedef struct private_data {
  uint64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

void* greet(void* data);
int create_threads(shared_data_t* shared_data);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) != 1) {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->can_greet = sem_open("/semaphore", O_CREAT, 0666, 1);
    if (shared_data->can_greet != SEM_FAILED) {
      shared_data->thread_count = thread_count;

      error = create_threads(shared_data);

      sem_close(shared_data->can_greet);
      sem_unlink("/semaphore");
    } else {
      fprintf(stderr, "Error: could not create named semaphore\n");
      error = 13;
    }

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }
  return error;
}

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;

  pthread_t* threads = calloc(shared_data->thread_count, sizeof(pthread_t));
  private_data_t* private_data = calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count; ++thread_number) {
      if (error == EXIT_SUCCESS) {
        private_data[thread_number].thread_number = thread_number;
        private_data[thread_number].shared_data = shared_data;

        error = pthread_create(&threads[thread_number], NULL, greet, &private_data[thread_number]);
        if (error != EXIT_SUCCESS) {
          fprintf(stderr, "Error: could not create secondary thread\n");
          error = 21;
          break;
        }
      } else {
        fprintf(stderr, "Error: could not init semaphore\n");
        error = 22;
        break;
      }
    }

    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count; ++thread_number) {
      pthread_join(threads[thread_number], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n", shared_data->thread_count);
    error = 23;
  }

  return error;
}

void* greet(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  sem_wait(shared_data->can_greet);

  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n", private_data->thread_number,
         shared_data->thread_count);

  sem_post(shared_data->can_greet);

  return NULL;
}
