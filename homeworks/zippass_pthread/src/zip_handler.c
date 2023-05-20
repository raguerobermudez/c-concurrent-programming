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

enum program_error_code search_zip_passwords(uint32_t num_threads,
                                             txt_data* txt_file) {
  enum program_error_code error_code = 0;

  // Generate basic zip files password
  //  For each zip file at least the zip direccion will be printed

  zip_files_passwords* zip_passwords = malloc(sizeof(zip_passwords));
  generate_zip_basic_password(zip_passwords, txt_file->num_of_zip_files,
                              txt_file->zip_files_directions);

  if (!zip_passwords->passwords) {
    return INVALID_ZIP_BASIC_PASSWORD;
  }
  bool* zip_password_found = calloc(txt_file->num_of_zip_files, sizeof(bool));
  bool all_zip_passwords_found = false;
  uint64_t pass_length_counter = 1;
  while ((pass_length_counter <= txt_file->max_password_length) &&
         !all_zip_passwords_found) {
    passwords_data* pass_data = malloc(sizeof(passwords_data));
    if (!pass_data) {
      fprintf(stderr,
              "Error, The program could not allocated enough dinamic memory\n");
      return INSUFFICIENTE_DINAMIC_MEMORY;
    }
    error_code = generate_zip_passwords(pass_length_counter, txt_file,
                                        pass_data, num_threads);

    free_generated_passwords(pass_data, pass_length_counter);
    if (error_code != NO_ERROR) {
     
      return error_code;
    }

    all_zip_passwords_found = false;

    for (uint64_t i = 0; i < txt_file->num_of_zip_files; i++) {
      if (/*!zip_password_found[i] ||*/ pass_length_counter>6) {
         all_zip_passwords_found = true;
      }
    }
     
    pass_length_counter++;
  }
  return NO_ERROR;
}
