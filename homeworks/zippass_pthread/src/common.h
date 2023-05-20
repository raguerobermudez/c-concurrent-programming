// Copyright 2023 Randy Aguero Bermudez
#ifndef COMMON_H
#define COMMON_H
/**
 * @file common.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
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
#include <unistd.h>

enum program_error_code {
  INVALID_ARGUMENTS,
  INVALID_TXT_FILE,
  INVALID_ZIP_BASIC_PASSWORD,
  INSUFFICIENT_PASSWORDS_MEMORY,
  INSUFFICIENTE_THREAD_PASSWORD_MEMORY,
  INSUFFICIENTE_DINAMIC_MEMORY,
  NO_ERROR
};

/**
 * @brief
 *
 * @param base Base of a power
 * @param exp Exponent of a power
 * @return uint64_t Return the result of the power
 */
uint64_t  pow_u(uint64_t base, uint64_t exp);
#endif