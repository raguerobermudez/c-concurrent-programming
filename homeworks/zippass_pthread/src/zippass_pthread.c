#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zippass_pthread.h"
#include "file_handler.h"
#include "passwords_handler.h"
#include "zip_handler.h"
typedef struct paswword_data {
  char** passwords_gen;
  uint64_t max_password_length;
  char* alphabet;
} password_data;

uint64_t pow_u(uint64_t base, uint64_t exp) {
  // Example taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  uint64_t result = 1;
  for (uint64_t i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

void* generate_passwords_list(uint64_t amount_passwords, uint32_t pass_length) {
  char** password_list = calloc(amount_passwords, sizeof(char*));
  if (!(password_list)) {
    fprintf(stderr, "Error, failed to allocated memory for passwords");
    return NULL;
  }
  for (uint64_t i = 0; i < amount_passwords; i++) {
    (password_list)[i] = calloc(pass_length + 1, sizeof(char));
    if (!(password_list[i])) {
      for (uint64_t j = 0; j < i; j++) {
        free((password_list)[j]);
      }
      free(password_list);
      return NULL;
    }
  }

  return password_list;
}

/*void generate_password(char** passwords, uint64_t amount_password,
                       char* alphabet, uint64_t pos_char, uint64_t interval) {*/
void generate_password(thread_pass_gen_info* thread_info) {
  uint64_t counter_interval = 0;
  uint64_t counter_alphabet = 0;
  for (uint64_t i = 0; i < thread_info->amount_password; i++) {
    thread_info->passwords[i][thread_info->pos_char] =
        thread_info->alphabet[counter_alphabet];
    counter_interval++;
    if (counter_interval == thread_info->interval) {
      counter_alphabet++;
      counter_interval = 0;
    }
    if (counter_alphabet == strlen(thread_info->alphabet)) {
      counter_alphabet = 0;
    }
  }
}
int main() {
  password_data data;
  data.alphabet = "abcdefghijklmopqrstuvwxyz";
  uint32_t pass_length = 5;
  uint64_t amount_passwords = pow_u(strlen(data.alphabet), pass_length);
  // Memory is save
  data.passwords_gen = generate_passwords_list(amount_passwords, pass_length);
  if (!data.passwords_gen) {
    return 0;
  }

  uint64_t num_threads = sysconf(_SC_NPROCESSORS_ONLN);
  pthread_t threads[num_threads];
  thread_pass_gen_info thread_info;
  thread_info.passwords = data.passwords_gen;
  thread_info.amount_password = amount_passwords;
  thread_info.alphabet = data.alphabet;
  uint64_t thread_counter = 0;
  uint64_t used_thread_counter = 0;

  for (uint64_t index_pass = 0; index_pass < pass_length; index_pass++) {
    // alfabeto, intervalo, pos_char
    thread_info.pos_char = index_pass;
    thread_info.interval =
        pow_u(strlen(data.alphabet), (pass_length - 1) - index_pass);

    pthread_create(&threads[thread_counter], NULL,(void*) generate_password,
                   (void*)&thread_info);
    thread_counter++;
    if (thread_counter >= num_threads) {
      thread_counter = 0;
    }
    if (used_thread_counter < num_threads) {
      used_thread_counter++;
    }
    generate_password(&thread_info);
    /* generate_password(
       data.passwords_gen, amount_passwords, data.alphabet, index_pass,
       pow_u(strlen(data.alphabet), (pass_length - 1) - index_pass));*/
  }

  for (uint64_t i = 0; i < used_thread_counter; i++) {
    pthread_join(threads[i],NULL);
  }

  for (uint64_t i = 0; i < amount_passwords; i++) {
    printf("i %" PRIu64 " %s\n", i, data.passwords_gen[i]);
  }
  for (uint64_t i = 0; i < amount_passwords; i++) {
    free(data.passwords_gen[i]);
  }
  free(data.passwords_gen);
}
