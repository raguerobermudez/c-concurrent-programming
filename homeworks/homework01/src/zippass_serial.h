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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zip.h>

#define MAX_LINE_LENGHT 2048
#define MAX_NUMBER_ZIP_FILES 100

typedef struct txt_file_data {
  FILE* file;
  char* alphabet;
  uint64_t MAX_PASSWORD_LENGTH;
  uint64_t* num_of_zip_files;
  char** zip_files_directions;
} txt_data;

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
 * @return true if a password has been found
 * @return false if a password has not been found
 */
bool generate_zip_password(uint64_t* password_lenght, const char* ALPHABET,
                           const char* zip_dir);

/**
 * @brief test_zip_password tries to open a file using a given password
 *
 * @param password A password that will be used to try to open a zip file
 * @param zip_dir The directory of the zip file to be opened
 * @return true if a password is correct
 * @return false if the password in incorrect
 */
bool test_zip_password(const char* password, const char* zip_dir);
