// Copyright 2023 Randy Aguero Bermudez

/**
 * @file passwords_handler.c
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "passwords_handler.h"

/*void generate_zip_password(uint64_t* password_lenght, const char* ALPHABET,
                           const char* zip_dir) {
  //  declare num_position :=0
  //  declare password_generated[password_lenght];
  //  declare password_temp[]
  //  declare is_password_found

  uint64_t pass_lenght = 0;
  bool generate_more_password = true;
  test_code* password_test = NULL;
  char* password = 0;

  // Password generation was taken from
  //
https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
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
      if (password_test->error_code == ZIP_DOES_NOT_EXIST) {
      clear && make clean && make && bin/zippass_pthread tests/input003.txt 8
break;
      }
      if (password_test->error_code == INVALID_FILE_DATA) {
        generate_more_password = false;
        break;
      }
      if (password_test->error_code == FAILED_ALLOCATE_MEMORY) {
        generate_more_password = false;
        break;
      }
      if (password_test->error_code == ZIP_PROCESSED_SUCESSFULLY) {
        password = password_gen;
        generate_more_password = false;
        printf("%s %s\n", zip_dir, password);
        break;
      }
    }
    pass_lenght++;
  }

  if (password_test->error_code != ZIP_PROCESSED_SUCESSFULLY) {
    printf("%s\n", zip_dir);
  }
  free(password_test);
}*/

// Cambiar funcion void
void find_password(struct thread_pass_search_info* thread_info) {
  //  declare num_position :=0
  //  declare password_generated[password_lenght];
  //  declare password_temp[]
  //  declare is_password_found

  uint64_t pass_lenght = 0;
  bool generate_more_password = true;

  struct thread_pass_test* test_pass = malloc(sizeof(*test_pass));
  test_pass->pass_is_found = thread_info->pass_is_found;
  test_pass->zip_file_dir = thread_info->zip_file_dir;
  test_pass->stat = thread_info->stat;
  test_pass->password_file = thread_info->password_file;

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  while (pass_lenght <= thread_info->password_length &&
         generate_more_password) {
    char* password_gen = calloc(pass_lenght + 1, sizeof(char));
    uint64_t total_posible_combination =
        pow_u(strlen(thread_info->alphabet), pass_lenght);
    uint64_t alphabet_index = 0;
    while (alphabet_index < total_posible_combination &&
           generate_more_password) {
      u_int64_t n = alphabet_index;
      for (uint64_t k = 0; k < pass_lenght; k++) {
        password_gen[pass_lenght - k - 1] =
            thread_info->alphabet[n % strlen(thread_info->alphabet)];
        n /= strlen(thread_info->alphabet);
      }
      alphabet_index++;
      test_pass->password = password_gen;
      // printf("%s\n", password_gen);
      open_file(test_pass);

      if (test_pass->stat != ZIP_NOT_PROCESSED) {
        break;
      }
    }
    free(password_gen);
    pass_lenght++;
  }

  free(test_pass);
  /* if (password_test->error_code != ZIP_PROCESSED_SUCESSFULLY) {
     printf("%s\n", zip_dir);
   }
   free(password_test);*/
}
