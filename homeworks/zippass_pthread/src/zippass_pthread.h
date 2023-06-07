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

enum test_code_stats{
  ZIP_DOES_NOT_EXIST,
  ZIP_IS_EMPTY,  
  INVALID_FILE_DATA,
  FAILED_ALLOCATE_MEMORY,
  ZIP_PROCESSED_SUCESSFULLY,
  ZIP_NOT_PROCESSED
};
struct thread_pass_search_info{
  //Shared info
  uint64_t password_length;
  char* alphabet;

  // Private info
  char* password_file;
  bool* pass_is_found;
  char* zip_file_dir;
  enum test_code_stats stat; 
};

struct thread_pass_test{
  char* password_file;
  char* password;
  bool* pass_is_found;
  char* zip_file_dir;
  enum test_code_stats stat;
};
/**
 * @brief generate_zip_password uses a force-brute algorithm to find the correct
 * password for a zip file
 *
 * @param password_lenght The maximum posible length of a password
 * @param ALPHABET Posible characters that can be used in a password
 * @param zip_dir The directory of the zip file to be opened
 */
void generate_zip_password(uint64_t* password_lenght, const char* ALPHABET,
                           const char* zip_dir, const uint64_t num_threads);

// test_code test_password_zip_file(test_password_info pass_info);
#endif