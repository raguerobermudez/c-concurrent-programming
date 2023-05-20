// Copyright 2023 Randy Aguero Bermudez
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
/**
 * @file file_handler.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-05-17
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

#define MAX_LINE_LENGTH 2048
#define MAX_NUMBER_ZIP_FILES 100

typedef struct txt_file_data {
  char* alphabet;
  uint64_t max_password_length;
  uint64_t num_of_zip_files;
  char** zip_files_directions;
} txt_data;

/**
 * @brief read_txt_file processes txt file
 *
 * @param file A txt file gived as a program argument
 * @param file_data A variable of type struct txt_file_data
 * @return true If the txt file can be sucesfully read
 * @return false If the txt file is not a valid txt file
 */
bool read_txt_file(char* file, txt_data* file_data);

#endif