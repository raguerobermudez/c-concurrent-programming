// Copyright 2023 Randy Aguero Bermudez

#include "zippass_pthread.h"

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <zip.h>

// procedure main:
int main(int argc, char* argv[]) {
  uint32_t error_code = 0;
  /* if(num_args = 2) then
    declare file_txt := read_txt_file(argument[1])
  */
  if (argc != 3) {
    fprintf(stderr,
            "Error, you must include a .txt file as argument and a number of "
            "threads (It has to be integer greater than 0\n");
    error_code = 1;
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

  txt_data txt_file;
  if (!read_txt_file(argv[1], &txt_file)) {
    error_code = 2;
  }

  for (uint64_t i = 0; i < *txt_file.num_of_zip_files; i++) {
    generate_zip_password(&txt_file.MAX_PASSWORD_LENGTH, txt_file.alphabet,
                          txt_file.zip_files_directions[i]);
  }

  for (uint64_t i = 0; i < *txt_file.num_of_zip_files; i++) {
    free(txt_file.zip_files_directions[i]);
  }
  free(txt_file.zip_files_directions);
  free(txt_file.num_of_zip_files);
  free(txt_file.alphabet);
  // free(txt_file.MAX_PASSWORD_LENGTH);
  // fclose(txt_file.file);

  return error_code;
}

uint64_t pow_u(uint64_t base, uint64_t exp) {
  // Example taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  u_int64_t result = 1;
  for (uint64_t i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

void generate_zip_password(uint64_t* password_lenght, const char* ALPHABET,
                           const char* zip_dir) {
  //  declare num_position :=0
  //  declare password_generated[password_lenght];
  //  declare password_temp[]
  //  declare is_password_found

  uint64_t pass_lenght = 0;
  bool generate_more_password = true;
  test_code password_test;
  char* password = 0;

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  while (pass_lenght <= *password_lenght && generate_more_password) {
    char* password_gen = calloc(pass_lenght + 1, sizeof(char));
    uint64_t total_posible_combination = pow_u(strlen(ALPHABET), pass_lenght);
    uint64_t alphabet_index = 0;
    while (alphabet_index < total_posible_combination &&
           generate_more_password) {
      u_int64_t n = alphabet_index;
      for (uint64_t k = 0; k < pass_lenght; k++) {
        password_gen[pass_lenght - k - 1] = ALPHABET[n % strlen(ALPHABET)];
        n /= strlen(ALPHABET);
      }
      alphabet_index++;

      password_test = test_password_zip_file(password_gen, zip_dir);
      if (password_test.error_code == ZIP_DOES_NOT_EXIST) {
        generate_more_password = false;
        break;
      }
      if (password_test.error_code == ZIP_IS_EMPTY) {
        generate_more_password = false;
        break;
      }
      if (password_test.error_code == INVALID_FILE_DATA) {
        generate_more_password = false;
        break;
      }
      if (password_test.error_code == FAILED_ALLOCATE_MEMORY) {
        generate_more_password = false;
        break;
      }
      if (password_test.error_code == ZIP_PROCESSED_SUCESSFULLY) {
        password = password_gen;
        generate_more_password = false;
        printf("%s %s\n", zip_dir, password);
        break;
      }
    }
    pass_lenght++;
    free(password_gen);
  }

  if (password_test.error_code != ZIP_PROCESSED_SUCESSFULLY) {
    printf("%s\n", zip_dir);
  }
  // free(password_test);
}

test_code test_password_zip_file(char* password, const char* zip_file_dir) {
  test_code password_test_code;

  struct zip* zip_file_data = zip_open(zip_file_dir, 0, NULL);
  // zip_open(zip directory, flags, error variable)

  if (!zip_file_data) {
    fprintf(stderr, "Error, the zip file %s does not exist\n", zip_file_dir);
    password_test_code.error_code = ZIP_DOES_NOT_EXIST;
    return password_test_code;
  }

  // It will analyze if the zip is empty
  uint64_t num_files = zip_get_num_entries(zip_file_data, 0);

  if (!num_files) {
    // The zip file is empty
    fprintf(stderr, "Error, the zip file %s does not have files\n",
            zip_file_dir);
    password_test_code.error_code = ZIP_IS_EMPTY;
    zip_close(zip_file_data);
    return password_test_code;
  }

  for (uint64_t i = 0; i < num_files; i++) {
    struct zip_stat file_stat;
    if (zip_stat_index(zip_file_data, i, 0, &file_stat) != 0) {
      fprintf(stderr,
              "Error the file %s could no be read"
              " it has invalid data\n",
              zip_get_name(zip_file_data, i, 0));
      password_test_code.error_code = INVALID_FILE_DATA;
      zip_close(zip_file_data);
      return password_test_code;
      // This print that a fail has not valid data.
    }
    struct zip_file* file =
        zip_fopen_index_encrypted(zip_file_data, i, 0, password);
    if (file) {
      // If the combination of characters
      // can give access to a protected ZIP file:
      char* file_content = malloc(file_stat.size+1);
      if (!file_content) {
        fprintf(stderr, "Error: couldn't allocate memory for the file %s",
                file_stat.name);
        password_test_code.error_code = FAILED_ALLOCATE_MEMORY;

        zip_close(zip_file_data);
        return password_test_code;
      }
      if (zip_fread(file, file_content, file_stat.size) > 0) {
        char* file_characters = "CI0117-23a\0";
        if (strcmp(file_content, file_characters) == 0) {
          password_test_code.error_code = ZIP_PROCESSED_SUCESSFULLY;

          free(file_content);
          zip_fclose(file);
          zip_close(zip_file_data);
          return password_test_code;
        }
        zip_fclose(file);
      }
      free(file_content);
    }
    zip_close(zip_file_data);
  }
  password_test_code.error_code = ZIP_FILE_NOT_READ;
  return password_test_code;
}

bool read_txt_file(char* file, txt_data* file_data) {
  // procedure read_txt_file:
  FILE* txt_file = fopen(file, "r");
  // file = open_file(argument[1])
  // "r" means "read" mode
  // txt_file will be stored in dynamic memory

  if (!(txt_file)) {
    fprintf(stderr, "The file could not be open\n");
    return false;
  }
  file_data->file = txt_file;
  char* char_alphabet = calloc(sizeof(char), MAX_LINE_LENGHT);
  fgets(char_alphabet, MAX_LINE_LENGHT, txt_file);

  if (!char_alphabet) {
    fprintf(stderr, "Error, Could not read the alphabet\n");
    free(char_alphabet);
    fclose(txt_file);
    return false;
  }
  char_alphabet[strcspn(char_alphabet, "\n")] = '\0';

  file_data->alphabet = char_alphabet;
  // "ALPHABET" will be the characters that can be contained
  // in a password of an protected ZIP file

  // Read maximum password length from the .txt file
  char char_max_password_length[MAX_LINE_LENGHT];
  fgets(char_max_password_length, MAX_LINE_LENGHT, txt_file);
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
  file_data->MAX_PASSWORD_LENGTH = max_pass_length;
  char blank_line[MAX_LINE_LENGHT];
  fgets(blank_line, MAX_LINE_LENGHT, txt_file);
  // skip blank line from txt file

  char** zip_directions = calloc(MAX_NUMBER_ZIP_FILES, sizeof(char*));
  char zip_dir[MAX_LINE_LENGHT];

  uint64_t* num_files = malloc(sizeof(uint64_t));
  *num_files = 0;

  while (fgets(zip_dir, MAX_LINE_LENGHT, txt_file)) {
    // while (is_not_end_of_line) do zip_files_direccionts =
    // file(read_zip_direction)
    // num_zip_files : = +1;

    zip_directions[*num_files] = malloc(strlen(zip_dir) + 1);
    zip_dir[strcspn(zip_dir, "\n")] = '\0';
    strcpy(zip_directions[*num_files], zip_dir);
    (*num_files)++;
  }

  file_data->zip_files_directions = zip_directions;

  // file = close(file)
  if (num_files == 0) {
    free(num_files);
    free(file_data);
    free(zip_directions);
    fclose(txt_file);
    return false;
  }
  file_data->num_of_zip_files = num_files;
  fclose(txt_file);
  return true;
}
