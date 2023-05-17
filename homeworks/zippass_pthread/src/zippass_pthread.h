/**
 * @file zippass_serial.h
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief This file declares all the functions that will be used to open a zip
 * file
 * @version 1.0
 * @date 2023-04-12
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
#include <unistd.h>
#include <zip.h>

#define MAX_LINE_LENGHT 2048
#define MAX_NUMBER_ZIP_FILES 100

enum error_code {
  ZIP_PROCESSED_SUCESSFULLY,
  ZIP_FILE_NOT_READ,
  ZIP_DOES_NOT_EXIST,
  ZIP_HAS_NOT_ANY_PASSWORD,
  ZIP_IS_EMPTY,
  INVALID_FILE_DATA,
  FAILED_ALLOCATE_MEMORY
};

typedef struct password_test_codes {
  char* password;
  enum error_code error_code;
} test_code;

typedef struct txt_file_data {
  char* alphabet;
  uint64_t max_password_length;
  uint64_t num_of_zip_files;
  char** zip_files_directions;
} txt_data;

typedef struct threads_shared_info {
  char* zip_dir;
  char* password_found;
  bool continue_test_password;
  pthread_mutex_t mutex;
  } threads_shared_info;

typedef struct test_password_info{
  char* password;
  char* zip_dir;
  test_code password_test_code;
}test_password_info;  

typedef struct thread_pass_gen_info {
  char** passwords;
  uint64_t amount_password;
  char* alphabet;
  uint64_t pos_char;
  uint64_t interval;
} thread_pass_gen_info;




/**
 * @brief
 *
 * @param base Base of a power
 * @param exp Exponent of a power
 * @return uint64_t Return the result of the power
 */
uint64_t pow_u(uint64_t base, uint64_t exp);

/**
 * @brief read_txt_file processes txt file
 *
 * @param file A txt file gived as a program argument
 * @param file_data A variable of type struct txt_file_data
 * @return true If the txt file can be sucesfully read
 * @return false If the txt file is not a valid txt file
 */
bool read_txt_file(char* file, txt_data* file_data);

/**
 * @brief generate_zip_password uses a force-brute algorithm to find the correct
 * password for a zip file
 *
 * @param password_lenght The maximum posible length of a password
 * @param ALPHABET Posible characters that can be used in a password
 * @param zip_dir The directory of the zip file to be opened
 */
void generate_zip_password(uint64_t* password_lenght, const char* ALPHABET,
                           const char* zip_dir, const uint64_t num_threads);


test_code test_password_zip_file(test_password_info pass_info);
