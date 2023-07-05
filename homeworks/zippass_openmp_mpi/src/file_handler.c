// Copyright 2023 Randy Aguero Bermudez

/**
 * @file file_handler.c
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief  This file contains the implementation of methods related to file
 * handling used in the program.
 *
 * @version 1.1
 * @date 2023-06-10
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
    fprintf(stderr, "Error: The file could not be opened\n");
    return false;
  }

  char* char_alphabet = calloc(sizeof(char), MAX_LINE_LENGTH);
  fgets(char_alphabet, MAX_LINE_LENGTH, txt_file);

  if (!char_alphabet) {
    fprintf(stderr, "Error: Could not read the alphabet\n");
    free(char_alphabet);
    fclose(txt_file);
    return false;
  }
  char_alphabet[strcspn(char_alphabet, "\n")] = '\0';
  file_data->alphabet = char_alphabet;
  // "alphabet" will be the characters that can be contained
  // in a password of a protected ZIP file

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

  // skip blank line from txt file
  char blank_line[MAX_LINE_LENGTH];
  fgets(blank_line, MAX_LINE_LENGTH, txt_file);

  char** zip_directions = calloc(MAX_NUMBER_ZIP_FILES, sizeof(char*));
 
  char zip_dir[MAX_LINE_LENGTH];

  uint64_t num_files = 0;
  num_files = 0;

  while (fgets(zip_dir, MAX_LINE_LENGTH, txt_file)) {
    // while (is_not_end_of_line) do zip_files_directions =
    // file (read_zip_direction)
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
  file_data->zip_passwords = malloc(num_files*sizeof(char*)); 
  fclose(txt_file);
  return true;
}

void open_file(struct thread_pass_test* test_info) {
  struct zip* zip_file_data = zip_open(test_info->zip_file_dir, 0, NULL);
#pragma omp critical
  {
    if (!zip_file_data) {
      fprintf(stderr, "Error, the zip file %s does not exist\n",
              test_info->zip_file_dir);

      *test_info->stat = ZIP_DOES_NOT_EXIST;
    }
  }

  // Check if the zip is empty
  uint64_t num_files = zip_get_num_entries(zip_file_data, 0);
#pragma omp critical
  {
    if (!num_files) {
      // The zip file is empty
      fprintf(stderr, "Error: The zip file %s does not have files\n",
              test_info->zip_file_dir);
      *test_info->stat = ZIP_IS_EMPTY;
      free(zip_file_data);
    }
  }
  if (!num_files) {
    return;
  }

  for (uint64_t i = 0; i < num_files; i++) {
    struct zip_stat file_stat;
    if (zip_stat_index(zip_file_data, i, 0, &file_stat) != 0) {
#pragma omp critical
      {
        fprintf(stderr,
                "Error: The file %s could not be read; it has invalid data\n",
                zip_get_name(zip_file_data, i, 0));

        *test_info->stat = INVALID_FILE_DATA;
        free(zip_file_data);
      }
      return;
    }
    bool test_more_passwords = true;
    uint64_t pass_counter = 0;
    while (test_more_passwords) {
#pragma omp critical
      {
        if (*(test_info->pass_counter) >= test_info->total_passwords) {
          test_more_passwords = false;
        } else {
          pass_counter = *test_info->pass_counter;
          (*test_info->pass_counter)++;
          if (*test_info->stat != ZIP_NOT_PROCESSED) {
            test_more_passwords = false;
          }
        }
      }
      struct zip_file* file = zip_fopen_index_encrypted(
          zip_file_data, i, 0, test_info->passwords[pass_counter]);
      if (file) {
        // If the combination of characters can give access to a protected ZIP
        // file:
        char* file_content = malloc(file_stat.size);
        if (!file_content) {
          fprintf(stderr, "Error: couldn't allocate memory for the file %s",
                  file_stat.name);
#pragma omp critical
          { *test_info->stat = FAILED_ALLOCATE_MEMORY; }
          free(zip_file_data);
          return;
        }
        char* file_characters = "CI0117-23a";

        uint64_t char_count = zip_fread(file, file_content, file_stat.size);

        if (char_count > 0) {
          if (strncmp(file_characters, file_content, file_stat.size) == 0) {
#pragma omp critical
            {
              *test_info->pass_is_found = true;
              *test_info->stat = ZIP_PROCESSED_SUCESSFULLY;

              test_info->password_file =
                  malloc(strlen(test_info->passwords[pass_counter]) + 1);
              memcpy(test_info->password_file,
                     test_info->passwords[pass_counter],
                     strlen(test_info->passwords[pass_counter]) + 1);
              free(file_content);
              zip_fclose(file);

              zip_close(zip_file_data);
            }

            return;
          }
          zip_fclose(file);
        }
        free(file_content);
      }
    }
  }
  zip_close(zip_file_data);

  return;
}
