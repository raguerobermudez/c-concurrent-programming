// Copyright 2023 Randy Aguero Bermudez
#ifndef COMMON_H
#define COMMON_H
/**
 * @file common_handler.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-04-12
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
uint64_t pow_u(uint64_t base, uint64_t exp) {
  // Example taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  uint64_t result = 1;
  for (uint64_t i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}
#endif