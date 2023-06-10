// Copyright 2023 Randy Aguero Bermudez

#include "common.h"

uint64_t pow_u(uint64_t base, uint64_t exp) {
  // Example taken from
  // https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition
  uint64_t result = 1;
  for (uint64_t i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

uint64_t min_val(uint64_t value_1, uint64_t value_2){
  if(value_1<value_2){
    return value_1;
  }else{
    return value_2;
  }
};

uint64_t mod_val(uint64_t dividend, uint64_t divisor) {
  if (divisor == 0) {
    return 0;  
  }
  uint64_t quotient = dividend / divisor;
  uint64_t remainder = dividend - (quotient * divisor);
  return remainder;
}
