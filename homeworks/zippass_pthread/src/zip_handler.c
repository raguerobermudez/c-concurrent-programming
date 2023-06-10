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

enum program_error_code generate_zip_file_data(
    zip_files_passwords* zip_passwords, uint64_t num_zip_files,
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
    zip_passwords->zip_files_dir[i] =
        calloc(strlen(zip_dir[i]) + 1, sizeof(char));
    if (!zip_passwords->zip_files_dir) {
      fprintf(stderr,
              "Error, failed to allocate memory for zip_passwords\n"
              "Function generate_zip_file_data() : passwords_handler.c\n");
      for (uint64_t j = 0; j < i; j++) {
        free(zip_passwords->zip_files_dir[j]);
      }

      free(zip_passwords->zip_files_dir);
      free(zip_passwords->files_passwords);
      return ERROR_DINAMIC_MEMORY;
    }

    snprintf(zip_passwords->zip_files_dir[i], strlen(zip_dir[i]) + 1, "%s",
             zip_dir[i]);
  }
  // The array zip_password_found will store boolean values indicating whether a
  // password for a zip file has been found.
  zip_passwords->zip_password_found = calloc(num_zip_files, sizeof(bool));
  if (!zip_passwords->zip_password_found) {
    fprintf(stderr,
            "Error, failed to allocate memory for zip_passwords\n"
            "Function generate_zip_file_data() : passwords_handler.c\n");
    for (uint64_t j = 0; j < num_zip_files; j++) {
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
            "Function search_zip_passwords() zip_handler.c\n");
    return ERROR_DINAMIC_MEMORY;
  }
  zips_passwords->num_threads = num_threads;
  error_code =
      generate_zip_file_data(zips_passwords, txt_data->num_of_zip_files,
                             txt_data->zip_files_directions);
  if (error_code == ERROR_DINAMIC_MEMORY) {
    if (zips_passwords) {
      free(zips_passwords);
    }
    return ERROR_DINAMIC_MEMORY;
  }

  // At this line we have, zip_passwords
  // have zip_directions
  // zip_password for each zip_file
  // zip_passwords_status
  // All of them are properly initilized

  // Data for password generation will be get it

  struct thread_pass_search_info* thread_info = malloc(sizeof(*thread_info));
  if (!thread_info) {
    fprintf(stderr,
            "Error, The program could not allocated dinamic memory, for "
            "thread_info\n"
            "Function search_zip_passwords() zip_handler.c");
    free_zips_passwords(zips_passwords, txt_data);
    return ERROR_DINAMIC_MEMORY;
  }
  for (uint64_t i = 0; i < txt_data->num_of_zip_files; i++) {
    // The program will search passwords for each file
    thread_info->alphabet = txt_data->alphabet;
    thread_info->pass_is_found = &zips_passwords->zip_password_found[i];
    thread_info->password_file = zips_passwords->files_passwords[i];
    thread_info->password_length = txt_data->max_password_length;
    thread_info->zip_file_dir = zips_passwords->zip_files_dir[i];
    thread_info->stat = ZIP_NOT_PROCESSED;
    thread_info->num_threads = zips_passwords->num_threads;

    find_password(thread_info);
  }
  // The results will be printed
  // Print

  for (uint64_t i = 0; i < txt_data->num_of_zip_files; i++) {
    if (zips_passwords->zip_password_found) {
      // if is found
      printf("|%s |", zips_passwords->zip_files_dir[i]);
      if(!zips_passwords->files_passwords[i]){
      printf(" %s\n", zips_passwords->files_passwords[i]);
      }
    }
  }

  free_zips_passwords(zips_passwords, txt_data);
  if (thread_info) {
    free(thread_info);
  }
  return NO_ERROR;
}

void free_zips_passwords(struct zip_files_passwords* zips_passwords,
                         txt_file_data* txt_data) {
  // Free zips_passwords
  if (zips_passwords->zip_password_found) {
    free(zips_passwords->zip_password_found);
  }
  for (uint64_t i = 0; i < txt_data->num_of_zip_files; i++) {
    if (zips_passwords->files_passwords[i]) {
      free(zips_passwords->files_passwords[i]);
    }
    if (zips_passwords->zip_files_dir[i]) {
      free(zips_passwords->zip_files_dir[i]);
    }
  }
  if (zips_passwords->zip_files_dir) {
    free(zips_passwords->zip_files_dir);
  }
  if (zips_passwords->files_passwords) {
    free(zips_passwords->files_passwords);
  }
  if (zips_passwords) {
    free(zips_passwords);
  }
}
