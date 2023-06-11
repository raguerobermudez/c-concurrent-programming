// Copyright 2023 Randy Aguero Bermudez
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
/**
 * @file file_handler.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file_handler.h"
#include "zippass_pthread.h"

#define MAX_LINE_LENGTH 2048
#define MAX_NUMBER_ZIP_FILES 100

struct thread_pass_test;

typedef struct txt_file_data {
  char* alphabet;
  uint64_t max_password_length;
  uint64_t num_of_zip_files;
  char** zip_files_directions;
} txt_file_data;

/**
 * @brief Reads data from a text file.
 *
 * @param file The name of the text file.
 * @param file_data Pointer to a struct to store the read data.
 * @return bool Returns true if the file is successfully read, false otherwise.
 */
bool read_txt_file(char* file, struct txt_file_data* file_data);

/**
 * @brief Opens and analyzes a zip file.
 *
 * @param test_info Pointer to a struct containing test information.
 */
void open_file(struct thread_pass_test* test_info);

#endif
