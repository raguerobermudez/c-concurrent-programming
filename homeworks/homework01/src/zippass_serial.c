// Copyright 2023 Randy Aguero Bermudez

#include "zippass_serial.h"

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
    ERROR = 2;
  }

  /*
  typedef struct txt_file_data {
  FILE* file;
  char* alphabet;
  uint64_t MAX_PASSWORD_LENGTH;
  uint64_t* num_of_zip_files;
  char** zip_files_directions;
} txt_data;
*/

  for (uint64_t i = 0; i < *txt_file.num_of_zip_files; i++) {
    generate_zip_password(&txt_file.MAX_PASSWORD_LENGTH, txt_file.alphabet,
                          txt_file.zip_files_directions[i]);
  }
    return ERROR;
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

bool generate_zip_password(uint64_t* password_lenght, const char* ALPHABET,
                           const char* zip_dir) {
  // printf("LLega aqui");
  //  declare num_position :=0
  //  declare password_generated[password_lenght];
  //  declare password_temp[]
  //  declare is_password_found

  uint64_t pass_lenght = 0;
  bool is_password_found = false;

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  while (pass_lenght <= *password_lenght && !is_password_found) {
    char* password_gen = calloc(pass_lenght, sizeof(char));
    uint64_t total_posible_combination = pow_u(strlen(ALPHABET), pass_lenght);
    uint64_t alphabet_index = 0;
    while (alphabet_index < total_posible_combination && !is_password_found) {
      u_int64_t n = alphabet_index;
      for (uint64_t k = 0; k < pass_lenght; k++) {
        password_gen[pass_lenght - k - 1] = ALPHABET[n % strlen(ALPHABET)];
        n /= strlen(ALPHABET);
      }
      alphabet_index++;
      is_password_found = test_password_zip_file(password_gen, zip_dir);
    }
    pass_lenght++;
  }

  return is_password_found;
}

bool test_password_zip_file(const char* password, const char* zip_file_dir) {
  // error_codes:
  // ZIP_PROCESSED_SUCESSFULLY
  // ZIP_DOES_NOT_EXIST
  // ZIP_HAS_NOT_ANY_PASSWORD
  // ZIP_IS_EMPTY
  // OPEN_FILE_UNSUSSESFULLY (Invalid password)

  test_code* password_test_code;
  password_test_code = malloc(sizeof(test_code));
  bool is_valid_password = false;

  struct zip* zip_file_data = zip_open(zip_file_dir, 0, NULL);
  // zip_open(zip directory, flags, error variable)

  if (!zip_file_data) {
    // if rhe zip file does not exist:
    fprintf(stderr, "Error, the zip file does not exist\n");
    password_test_code->error_code = ZIP_DOES_NOT_EXIST;
    return false;
  }

  // It will analyze if the zip is empty
  uint64_t num_files = zip_get_num_entries(zip_file_data, 0);
  
  // zip_get_num_files — is obsolete,
  if (!num_files) {
    // The zip file is empty
    password_test_code->error_code = ZIP_IS_EMPTY;
    // return password_test_code;
  }
  // For each file in the zip, the password, will be used to read each
  // file in the zip
  for (uint64_t i = 0; i < num_files; i++) {
    struct zip_stat file_stat;
    if (zip_stat_index(zip_file_data, i, 0, &file_stat) != 0) {
      fprintf(stderr,
              "Error the file %s could no be read"
              " it has invalid data\n",
              zip_get_name(zip_file_data, i, 0));
      // This print that a fail has not valid data.
    }

    // If the combination of characters
    // can give access to a protected ZIP file:
    // print(zip_file_open + password)
    // Print the name of the ZIP file and the found
    // password to standard output.

    struct zip_file* file = zip_fopen_index(zip_file_data, i, 0);
    if (file) {
      // The zip file is not encrypted
      printf("%s", zip_file_dir);
      password_test_code->error_code = ZIP_HAS_NOT_ANY_PASSWORD;
      is_valid_password = true;
    } else {
      file = zip_fopen_index_encrypted(zip_file_data, i, 0, password);
      if (file) {
        printf("%s %s\n", zip_file_dir, password);
       
        is_valid_password = true;
      } else {
        password_test_code->error_code = OPEN_FILE_UNSUSSESFULLY;
      }
    }
  }
  zip_close(zip_file_data);
  return is_valid_password;
  
}

bool read_txt_file(char* file, txt_data* file_data) {
  // procedure read_txt_file:
  FILE* txt_file = malloc(sizeof(FILE));
  // txt_file will be stored in dynamic memory
  txt_file = fopen(file, "r");
  // file = open_file(argument[1])
  // "r" means "read" mode

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
    return false;
  }

  uint64_t* max_pass_length = malloc(sizeof(uint64_t));
  if (!(*max_pass_length = (uint64_t)atoi(char_max_password_length))) {
    fprintf(stderr, "%s is not a valid integer number\n",
            char_max_password_length);
    return false;
  }
  file_data->MAX_PASSWORD_LENGTH = *max_pass_length;
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
  fclose(txt_file);
  // file = close(file)
  if (num_files == 0) {
    return false;
    free(num_files);
    free(file_data);
    free(zip_directions);
  }
  file_data->num_of_zip_files = num_files;

  return true;
}