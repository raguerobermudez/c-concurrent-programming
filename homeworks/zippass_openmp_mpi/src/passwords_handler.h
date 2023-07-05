// Copyright 2023 Randy Aguero Bermudez
#ifndef PASSWORD_HANDLER_H
#define PASSWORD_HANDLER_H

/**
 * @file passwords_handler.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <semaphore.h>
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

struct thread_pass_search_info;

/**
 * @brief  This function generates passwords by combining characters from a
 * given alphabet to form passwords of a specified length.
 *
 * @param passwords Array to store the generated passwords.
 * @param alphabet The characters to use for password generation.
 * @param password_length The length of each generated password.
 */
void generate_passwords(char** passwords, char* alphabet,
                        uint64_t password_length);

/**
 * @brief Free the allocated memory for an array of passwords.
 *
 * @param passwords Array of passwords.
 * @param amount_passwords Number of passwords in the array.
 */
void free_passwords(char** passwords, uint64_t amout_passwords);

#endif
