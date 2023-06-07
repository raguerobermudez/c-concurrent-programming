// Copyright 2023 Randy Aguero Bermudez

#ifndef ZIP_HANDLER_H
#define ZIP_HANDLER_H
/**
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief This file declares all the functions that will be used to open a zip
 * file
 * @version 1.0
 * @date 2023-05-17
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

struct thread_test_password;
struct passwords_data;
struct thread_test_pass_stats;
struct txt_file_data;

typedef struct zip_files_passwords {
  char** files_passwords;
  pthread_mutex_t password_found_mutex;
  bool* zip_password_found;
  char** zip_files_dir;
} zip_files_passwords;

struct zip_file_test_data {
  struct zip* zip_data;
  uint64_t num_files;
  struct zip_stat file_stat;
};

enum program_error_code generate_zip_file_data(
    zip_files_passwords* zip_passwords, uint64_t num_zip_files, char** zip_dir);

void test_password(struct thread_test_pass_stats* test_data);


enum program_error_code search_zip_passwords(uint32_t num_threads,
                                             struct txt_file_data* txt_file);

void free_zips_passwords(struct zip_files_passwords* zips_passwords,
                        struct txt_file_data* txt_data);
#endif
