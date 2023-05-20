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

struct passwords_data;

enum error_zip_code {
  ZIP_PROCESSED_SUCESSFULLY,
  ZIP_FILE_NOT_READ,
  ZIP_DOES_NOT_EXIST,
  ZIP_HAS_NOT_ANY_PASSWORD,
  ZIP_IS_EMPTY,
  INVALID_FILE_DATA,
  FAILED_ALLOCATE_MEMORY
};

typedef struct zip_files_passwords {
  char** passwords;
  bool* zip_password_found;
} zip_files_passwords;

enum program_error_code search_zip_passwords(uint32_t num_threads,
                                             txt_data* txt_file);
#endif