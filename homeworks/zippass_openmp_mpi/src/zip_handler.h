// Copyright 2023 Randy Aguero Bermudez

#ifndef ZIP_HANDLER_H
#define ZIP_HANDLER_H
/**
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief This file declares all the functions that will be used to open a zip
 * file
 * @version 1.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zip.h>

#include "common.h"
#include "file_handler.h"
#include "passwords_handler.h"
#include "zippass_pthread.h"

struct thread_pass_search_info;
struct thread_test_pass_stats;
struct txt_file_data;
struct process_zip_data;

typedef struct zip_files_passwords {
  char** files_passwords;                // Array of file passwords
  pthread_mutex_t password_found_mutex;  // Mutex for protecting the
                                         // zip_password_found variable
  bool* zip_password_found;  // Boolean variable indicating if the zip password
                             // was found
  char** zip_files_dir;      // Array of zip file directories
  uint32_t num_threads;      //  Number of threads
} zip_files_passwords;

/**
 * @brief Generates the data needed for zip files
 *
 * @param zip_passwords Pointer to the zip_files_passwords structure to store
 * the generated data.
 * @param num_zip_files Number of ZIP files.
 * @param zip_dir Array of ZIP file directories.
 * @return enum program_error_code Error code indicating the status of the
 * operation.
 */
enum program_error_code generate_zip_file_data(
    zip_files_passwords* zip_passwords, uint64_t num_zip_files, char* zip_dir);

/**
 * @brief Tests a password for a zip file
 *
 * @param test_data The test password data
 */
void test_password(struct thread_test_pass_stats* test_data);

/**
 * @brief Searches for passwords in zip files
 *
 * @param num_threads Number of threads to use for password search.
 * @param txt_data Pointer to the txt_file_data structure containing input data.
 * @return Error code indicating the success or failure of the function.
 */

enum program_error_code search_zip_passwords(uint32_t num_threads,
                                             struct process_zip_data* txt_file);

/**
 * @brief Frees the allocated memory for zip files passwords data
 *
 * @param zips_passwords The zip files passwords structure
 * @param txt_data The text file data
 */
void free_zips_passwords(struct zip_files_passwords* zips_passwords,
                         struct process_zip_data* txt_data);

/**
 * @brief Finds the password for a zip file in a separate thread
 *
 * @param thread_info The thread password search info
 */
void find_password(struct thread_pass_search_info* thread_info);
#endif
