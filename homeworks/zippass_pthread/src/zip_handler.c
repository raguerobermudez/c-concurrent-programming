// Copyright 2023 Randy Aguero Bermudez
/**
 * @file zip_handler.c
 * @brief Implementation of functions for handling ZIP files and searching for
 * passwords.
 *
 * @version 1.1
 * @date 2023-06-10
 */

#include "zip_handler.h"

enum program_error_code generate_zip_file_data(
    zip_files_passwords* zip_passwords, uint64_t num_zip_files,
    char** zip_dir) {
  // Allocate memory for files_passwords and zip_files_dir
  zip_passwords->files_passwords = calloc(num_zip_files, sizeof(char*));
  zip_passwords->zip_files_dir = calloc(num_zip_files, sizeof(char*));
  if (!zip_passwords->files_passwords || !zip_passwords->zip_files_dir) {
    fprintf(stderr,
            "Error: Failed to allocate memory for zip_passwords\n"
            "Function generate_zip_file_data() : zip_handler.c");
    return ERROR_DINAMIC_MEMORY;
  }

  for (uint64_t i = 0; i < num_zip_files; i++) {
    // Allocate memory for zip_files_dir[i] and copy the directory
    zip_passwords->zip_files_dir[i] =
        calloc(strlen(zip_dir[i]) + 1, sizeof(char));

    if (!zip_passwords->zip_files_dir) {
      fprintf(stderr,
              "Error: Failed to allocate memory for zip_passwords\n"
              "Function generate_zip_file_data() : zip_handler.c\n");
      // Free the allocated memory before returning the error
      for (uint64_t j = 0; j < i; j++) {
        free(zip_passwords->zip_files_dir[j]);
      }

      free(zip_passwords->zip_files_dir);
      free(zip_passwords->files_passwords);
      return ERROR_DINAMIC_MEMORY;
    }

    // Copy the ZIP file directory
    snprintf(zip_passwords->zip_files_dir[i], strlen(zip_dir[i]) + 1, "%s",
             zip_dir[i]);
  }
  // Allocate memory for zip_password_found array
  zip_passwords->zip_password_found = calloc(num_zip_files, sizeof(bool));
  if (!zip_passwords->zip_password_found) {
    fprintf(stderr,
            "Error: Failed to allocate memory for zip_passwords\n"
            "Function generate_zip_file_data() : zip_handler.c\n");
    // Free the allocated memory before returning the error
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
  // Each zip file will have to show the file direction
  zip_files_passwords* zips_passwords = malloc(sizeof(*zips_passwords));
  if (!zips_passwords) {
    fprintf(stderr,
            "Error: Failed to allocate memory for zip passwords\n"
            "Function search_zip_passwords(): zip_handler.c\n");
    return ERROR_DINAMIC_MEMORY;
  }
  zips_passwords->num_threads = num_threads;

  // Generate the necessary data for the ZIP files
  error_code =
      generate_zip_file_data(zips_passwords, txt_data->num_of_zip_files,
                             txt_data->zip_files_directions);
  if (error_code == ERROR_DINAMIC_MEMORY) {
    if (zips_passwords) {
      free(zips_passwords);
    }
    return ERROR_DINAMIC_MEMORY;
  }

  struct thread_pass_search_info* thread_info =
      malloc(sizeof(*thread_info) * txt_data->num_of_zip_files);
  if (!thread_info) {
    fprintf(
        stderr,
        "Error: The program could not allocate dynamic memory for thread_info\n"
        "Function search_zip_passwords(): zip_handler.c\n");
    free_zips_passwords(zips_passwords, txt_data);
    return ERROR_DINAMIC_MEMORY;
  }
  for (uint64_t i = 0; i < txt_data->num_of_zip_files; i++) {
    thread_info[i].alphabet = txt_data->alphabet;
    thread_info[i].pass_is_found = &zips_passwords->zip_password_found[i];
    thread_info[i].password_file = NULL;
    thread_info[i].password_length = txt_data->max_password_length;
    thread_info[i].zip_file_dir = zips_passwords->zip_files_dir[i];
    thread_info[i].stat = ZIP_NOT_PROCESSED;
    thread_info[i].num_threads = zips_passwords->num_threads;

    find_password(&thread_info[i]);
  }

  for (uint64_t i = 0; i < txt_data->num_of_zip_files; i++) {
    if (*thread_info[i].pass_is_found) {
      printf("%s %s\n", thread_info[i].zip_file_dir,
             thread_info[i].password_file);
    } else {
      printf("%s\n", thread_info[i].zip_file_dir);
    }
  }

  for (uint64_t i = 0; i < txt_data->num_of_zip_files; i++) {
    if (thread_info[i].password_file) {
      free(thread_info[i].password_file);
    }
  }

  if (thread_info) {
    free(thread_info);
  }
  free_zips_passwords(zips_passwords, txt_data);

  return NO_ERROR;
}

void find_password(struct thread_pass_search_info* thread_info) {
  // Thread mutex
  pthread_mutex_t* mutex_pass = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(mutex_pass, NULL);

  enum test_code_stats* stat = malloc(sizeof(*stat));
  *stat = ZIP_NOT_PROCESSED;
  // This will check password if a zip file is finally processed.

  uint64_t pass_length_counter = 1;
  bool generate_more_password = true;
  // If a problem is found or a password is found, no more passwords
  // will be generated

  uint64_t* thread_id_found_pass = malloc(sizeof(*thread_id_found_pass));
  *thread_id_found_pass = 0;

  // Password generation was taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  while (pass_length_counter <= thread_info->password_length &&
         generate_more_password) {
    uint64_t total_combinations =
        pow_u(strlen(thread_info->alphabet), pass_length_counter);

    // Password will be generated
    char** passwords = malloc(total_combinations * sizeof(char*));
    generate_passwords(passwords, thread_info->alphabet, pass_length_counter);
    if (!passwords) {
      fprintf(stderr,
              "Error: Failed to allocate dynamic memory\n"
              "Function find_password() in password_handler.c\n");
      free(thread_id_found_pass);
      free(stat);
      pthread_mutex_destroy(mutex_pass);
      free(mutex_pass);
      return;
    }

    // Thread mapping
    // Static mapping will be used

    uint64_t amount_passwords_thread =
        floor(total_combinations / thread_info->num_threads);

    uint64_t num_threads = 0;
    if (thread_info->num_threads > total_combinations) {
      num_threads = total_combinations;

    } else {
      num_threads = thread_info->num_threads;
    }

    struct thread_test_passwords* thread_passwords =
        malloc(sizeof(*thread_passwords) * num_threads);
    pthread_t threads[num_threads];

    for (uint64_t i = 0; i < num_threads; i++) {
      thread_passwords[i].mutex_pass = mutex_pass;
      thread_passwords[i].pass_is_found = thread_info->pass_is_found;
      thread_passwords[i].password_file = NULL;
      thread_passwords[i].stat = stat;
      thread_passwords[i].thread_id = i;
      thread_passwords[i].thread_id_found_pass = thread_id_found_pass;
      thread_passwords[i].passwords = passwords;
      thread_passwords[i].zip_file_dir = thread_info->zip_file_dir;

      thread_passwords[i].start_index =
          i * amount_passwords_thread +
          min_val(i, mod_val(total_combinations, num_threads));
    }

    for (uint64_t i = 0; i < num_threads; i++) {
      if ((i + 1) < num_threads) {
        thread_passwords[i].finish_index = thread_passwords[i + 1].start_index;

      } else {
        thread_passwords[i].finish_index = total_combinations;
      }
    }
    /*for (uint64_t i = 0; i < num_threads; i++) {
      printf("%" PRIu64 "i: start index %" PRIu64 "\n", i,
             thread_passwords[i].start_index);
      printf("%" PRIu64 "i: finish index %" PRIu64 "\n", i,
             thread_passwords[i].finish_index);
    }*/
    for (uint64_t i = 0; i < num_threads; i++) {
      pthread_create(&threads[i], NULL, (void*)thread_test_passwords,
                     &thread_passwords[i]);
    }

    for (uint64_t i = 0; i < num_threads; i++) {
      // Wait for all threads to finish
      pthread_join(threads[i], NULL);
    }

    if (*stat != ZIP_NOT_PROCESSED) {
      generate_more_password = false;
    }

    if (*thread_info->pass_is_found) {
      thread_info->password_file = malloc(
          strlen(thread_passwords[(*thread_id_found_pass)].password_file) + 1);

      memcpy(thread_info->password_file,
             thread_passwords[(*thread_id_found_pass)].password_file,
             strlen(thread_passwords[*thread_id_found_pass].password_file) + 1);
    }

    for (uint64_t i = 0; i < num_threads; i++) {
      if (thread_passwords[i].password_file) {
        free(thread_passwords[i].password_file);
      }
    }

    free(thread_passwords);
    free_passwords(passwords, total_combinations);
    pass_length_counter++;
  }
  pthread_mutex_destroy(mutex_pass);
  free(mutex_pass);
  free(thread_id_found_pass);
  free(stat);
}

void free_zips_passwords(struct zip_files_passwords* zips_passwords,
                         txt_file_data* txt_data) {
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
