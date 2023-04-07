// Copyright 2023 Randy Aguero Bermudez
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int NUMBER_AMOUNT = 2;
// Procedure buy Numbers: Each thread will generate random numbers
void *buy_numbers(void* seed);

// The procedure to get return values using pthread_join was taken from
// https://www.youtube.com/watch?v=ln3el6PR__Q
// "CodeVault: Get return value from a thread (pthread_join)"

// procedure main:
int main(void) {
     // The numbers will be bought
    int *numbers[NUMBER_AMOUNT];

    // create_thread(greet)
    pthread_t thread[NUMBER_AMOUNT];
    int error = 0;
    // Random numbers seed
    unsigned int seed = time(NULL);
    for (int i = 0; i < NUMBER_AMOUNT; i++) {
        error = pthread_create(&thread[i], /*thread atributes*/ NULL,
         buy_numbers, /*function arguments*/ (void**)&seed);
        if (error != EXIT_SUCCESS) {
            fprintf(stderr, "Error: %d could not bought an number\n", i++);
            break;
        }
    }

    for (int i = 0; i < NUMBER_AMOUNT; i++) {
        pthread_join(thread[i], (void **)&numbers[i]);
    }

    if (error == EXIT_SUCCESS) {
        for (int i = 0; i < NUMBER_AMOUNT; i++) {
            printf("The thread # %d has bougth the number: %d\n",
             i + 1, *numbers[i]);
        }
    }
    for (int i = 0; i < NUMBER_AMOUNT; i++) {
        free(numbers[i]);
    }
    return error;
}
// end procedure

void *buy_numbers(void* seed) {
    int random_number = (rand_r(seed) % 100) + 1;
    // print("Hello from secondary thread")
    int *selected_number = malloc(sizeof(int));
    *selected_number = random_number;
    return (void *)selected_number;
}
