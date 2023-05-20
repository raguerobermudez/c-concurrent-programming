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

void free_generated_passwords(passwords_data* pass_data, uint64_t pass_length) {
  uint64_t amount_passwords = pow_u(strlen(pass_data->alphabet), pass_length);
  for (uint64_t i = 0; i < amount_passwords; i++) {
    free(pass_data->generated_passwords[i]);
  }
  free(pass_data->generated_passwords);
}

enum program_error_code generate_zip_passwords(uint64_t pass_length,
                                               txt_data* txt_file,
                                               passwords_data* pass_data,
                                               uint32_t num_threads) {
  pass_data->alphabet = txt_file->alphabet;
  uint64_t amount_passwords = pow_u(strlen(pass_data->alphabet), pass_length);
  // Memory is save
  pass_data->generated_passwords =
      generate_passwords_list(amount_passwords, pass_length);
  if (!pass_data->generated_passwords) {
    return INSUFFICIENT_PASSWORDS_MEMORY;
  }
  pthread_t threads[num_threads];
  thread_pass_gen_info* thread_info;
  thread_info = malloc(sizeof(thread_pass_gen_info) * num_threads);
  if (!thread_info) {
    fprintf(stderr, "Error, could not allocate enough memory for thread data");
    return INSUFFICIENTE_THREAD_PASSWORD_MEMORY;
  }
  for (uint64_t i = 0; i < num_threads; i++) {
    thread_info[i].passwords = pass_data->generated_passwords;
    thread_info[i].amount_password = amount_passwords;
    thread_info[i].alphabet = pass_data->alphabet;
  }

  uint64_t thread_counter = 0;
  uint64_t used_thread_counter = 0;

  for (uint64_t index_pass = 0; index_pass < pass_length; index_pass++) {
    // alfabeto, intervalo, pos_char
    thread_info[thread_counter].pos_char = index_pass;
    thread_info[thread_counter].interval =
        pow_u(strlen(pass_data->alphabet), (pass_length - 1) - index_pass);

    pthread_create(&threads[thread_counter], NULL, (void*)generate_passwords,
                   (void*)&thread_info[thread_counter]);
    thread_counter++;
    if (thread_counter >= num_threads) {
      thread_counter = 0;
    }
    if (used_thread_counter < num_threads) {
      used_thread_counter++;
    }
  }

  for (uint64_t i = 0; i < used_thread_counter; i++) {
    pthread_join(threads[i], NULL);
  }

 /*for (uint64_t i = 0; i < amount_passwords; i++) {
    printf("|i :%" PRIu64 "|  Password: !%s!\n", i, pass_data->generated_passwords[i]);
  }
  printf("\n");*/
  return NO_ERROR;
}

void* generate_passwords_list(uint64_t amount_passwords, uint32_t pass_length) {
  char** password_list = calloc(amount_passwords, sizeof(char*));
  if (!(password_list)) {
    fprintf(stderr,
            "Error, The program could not allocated enough memory to save "
            "passwords\n");
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

void generate_passwords(thread_pass_gen_info* thread_info) {
  assert(thread_info);
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
void generate_zip_basic_password(zip_files_passwords* zip_passwords,
                                 uint64_t num_zip_files, char** zip_dir) {
  zip_passwords->passwords = calloc(num_zip_files, sizeof(char*));
  if (!zip_passwords->passwords) {
    fprintf(stderr, "Error, failed to allocate memory for zip files passwords");
    return;
  }
  for (uint64_t i = 0; i < num_zip_files; i++) {
    zip_passwords->passwords[i] = calloc(strlen(zip_dir[i]) + 1, sizeof(char));
    if (!zip_passwords->passwords[i]) {
      fprintf(stderr, "Error, a zip file direction could not be allocated");
      for (uint64_t j = 0; j < i; j++) {
        free(zip_passwords->passwords[j]);
      }
      free(zip_passwords->passwords);
      return;
    }
    snprintf(zip_passwords->passwords[i], strlen(zip_dir[i]) + 1, "%s",
             zip_dir[i]);
  }
}
