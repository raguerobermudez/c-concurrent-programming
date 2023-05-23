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

program_error_code search_zip_passwords (uint32_t num_threads,txt_file_data* txt_data){
 program_error_code error_code = 0;

  // Allocate passwords for each zip_file
  // each zip file will have to show the file direction
  zip_files_passwords*  zips_passwords= malloc(sizeof(*zips_passwords));
  if(!zips_passwords){
    fprintf(stderr, "Error, The program could not allocated dinamic memory, for zip passwords\n"
    "Function search_zip_passwords() zip_handler.c");
  }

  return NO_ERROR;
}

/*enum program_error_code search_zip_passwords(uint32_t num_threads,
                                             txt_data* txt_file) {
  enum program_error_code error_code = 0;

  // Generate basic zip files password
  //  For each zip file at least the zip direccion will be printed

  struct zip_files_passwords* zip_passwords_stats =
      malloc(sizeof(*zip_passwords_stats));
  generate_zip_basic_password(zip_passwords_stats, txt_file->num_of_zip_files,
                              txt_file->zip_files_directions);

  if (!zip_passwords_stats->zip_passwords) {
    return INVALID_ZIP_BASIC_PASSWORD;
  }
  zip_passwords_stats->zip_password_found =
      calloc(txt_file->num_of_zip_files, sizeof(bool));
  if (!zip_passwords_stats->zip_password_found) {
    fprintf(stderr,
            "Error, the programa could not allocated enough memory for "
            "zip_passsword_found array");
    return INSUFFICIENT_PASS_FOUND_MEMORY;
  }

  bool all_zip_passwords_found = false;
  uint64_t pass_length_counter = 1;
  while ((pass_length_counter <= txt_file->max_password_length) &&
         !all_zip_passwords_found) {
    struct passwords_data* pass_data = malloc(sizeof(*pass_data));
    if (!pass_data) {
      fprintf(stderr,
              "Error, The program could not allocated enough dinamic memory\n");
      return INSUFFICIENT_DINAMIC_MEMORY;
    }
    error_code = generate_zip_passwords(pass_length_counter, txt_file,
                                        pass_data, num_threads);

    // Seach passwords for each zip file

    // find_zip_passwords(txt_file, pass_data, zip_passwords_stats,
num_threads);

    if (error_code != NO_ERROR) {
      return error_code;
    }

    all_zip_passwords_found = false;

    for (uint64_t i = 0; i < txt_file->num_of_zip_files; i++) {
      if ((zip_passwords_stats->zip_password_found[i] == true) ||
          pass_length_counter > 6) {
        all_zip_passwords_found = true;
      }
    }
    free_generated_passwords(pass_data, pass_length_counter);
    pass_length_counter++;
  }
  return NO_ERROR;
}*/