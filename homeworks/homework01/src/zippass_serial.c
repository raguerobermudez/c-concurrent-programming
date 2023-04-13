// Copyright 2023 Randy Aguero Bermudez

#include "zippass_serial.h"

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <zip.h>

// procedure main:
int main(int argc, char* argv[]) {
  uint32_t ERROR = 0;
  /* if(num_args = 2) then
    declare file_txt := read_txt_file(argument[1])
  */
  if (argc != 2) {
    fprintf(stderr, "Error, you must include a .txt file as argument\n");
    ERROR = 1;
    return ERROR;
  }
  txt_data txt_file;
  if (!read_txt_file(argv[1], &txt_file)) {
    fprintf(stderr, "Error, Could not open the file");
    ERROR = 2;
  }

  return ERROR;
}

bool read_txt_file(char* file, txt_data* file_data) {
  // procedure read_txt_file:
  FILE* txt_file = malloc(sizeof(FILE));
  // txt_file will be stored in dynamic memory
  txt_file = fopen(file, "r");
  // file = open_file(argument[1])
  // "r" means "read" mode

  if (!txt_file) {
    fprintf(stderr, "The file could not be open");

    return false;
  }
  file_data->file = txt_file;
  char* char_alphabet = calloc(sizeof(char), MAX_LINE_LENGHT);
  fgets(char_alphabet, MAX_LINE_LENGHT, txt_file);

  if (!char_alphabet) {
    fprintf(stderr, "Error, Could not read the alphabet");
    free(char_alphabet);
    return false;
  }
  file_data->alphabet = char_alphabet;
  // "ALPHABET" will be the characters that can be contained
  // in a password of an protected ZIP file

  // Read maximum password length from the .txt file
  char char_max_password_length[MAX_LINE_LENGHT];
  fgets(char_max_password_length, MAX_LINE_LENGHT, txt_file);
  // declare MAX_PASSWORD_LENGHT = file(read second line)
  if (*char_max_password_length) {
    fprintf(stderr, "Error, Could not read the maximum password length");
    return false;
  }

  uint64_t* max_pass_length = malloc(sizeof(uint64_t));
  if (!(*max_pass_length = (uint64_t)atoi(char_max_password_length))) {
    fprintf(stderr, "%s is not a valid integer number",
            char_max_password_length);
    return false;
  }

  char blank_line[MAX_LINE_LENGHT];
  fgets(blank_line, MAX_LINE_LENGHT, txt_file);
  // skip blank line from txt file

  char** zip_directions = calloc(MAX_NUMBER_ZIP_FILES, sizeof(char*));
  char zip_dir[MAX_LINE_LENGHT];

  file_data->num_of_zip_files = malloc(sizeof(uint64_t));
  file_data->num_of_zip_files = 0;

  while (fgets(zip_dir, MAX_LINE_LENGHT, txt_file)) {
    // while (is_not_end_of_line) do zip_files_direccionts =
    // file(read_zip_direction)
    // num_zip_files : = +1;

    printf("Zip file %s\n", zip_dir);
    zip_directions[*file_data->num_of_zip_files] = zip_dir;
    file_data->num_of_zip_files++;
  }
  file_data->zip_files_directions = zip_directions;
  fclose(txt_file);
  // file = close(file)
  if (file_data->num_of_zip_files == 0) {
    return false;
  }
  return true;
}