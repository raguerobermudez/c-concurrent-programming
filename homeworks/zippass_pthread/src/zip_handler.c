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

enum program_error_code generate_zip_file_data(zip_files_passwords* zip_passwords,
                                          uint64_t num_zip_files,
                                          char** zip_dir) {

  // The zip directions will be saved in files_passwords and zip_files_dir
  zip_passwords->files_passwords = calloc(num_zip_files, sizeof(char*));
  zip_passwords->zip_files_dir = calloc(num_zip_files, sizeof(char*));
  if (!zip_passwords->files_passwords || !zip_passwords->zip_files_dir) {
    fprintf(stderr,
            "Error, failed to allocate memory for zip_passwords\n"
            "Function generate_zip_file_data() : passwords_handler.c");
    return ERROR_DINAMIC_MEMORY;
  }


  for (uint64_t i = 0; i < num_zip_files; i++) {
    zip_passwords->files_passwords[i] =
        calloc(strlen(zip_dir[i]) + 1, sizeof(char));
    zip_passwords->zip_files_dir[i] =
        calloc(strlen(zip_dir[i]) + 1, sizeof(char));
    if (!zip_passwords->files_passwords[i] || !zip_passwords->zip_files_dir) {
      fprintf(stderr,
              "Error, failed to allocate memory for zip_passwords\n"
              "Function generate_zip_file_data() : passwords_handler.c");
      for (uint64_t j = 0; j < i; j++) {
        free(zip_passwords->files_passwords[j]);
        free(zip_passwords->zip_files_dir[j]);
      }
      free(zip_passwords->files_passwords);
      free(zip_passwords->zip_files_dir);
      return ERROR_DINAMIC_MEMORY;
    }
    snprintf(zip_passwords->files_passwords[i], strlen(zip_dir[i]) + 1, "%s",
             zip_dir[i]);
    snprintf(zip_passwords->zip_files_dir[i], strlen(zip_dir[i]) + 1, "%s",
             zip_dir[i]);
  }
  // The array zip_password_found will store boolean values indicating whether a
  // password for a zip file has been found.
  zip_passwords->zip_password_found = calloc(num_zip_files, sizeof(bool));
  if (!zip_passwords->zip_password_found) {
    fprintf(stderr,
            "Error, failed to allocate memory for zip_passwords\n"
            "Function generate_zip_file_data() : passwords_handler.c");
    for (uint64_t j = 0; j < num_zip_files; j++) {
      free(zip_passwords->files_passwords[j]);
      free(zip_passwords->zip_files_dir[j]);
    }
    free(zip_passwords->files_passwords);
    free(zip_passwords->zip_files_dir);
  }
  return NO_ERROR;
}




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

  // At this line we have, zip_passwords
  // have zip_directions
  // zip_password for each zip_file
  // zip_passwords_status
  // All of them are properly initilized
  
  // Data for password generation will be get it

  struct thread_pass_search_info* thread_info = malloc(sizeof(*thread_info));
  if(!thread_info){
    fprintf(stderr,
            "Error, The program could not allocated dinamic memory, for thread_info\n"
            "Function search_zip_passwords() zip_handler.c");
    return ERROR_DINAMIC_MEMORY;
  }
  for(uint64_t i = 0;i<txt_data->num_of_zip_files;i++){
    //The program will search passwords for each file
    thread_info->alphabet = txt_data->alphabet;
    thread_info->pass_is_found = &zips_passwords->zip_password_found[i];
    thread_info->password_file = zips_passwords->files_passwords[i];
    thread_info->password_length = txt_data->max_password_length;
    thread_info->zip_file_dir = zips_passwords->zip_files_dir[i];
    thread_info->stat = ZIP_NOT_PROCESSED;
   
    find_password(thread_info);
  }

  //The results will be printed
  //Print
  return NO_ERROR;
}
