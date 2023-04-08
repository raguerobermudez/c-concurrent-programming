// Copyright 2023 Randy Aguero Bermudez

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  size_t pinata_max_hits;
  size_t* pinata_saved_hits;
  size_t thread_broke_pinata_id;
  pthread_mutex_t pinata_can_take_hit;
} pinata;

typedef struct {
  size_t thread_id;
  pinata* pinata;
} thread_turn_info;

void* hit_pinata(void* t_info);

// procedure main:
int main(int argc, char* argv[]) {
  // argc anumber of arguments
  // argv array with arguments
  if (argc != 3) {
    fprintf(
        stderr,
        "Error, you must enter a number of threads, and max number of hits\n");
    return 1;
  }
  const size_t NUMBER_THREADS = atoi(argv[1]);
  if (NUMBER_THREADS == 0) {
    fprintf(stderr, "Error, A positive number of thread must be entered\n");
    return 1;
  }

  const size_t MAX_HITS = atoi(argv[2]);
  if (MAX_HITS < 1) {
    fprintf(stderr,
            "Error, A valid maximum number of hits must be entered, it must be "
            "a positive integer\n");
    return 1;
  }
  // Number of hits

  pinata pinata_party;
  pinata_party.pinata_max_hits = MAX_HITS;
  pinata_party.thread_broke_pinata_id = 0;
  pinata_party.pinata_saved_hits = calloc(NUMBER_THREADS, sizeof(size_t));

  pthread_t threads[NUMBER_THREADS];
  // threads generated

  // Array with a counter of hits

  int error_T1 = 0;
  // Program error status

  pthread_mutex_init(&pinata_party.pinata_can_take_hit, NULL);
  // Only one thread can hit the pinata at the same time
  thread_turn_info* thread_info =
      malloc(sizeof(thread_turn_info) * NUMBER_THREADS);
  for (size_t i = 0; i < NUMBER_THREADS; i++) {
    thread_info[i].thread_id = i;
    thread_info[i].pinata = &pinata_party;
    error_T1 = pthread_create(&threads[i], /*thread atributes*/ NULL,
                              (void*)hit_pinata,
                              /*function arguments*/ (void*)&thread_info[i]);
    if (error_T1 != EXIT_SUCCESS) {
      fprintf(stderr, "Error: A thread failed to hit the pinata\n");
      break;
    }
  }
  for (size_t i = 0; i < NUMBER_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  for (size_t i = 0; i < NUMBER_THREADS; i++) {
    if (pinata_party.thread_broke_pinata_id == i) {
      printf("Thread %zu/%zu: %zu hits, I broke the pinata \n", i + 1,
             NUMBER_THREADS, pinata_party.pinata_saved_hits[i]);
    } else {
      printf("Thread %zu/%zu: %zu hits\n", i + 1, NUMBER_THREADS,
             pinata_party.pinata_saved_hits[i]);
    }
  }
  free(thread_info);
  free(pinata_party.pinata_saved_hits);

  return 0;
}

void* hit_pinata(void* t_info) {
  thread_turn_info* thread_info = t_info;
  size_t thread_id = thread_info->thread_id;
  size_t* hit_thread_list = thread_info->pinata->pinata_saved_hits;
  pinata* pinata_party = (thread_info->pinata);
  bool pinata_is_not_broken = true;

  while (pinata_is_not_broken) {
    pthread_mutex_lock(&(pinata_party->pinata_can_take_hit));
    if (pinata_party->pinata_max_hits > 0) {
      pinata_party->pinata_max_hits--;
      hit_thread_list[thread_id]++;

      if (pinata_party->pinata_max_hits == 0) {
        pinata_party->thread_broke_pinata_id = thread_id;
        pinata_is_not_broken = false;
      }
    } else {
      pinata_is_not_broken = false;
    }
    pthread_mutex_unlock(&(pinata_party->pinata_can_take_hit));
  }
  return NULL;
}
