// Copyright 2023 Randy Aguero Bermudez
#ifndef PASSWORD_HANDLER_H
#define PASSWORD_HANDLER_H

/**
 * @file passwords_handler.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zip.h>

#include "common.h"
#include "file_handler.h"
#include "zip_handler.h"
#include "zippass_pthread.h"

struct zip_files_passwords;
struct thread_pass_gen_info;


struct passwords_data {
  char** generated_passwords;
  uint64_t password_length;
  char* alphabet;
  uint64_t num_passwords;
};

void free_generated_passwords(struct passwords_data* pass_data,
                              uint64_t pass_length);

enum program_error_code generate_zip_passwords(uint64_t pass_length,
                                               struct txt_file_data* txt_file,
                                               struct passwords_data* pass_data,
                                               uint32_t num_threads);

void* generate_passwords_list(uint64_t amount_passwords, uint32_t pass_length);

void generate_zip_basic_password(struct zip_files_passwords* zip_passwords,
                                 uint64_t num_zip_files, char** zip_dir);

void generate_passwords(struct thread_pass_gen_info* thread_info);

#endif