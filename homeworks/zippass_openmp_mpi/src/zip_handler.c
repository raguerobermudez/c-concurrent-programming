// Copyright 2023 Randy Aguero Bermudez
/**
 * @file zip_handler.c
 * @brief Implementation of functions for handling ZIP files and searching for
 * passwords.
 *
 * @version 1.1
 * @date 2023-06-10
 */

#include "zip_handler.h"

program_error_code search_zip_passwords(uint32_t num_threads,
                                        struct process_zip_data* txt_data) {
  struct thread_pass_search_info* thread_info = malloc(sizeof(*thread_info));
  if (!thread_info) {
    fprintf(
        stderr,
        "Error: The program could not allocate dynamic memory for thread_info\n"
        "Function search_zip_passwords(): zip_handler.c\n");
    return ERROR_DINAMIC_MEMORY;
  }

  bool* pass_is_found = calloc(1, sizeof(*pass_is_found));
  if (!pass_is_found) {
    fprintf(stderr,
            "Error: The program could not allocate dynamic memory for "
            "pass_is_Found\n"
            "Function search_zip_passwords(): zip_handler.c\n");
    free(thread_info);
    return ERROR_DINAMIC_MEMORY;
  }

  thread_info->alphabet = txt_data->alphabet;
  thread_info->pass_is_found = pass_is_found;
  thread_info->password_file = NULL;
  thread_info->password_length = txt_data->max_password_length;
  thread_info->zip_file_dir = txt_data->zip_file_dir;
  thread_info->stat = ZIP_NOT_PROCESSED;
  thread_info->num_threads = num_threads;

  find_password(thread_info);

  if (*thread_info->pass_is_found) {
    txt_data->zip_file_pass = malloc(strlen(thread_info->password_file) + 1);
    memcpy(txt_data->zip_file_pass, thread_info->password_file,
           strlen(thread_info->password_file) + 1);
  }
  if (thread_info) {
    free(thread_info);
  }
  free(pass_is_found);

  return NO_ERROR;
}

void find_password(struct thread_pass_search_info* thread_info) {
  enum test_code_stats* stat = malloc(sizeof(*stat));
  *stat = ZIP_NOT_PROCESSED;
  // This will check password if a zip file is finally processed.

  uint64_t pass_length_counter = 1;
  bool generate_more_password = true;
  // If a problem is found or a password is found, no more passwords
  // will be generated

  uint64_t* thread_id_found_pass = malloc(sizeof(*thread_id_found_pass));
  *thread_id_found_pass = 0;

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  while (pass_length_counter <= thread_info->password_length &&
         generate_more_password) {
    uint64_t total_combinations =
        pow_u(strlen(thread_info->alphabet), pass_length_counter);

    // Password will be generated
    char** passwords = malloc(total_combinations * sizeof(char*));
    generate_passwords(passwords, thread_info->alphabet, pass_length_counter);
    if (!passwords) {
      fprintf(stderr,
              "Error: Failed to allocate dynamic memory\n"
              "Function find_password() in password_handler.c\n");
      free(thread_id_found_pass);
      free(stat);
      return;
    }

    // Thread mapping
    // Dynamic mapping
    uint64_t num_threads = 0;
    if (thread_info->num_threads > total_combinations) {
      num_threads = total_combinations;

    } else {
      num_threads = thread_info->num_threads;
    }

    struct thread_test_passwords* thread_passwords =
        malloc(sizeof(*thread_passwords) * num_threads);

    uint64_t* pass_counter = malloc(sizeof(uint64_t));
    *pass_counter = 0;

    bool* start_test = malloc(sizeof(*start_test));
    *start_test = false;

    for (uint64_t i = 0; i < num_threads; i++) {
      thread_passwords[i].pass_is_found = thread_info->pass_is_found;
      thread_passwords[i].password_file = NULL;
      thread_passwords[i].stat = stat;
      thread_passwords[i].thread_id = i;
      thread_passwords[i].thread_id_found_pass = thread_id_found_pass;
      thread_passwords[i].passwords = passwords;
      thread_passwords[i].zip_file_dir = thread_info->zip_file_dir;
      // Dynamic changes
      thread_passwords[i].pass_counter = pass_counter;
      thread_passwords[i].total_passwords = total_combinations;
      thread_passwords[i].star_test = start_test;
    }

    /*for (uint64_t i = 0; i < num_threads; i++) {
      printf("%" PRIu64 "i: start index %" PRIu64 "\n", i,
             thread_passwords[i].start_index);
      printf("%" PRIu64 "i: finish index %" PRIu64 "\n", i,
             thread_passwords[i].finish_index);
    }*/

#pragma omp parallel num_threads(num_threads)
    {
#pragma omp parallel for schedule(dynamic)
      for (uint64_t i = 0; i < num_threads; i++) {
        thread_test_passwords(&thread_passwords[i]);
      }
    }

    if (*stat != ZIP_NOT_PROCESSED) {
      generate_more_password = false;
    }

    if (*thread_info->pass_is_found) {
      thread_info->password_file = malloc(
          strlen(thread_passwords[(*thread_id_found_pass)].password_file) + 1);

      memcpy(thread_info->password_file,
             thread_passwords[(*thread_id_found_pass)].password_file,
             strlen(thread_passwords[*thread_id_found_pass].password_file) + 1);
    }

    for (uint64_t i = 0; i < num_threads; i++) {
      if (thread_passwords[i].password_file) {
        free(thread_passwords[i].password_file);
      }
    }
    free(thread_passwords);
    free(pass_counter);
    free(start_test);
    free_passwords(passwords, total_combinations);
    pass_length_counter++;
  }
  free(thread_id_found_pass);
  free(stat);
}
