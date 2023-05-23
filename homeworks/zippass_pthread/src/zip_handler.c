// Copyright 2023 Randy Aguero Bermudez

/**
 * @file zip_handler.c
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "zip_handler.h"

/*enum program_error_code open_zip_files(char* zip_direction,
                                       struct zip_file_test_data* zip_files) {
  struct zip* zip_file_data = zip_open(zip_direction, 0, NULL);
  // zip_open(zip directory, flags, error variable)

  if (!zip_file_data) {
    fprintf(stderr, "Error, the zip file %s does not exist\n", zip_direction);
    return ZIP_DOES_NOT_EXIST;
  }
  zip_files->zip_data = zip_file_data;
  // It will analyze if the zip is empty
  uint64_t num_files = zip_get_num_entries(zip_file_data, 0);
  zip_files->num_files = num_files;
  if (zip_files->num_files <= 0) {
    // The zip file is empty
    fprintf(stderr, "Error, the zip file %s does not have files\n",
            zip_direction);
    zip_close(zip_files->zip_data);
    return ZIP_IS_EMPTY;
  }

  for (uint64_t j = 0; j < zip_files->num_files; j++) {
    if (zip_stat_index(zip_files->zip_data, j, 0, &zip_files->file_stat) != 0) {
      fprintf(stderr,
              "Error the file %s could no be read"
              " it has invalid data\n",
              zip_get_name(zip_files->zip_data, j, 0));

      zip_close(zip_files->zip_data);
      return INVALID_FILE_DATA;
      // This print that a fail has not valid data.
    }
  }

  return NO_ERROR;
}*/

program_error_code search_zip_passwords(uint32_t num_threads,
                                        txt_file_data* txt_data) {
  program_error_code error_code = 0;

  // Save passwords for each zip_file
  // each zip file will have to show the file direction
  zip_files_passwords* zips_passwords = malloc(sizeof(*zips_passwords));
  if (!zips_passwords) {
    fprintf(stderr,
            "Error, The program could not allocated dinamic memory, for zip "
            "passwords\n"
            "Function search_zip_passwords() zip_handler.c");
    return ERROR_DINAMIC_MEMORY;
  }

  error_code =
      generate_zip_file_data(zips_passwords, txt_data->num_of_zip_files,
                             txt_data->zip_files_directions);
  if (error_code == ERROR_DINAMIC_MEMORY) {
    return ERROR_DINAMIC_MEMORY;
  }

  // The program will generate passwords from size 0 to the maximum password
  // length given in the input TXT file.

  struct passwords_data* pass_data = malloc(sizeof(*pass_data));
  if (!pass_data) {
    fprintf(stderr,
            "Error, The program could not allocated enough dinamic memory, for "
            "pass_data\n"
            "search_zip_passwords: zip_handler.c");
    return ERROR_DINAMIC_MEMORY;
  }
  pass_data->alphabet = txt_data->alphabet;

  pthread_mutex_init(&zips_passwords->password_found_mutex, NULL);
  uint64_t pass_length_counter = 1;
  while (pass_length_counter <= txt_data->max_password_length) {
    // Passwords will be generated;
    error_code =
        generate_zip_passwords(pass_length_counter, pass_data, num_threads);
    if (error_code != NO_ERROR) {
      break;
    }
    // the program will check if all passwords are found.
    error_code = find_zip_passwords(zips_passwords, pass_data, num_threads);
    pthread_mutex_lock(&zips_passwords->password_found_mutex);
    bool continue_searching_passwords = true;
    for (u_int64_t i = 0; i < txt_data->num_of_zip_files; i++) {
      if (zips_passwords->zip_password_found[i] == false) {
        continue_searching_passwords = false;
      }
    }

    if (continue_searching_passwords) {
      // free_generated_passwords(pass_data, pass_length_counter);
      // free(pass_data);
      break;
    }
    pthread_mutex_unlock(&zips_passwords->password_found_mutex);
    // free(pass_data->generated_passwords);
    pass_length_counter++;
  }
  // free_generated_passwords(pass_data, pass_length_counter);
  /*

    free(zips_dir);
    free(zips_passwords);

  */
  return NO_ERROR;
}
