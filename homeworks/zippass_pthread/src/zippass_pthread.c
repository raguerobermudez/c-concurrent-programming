// Copyright 2023 Randy Aguero Bermudez

/**
 * @file zippass_pthread.c
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "zippass_pthread.h"

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file_handler.h"
#include "zip_handler.h"

/*void thread_test_passwords(struct thread_test_passwords* test_passwords) {
  for(uint64_t i = 0; i< test_passwords->amount_passwords;i++){
    if(test_passwords->thread_test_data[i] != NULL){
      open_file(&test_passwords->thread_test_data[i]);
    }
  }
}*/

int main(int argc, char* argv[]) {
  program_error_code error_code = 0;

  // Input verification

  /* if(num_args = 2) then
    declare file_txt := read_txt_file(argument[1])
  */
  if (argc != 3) {
    fprintf(stderr,
            "Error, you must include a .txt file as argument and a number of "
            "threads (It has to be integer greater than 0\n");
    error_code = INVALID_ARGUMENTS;
    return error_code;
  }

  // Number of threads

  uint32_t number_threads = 0;
  number_threads = atoi(argv[2]);
  if (number_threads < 1) {
    fprintf(stderr,
            "Error, The number of threads must be greater than 0, Default "
            "system number of threads will be used\n");
    // If number of threads is less than 1, then,
    // default's number of threads will be used
    number_threads = sysconf(_SC_NPROCESSORS_ONLN);
  }
  // Input file read

  txt_file_data txt_file;
  if (!read_txt_file(argv[1], &txt_file)) {
    error_code = INVALID_TXT_FILE;
    return error_code;
  }
  // Search for passwords

  error_code = search_zip_passwords(number_threads, &txt_file);

  for (uint64_t i = 0; i < txt_file.num_of_zip_files; i++) {
    if (txt_file.zip_files_directions[i]) {
      free(txt_file.zip_files_directions[i]);
    }
  }
  if (txt_file.zip_files_directions) {
    free(txt_file.zip_files_directions);
  }

  if (txt_file.alphabet) {
    free(txt_file.alphabet);
  }

  return error_code;
}

void thread_test_passwords(struct thread_test_passwords* test_passwords) {
  bool generate_more_passwords = true;
  
  struct thread_pass_test* pass_test = malloc(sizeof(*pass_test));
  pass_test->mutex_pass = test_passwords->mutex_pass;
  pass_test->pass_is_found = test_passwords->passwords;
  pass_test->password_file = test_passwords->password_file;
  pass_test->stat = test_passwords->stat;

  uint64_t password_counter = test_passwords->start_index;
  while (password_counter < test_passwords->finish_index &&
         generate_more_passwords) {
    pass_test->password = test_passwords->passwords[password_counter];
    pthread_mutex_lock(test_passwords->mutex_pass);
    if (test_passwords->stat != ZIP_NOT_PROCESSED) {
      generate_more_passwords = false;
    }
    pthread_mutex_unlock(test_passwords->mutex_pass);
    password_counter++;
  }
  free(pass_test);
}
