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
  }
  // Search for passwords
  error_code = search_zip_passwords(number_threads, &txt_file);

  printf("%s\n",txt_file.alphabet);
  printf("%"PRIu64"\n",txt_file.max_password_length);
  printf("%"PRIu64"\n",txt_file.num_of_zip_files);
  for (uint64_t i = 0; i < txt_file.num_of_zip_files; i++) {
    printf("%s\n",txt_file.zip_files_directions[i]);
    free(txt_file.zip_files_directions[i]);
  }
  free(txt_file.zip_files_directions);
  free(txt_file.alphabet);
  return error_code;
}
