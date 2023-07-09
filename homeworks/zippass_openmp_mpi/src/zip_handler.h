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
 * @brief Finds the password for a zip file in a separate thread
 *
 * @param thread_info The thread password search info
 */
void find_password(struct thread_pass_search_info* thread_info);
#endif
