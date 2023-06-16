// Copyright 2023 Randy Aguero Bermudez

/**
 * @file passwords_handler.c
 * @brief Implementation file for password generation and handling functions.
 *
 *
 * @version 1.1
 * @date 2023-06-10
 */

#include "passwords_handler.h"

void free_passwords(char** passwords, uint64_t amout_passwords) {
  for (uint64_t i = 0; i < amout_passwords; i++) {
    free(passwords[i]);
  }
  free(passwords);
}

void generate_passwords(char** passwords, char* alphabet,
                        uint64_t password_length) {
  uint64_t total_combinations = pow_u(strlen(alphabet), password_length);

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  for (uint64_t i = 0; i < total_combinations; i++) {
    passwords[i] = malloc((password_length + 1) * sizeof(char));
    if (passwords[i] == NULL) {
      fprintf(stderr, "Error: Failed to allocate memory for passwords.");
      return;
    }
  }

  uint64_t password_counter = 0;

  while (password_counter < total_combinations) {
    uint64_t n = password_counter;
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
