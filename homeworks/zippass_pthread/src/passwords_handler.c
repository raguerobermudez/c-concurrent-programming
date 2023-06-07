// Copyright 2023 Randy Aguero Bermudez

/**
 * @file passwords_handler.c
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "passwords_handler.h"

/*void generate_zip_password(uint64_t* password_lenght, const char* ALPHABET,
                           const char* zip_dir) {
  //  declare num_position :=0
  //  declare password_generated[password_lenght];
  //  declare password_temp[]
  //  declare is_password_found

  uint64_t pass_lenght = 0;
  bool generate_more_password = true;
  test_code* password_test = NULL;
  char* password = 0;

  // Password generation was taken from
  //
https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  while (pass_lenght <= *password_lenght && generate_more_password) {
    char* password_gen = calloc(pass_lenght + 1, sizeof(char));
    uint64_t total_posible_combination = pow_u(strlen(ALPHABET), pass_lenght);
    uint64_t alphabet_index = 0;
    while (alphabet_index < total_posible_combination &&
           generate_more_password) {
      u_int64_t n = alphabet_index;
      for (uint64_t k = 0; k < pass_lenght; k++) {
        password_gen[pass_lenght - k - 1] = ALPHABET[n % strlen(ALPHABET)];
        n /= strlen(ALPHABET);
      }
      alphabet_index++;

      password_test = test_password_zip_file(password_gen, zip_dir);
      if (password_test->error_code == ZIP_DOES_NOT_EXIST) {
      clear && make clean && make && bin/zippass_pthread tests/input003.txt 8
break;
      }
      if (password_test->error_code == INVALID_FILE_DATA) {
        generate_more_password = false;
        break;
      }
      if (password_test->error_code == FAILED_ALLOCATE_MEMORY) {
        generate_more_password = false;
        break;
      }
      if (password_test->error_code == ZIP_PROCESSED_SUCESSFULLY) {
        password = password_gen;
        generate_more_password = false;
        printf("%s %s\n", zip_dir, password);
        break;
      }
    }
    pass_lenght++;
  }

  if (password_test->error_code != ZIP_PROCESSED_SUCESSFULLY) {
    printf("%s\n", zip_dir);
  }
  free(password_test);
}*/

// Cambiar funcion void

char* generate_password(char* alphabet, int pass_length) {
  uint64_t total_posible_combination = pow_u(strlen(alphabet), pass_length);

  char** passwords = calloc(total_posible_combination, sizeof(char*));
  if (!passwords) {
    return NULL;
  }
  for (uint64_t i = 0; i < total_posible_combination; i++) {
    passwords[i] = calloc(pass_length, sizeof(char));
    if (!passwords[i]) {
      for (uint64_t j = 0; j < i; j++) {
        free(&passwords[j]);
      }
      free(passwords);
    }
  }

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition

  char* password_gen = calloc(pass_length + 1, sizeof(char));
  uint64_t alphabet_index = 0;
  while (alphabet_index < total_posible_combination) {
    uint64_t n = alphabet_index;
    for (int k = 0; k < pass_length; k++) {
      passwords[alphabet_index][pass_length - k - 1] =
          alphabet[n % strlen(alphabet)];
      n /= strlen(alphabet);
    }
    alphabet_index++;
  }
  return passwords;
}
void find_password(struct thread_pass_search_info* thread_info) {
  //  declare num_position :=0
  //  declare password_generated[password_lenght];
  //  declare password_temp[]
  //  declare is_password_found

  // Threads
  pthread_mutex_t* mutex_pass = malloc(sizeof(pthread_mutex_t));
  // pthread_mutex_init(mutex_pass, NULL);

  // Semaphore
  // sem_t semaphore;
  // sem_init(&semaphore, 0, 0);

  // Threads
  pthread_t threads[thread_info->num_threads];
  bool thread_create[thread_info->num_threads];

  bool generate_more_password = true;

  for (uint64_t i = 0; i < thread_info->num_threads; i++) {
    thread_create[i] = false;
  }

  uint64_t pass_length_counter = 0;

  while (pass_length_counter <= 3) {
    char* passwords =
        generate_password(thread_info->alphabet, pass_length_counter);
     

    pass_length_counter++;
  }
}
// Password generation was taken from
// https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition

// Join

/*for (u_int64_t i = 0; i < thread_info->num_threads; i++) {
  if (thread_create[i]) {
    pthread_join(threads[i], NULL);
  }
}*/
// pthread_mutex_destroy(mutex_pass);
//  free(mutex_pass);
//   free(test_pass);
/* if (password_test->error_code != ZIP_PROCESSED_SUCESSFULLY) {
   printf("%s\n", zip_dir);
 }
 free(password_test);*/
