// Copyright 2023 Randy Aguero Bermudez
#ifndef COMMON_H
#define COMMON_H
/**
 * @file common.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief  * This file contains the declarations for methods of common functions
 * used in the program.
 *
 * @version 1.1
 * @date 2023-06-10
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

/**
 * @brief
 *
 * @param base Base of a power
 * @param exp Exponent of a power
 * @return uint64_t Return the result of the power
 */
uint64_t pow_u(uint64_t base, uint64_t exp);

/**
 * @brief Returns the minimum value between two numbers.
 *
 * @param dividend The dividend.
 * @param divisor The divisor.
 * @return uint64_t The minimum value between dividend and divisor.
 */
uint64_t min_val(uint64_t dividend, uint64_t divisor);

/**
 * @brief Returns the remainder of a division operation.
 *
 * @param dividend The dividend.
 * @param divisor The divisor.
 * @return uint64_t The remainder of the division operation.
 */
uint64_t mod_val(uint64_t dividend, uint64_t divisor);
#endif
