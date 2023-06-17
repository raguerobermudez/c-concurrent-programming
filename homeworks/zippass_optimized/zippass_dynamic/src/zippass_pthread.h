// Copyright 2023 Randy Aguero Bermudez
#ifndef ZIPPASS_PTHREAD_H
#define ZIPPASS_PTHREAD_H
/**
 * @file zippass_serial.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief This file declares all the functions that will be used to open a zip
 * file
 * @version 1.0
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zip.h>

#include "file_handler.h"
#include "zip_handler.h"

#define MAX_LINE_LENGTH 2048
#define MAX_NUMBER_ZIP_FILES 100

typedef enum program_error_code {
  INVALID_ARGUMENTS,
  INVALID_TXT_FILE,
  ERROR_DINAMIC_MEMORY,
  NO_ERROR
} program_error_code;

enum test_code_stats {
  ZIP_DOES_NOT_EXIST,
  ZIP_IS_EMPTY,
  INVALID_FILE_DATA,
  FAILED_ALLOCATE_MEMORY,
  ZIP_PROCESSED_SUCESSFULLY,
  ZIP_NOT_PROCESSED
};
struct thread_pass_search_info {
  // Shared info
  uint64_t password_length;
  char* alphabet;        // 26
  uint32_t num_threads;  // 4

  // Private info
  char* password_file;
  bool* pass_is_found;
  char* zip_file_dir;
  enum test_code_stats stat;
};

struct thread_pass_test {
  char* password_file;
  char** passwords;
  bool* pass_is_found;

  char* zip_file_dir;
  enum test_code_stats* stat;
  pthread_mutex_t* mutex_pass;

  uint64_t* pass_counter;
  uint64_t total_passwords;

};

/**
 * @brief Structure containing information for testing passwords in a ZIP file
 * using pthreads.
 */

struct thread_test_passwords {
  char** passwords;     // Array of passwords to test
  char* zip_file_dir;   // Directory path of the ZIP file
  char* password_file;  // Password file name
  uint64_t*
      thread_id_found_pass;  // Pointer to thread ID where password was found
  uint64_t thread_id;        // Thread id
  uint64_t* pass_counter;
  bool* star_test;
  uint64_t total_passwords;
  pthread_mutex_t* mutex_pass;  // Mutex for syncronizing password testing
  enum test_code_stats* stat;   // Pointer to test status codeu
  bool* pass_is_found;
};

/**
 * @brief This function tests a range of passwords in a ZIP file using multiple
 * threads.
 *
 * @param test_passwords Pointer to thread_test_passwords structure.
 */
void thread_test_passwords(struct thread_test_passwords* test_passwords);

/**
 * @brief Main function for ZIP file password cracking using pthreads.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @return Error code indicating the success or failure of the program.
 */
int main(int argc, char* argv[]);
#endif
