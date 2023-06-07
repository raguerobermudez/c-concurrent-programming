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
struct thread_pass_search_info;

struct passwords_data {
  uint64_t password_length;
  char* alphabet;
  uint64_t num_passwords;
};

void find_password(struct thread_pass_search_info* thread_info);

#endif