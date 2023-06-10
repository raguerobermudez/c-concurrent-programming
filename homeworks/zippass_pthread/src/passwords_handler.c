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

void free_passwords(char** passwords, uint64_t amout_passwords) {
  for (uint64_t i = 0; i < amout_passwords; i++) {
    free(passwords[i]);
  }
  free(passwords);
}

/*void free_test_pass(struct thread_pass_test* test_data, uint64_t total) {
  for (uint64_t i = 0; i < total; i++) {
  }
  free(test_data);
}
void free_test_pass(struct thread_pass_test* test_data, uint64_t total) {
  for (uint64_t i = 0; i < total; i++) {
  }
  free(test_data);
}*/

void find_password(struct thread_pass_search_info* thread_info) {
  //  declare num_position :=0
  //  declare password_generated[password_lenght];
  //  declare password_temp[]
  //  declare is_password_found

  // Threads
  pthread_mutex_t* mutex_pass = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(mutex_pass, NULL);

  enum test_code_stats* stat = malloc(sizeof(*stat));
  *stat = ZIP_NOT_PROCESSED;
  // This will check password if a zip file is finally processed.

  ////////////////////////////////////////////////////////////////////////////////
  uint64_t pass_length_counter = 1;
  bool generate_more_password = true;
  // If at problem is found or a password is found, no more passwords
  // will be generated

  // Static mapping will be used

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  while (pass_length_counter <= thread_info->password_length &&
         generate_more_password) {
    // This will be used to check if a thread is used.
    // It's used at join

    uint64_t total_combinations =
        pow_u(strlen(thread_info->alphabet), pass_length_counter);

    // Password will be generated
    char** passwords = malloc(total_combinations * sizeof(char*));
    generate_passwords(passwords, thread_info->alphabet, pass_length_counter);
    if (!passwords) {
      fprintf(stderr,
              "Error, failed to allocated dynamic memory\n"
              "password_handler.c find_passwords()\n");
      return;
    }

    // Threads info
    bool thread_create[thread_info->num_threads];
    for (uint64_t i = 0; i < thread_info->num_threads; i++) {
      thread_create[i] = false;
    }

    // Thread information

    // Thread mapping

    uint64_t amount_passwords_thread =
        floor(total_combinations / thread_info->num_threads);

    uint64_t num_threads = 0;
    if (thread_info->num_threads > total_combinations) {
      num_threads = total_combinations;
      printf("%" PRIu64 "\n", num_threads);
    } else {
      num_threads = thread_info->num_threads;
      printf("%" PRIu64 "\n", num_threads);
    }

    struct thread_test_passwords* thread_passwords =
        malloc(sizeof(*thread_passwords) * num_threads);
    pthread_t threads[num_threads];

    for (uint64_t i = 0; i < num_threads; i++) {
      thread_passwords[i].mutex_pass = mutex_pass;
      thread_passwords[i].pass_is_found = thread_info->pass_is_found;
      thread_passwords[i].password_file = thread_info->password_file;
      thread_passwords[i].stat = stat;
      thread_passwords[i].passwords = passwords;
      thread_passwords[i].zip_file_dir = thread_info->zip_file_dir;

      uint64_t amount_test_thread = floor(total_combinations * num_threads);
      thread_passwords[i].start_index =
          i * amount_passwords_thread +
          min_val(i, mod_val(total_combinations, num_threads));
    }
    for (uint64_t i = 0; i < num_threads; i++) {
      if ((i + 1) < num_threads) {
        thread_passwords[i].finish_index = thread_passwords[i + 1].start_index;
      } else {
        thread_passwords[i].finish_index = total_combinations;
      }
    }
    for (uint64_t i = 0; i < num_threads; i++) {
      pthread_create(&threads[i], NULL, (void*)thread_test_passwords,
                     &thread_passwords[i]);
    }

    for (uint64_t i = 0; i < num_threads; i++) {
      // All threads are made
      pthread_join(threads[i], NULL);
    }

    if (*stat != ZIP_NOT_PROCESSED) {
      generate_more_password = false;
      if (*stat == ZIP_PROCESSED_SUCESSFULLY) {
        // printf("|%s|\n", thread_passwords[0].password_file);
        // printf("*%s*\n", thread_info->password_file);
      }
    }

    free(thread_passwords);
    free(passwords);
    pass_length_counter++;
  }

  free(stat);
}

void generate_passwords(char** passwords, char* alphabet,
                        uint64_t password_length) {
  uint64_t total_combinations = pow_u(strlen(alphabet), password_length);

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  for (uint64_t i = 0; i < total_combinations; i++) {
    passwords[i] = malloc((password_length + 1) *
                          sizeof(char));  // +1 para el carácter nulo final
    if (passwords[i] == NULL) {
      fprintf(stderr, "Error: Failed to allocate memory for passwords");
      return;
    }
  }

  uint64_t password_counter = 0;

  while (password_counter < total_combinations) {
    uint64_t n = password_counter;
    char* password = calloc(password_length + 1, sizeof(char));
    for (uint64_t k = 0; k < password_length; k++) {
      passwords[password_counter][password_length - k - 1] =
          alphabet[n % strlen(alphabet)];
      n /= strlen(alphabet);
    }

    passwords[password_counter][password_length] = '\0';

    password_counter++;
    // printf("%s\n",password);
  }
}