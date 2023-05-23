// Copyright 2023 Randy Aguero Bermudez

/**
 * @file file_handler.c
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "file_handler.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool read_txt_file(char* file, struct txt_file_data* file_data) {
  // procedure read_txt_file:
  FILE* txt_file = fopen(file, "r");
  // file = open_file(argument[1])
  // "r" means "read" mode
  // txt_file will be stored in dynamic memory

  if (!txt_file) {
    fprintf(stderr, "The file could not be open\n");
    return false;
  }
  char* char_alphabet = calloc(sizeof(char), MAX_LINE_LENGTH);
  fgets(char_alphabet, MAX_LINE_LENGTH, txt_file);

  if (!char_alphabet) {
    fprintf(stderr, "Error, Could not read the alphabet\n");
    free(char_alphabet);
    fclose(txt_file);
    return false;
  }
  char_alphabet[strcspn(char_alphabet, "\n")] = '\0';
  file_data->alphabet = char_alphabet;

  // "alphabet" will be the characters that can be contained
  // in a password of an protected ZIP file

  // Read maximum password length from the .txt file
  char char_max_password_length[MAX_LINE_LENGTH];
  fgets(char_max_password_length, MAX_LINE_LENGTH, txt_file);
  // declare MAX_PASSWORD_LENGHT = file(read second line)

  if (!(*char_max_password_length)) {
    fprintf(stderr, "Error, Could not read the maximum password length\n");
    fclose(txt_file);
    return false;
  }

  uint64_t max_pass_length;
  if (!(max_pass_length = (uint64_t)atoi(char_max_password_length))) {
    fprintf(stderr, "%s is not a valid integer number\n",
            char_max_password_length);
    fclose(txt_file);
    return false;
  }
  file_data->max_password_length = max_pass_length;
  char blank_line[MAX_LINE_LENGTH];
  fgets(blank_line, MAX_LINE_LENGTH, txt_file);
  // skip blank line from txt file

  char** zip_directions = calloc(MAX_NUMBER_ZIP_FILES, sizeof(char*));
  char zip_dir[MAX_LINE_LENGTH];

  uint64_t num_files = 0;
  num_files = 0;

  while (fgets(zip_dir, MAX_LINE_LENGTH, txt_file)) {
    // while (is_not_end_of_line) do zip_files_direccionts =
    // file(read_zip_direction)
    // num_zip_files : = +1;

    zip_directions[num_files] = malloc(strlen(zip_dir) + 1);
    zip_dir[strcspn(zip_dir, "\n")] = '\0';
    snprintf(zip_directions[num_files], strlen(zip_dir) + 1, "%s", zip_dir);
    (num_files)++;
  }

  file_data->zip_files_directions = zip_directions;

  // file = close(file)
  if (num_files == 0) {
    // free(num_files);
    free(file_data);
    free(zip_directions);
    fclose(txt_file);
    return false;
  }
  file_data->num_of_zip_files = num_files;
  fclose(txt_file);
  return true;
}