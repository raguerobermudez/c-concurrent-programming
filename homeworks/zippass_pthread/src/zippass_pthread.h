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
  NO_ERROR
} program_error_code;

struct thread_pass_gen_info {
  char** passwords;
  uint64_t amount_password;
  char* alphabet;
  uint64_t pos_char;
  uint64_t interval;
};

struct thread_test_password {
  char** passwords;
  char* zip_dir;
  bool* pass_found;
  char* password_found;
  uint64_t zip_dir_num;
  uint64_t num_passwords;
  uint32_t num_threads;
};

struct thread_test_pass_stats {
  char* password;
  bool is_password_found;
  enum test_password_code* test_code;
  struct zip_file_test_data* zip_data_thread;
  struct test_status* status;
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