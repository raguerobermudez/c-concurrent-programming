// Copyright 2023 Randy Aguero Bermudez

/**
 * @file zippass_pthread.c
 * @author Randy Jossué Agüero Bermúdez B90082 randy.aguero@ucr.ac.cr
 * @brief
 *
 * @version 1.0
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "zippass_pthread.h"

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file_handler.h"
#include "zip_handler.h"

int main(int argc, char* argv[]) {
  int rank, size;
  program_error_code error_code = 0;

  // Inicializar MPI
  MPI_Init(&argc, &argv);

  // Obtener el rango y el tamaño del comunicador
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char filename[MAX_LINE_LENGTH];

  // Solo el proceso raíz (rango 0) manejará la entrada
  if (rank == 0) {
    printf("Enter the filename: ");
    scanf("%s", filename);
    getchar();  // Consumir el carácter de nueva línea del búfer de entrada
  }

  // Compartir el nombre del archivo a todos los procesos
  MPI_Bcast(filename, MAX_LINE_LENGTH, MPI_CHAR, 0, MPI_COMM_WORLD);

  uint32_t number_threads = 0;
  if (rank == 0) {
    printf("Enter the number of threads: ");
    scanf("%u", &number_threads);
    getchar();  // Consumir el carácter de nueva línea del búfer de entrada
  }

  // Compartir el número de hilos a todos los procesos
  MPI_Bcast(&number_threads, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

  // Input verification

  if (number_threads < 1) {
    fprintf(stderr,
            "Error, The number of threads must be greater than 0, Default "
            "system number of threads will be used\n");
    // If the number of threads is less than 1, use the default number of
    // threads provided by the system
    number_threads = sysconf(_SC_NPROCESSORS_ONLN);
  }

  // Input file read

  txt_file_data txt_file;
  if (!read_txt_file(filename, &txt_file)) {
    error_code = INVALID_TXT_FILE;
    MPI_Finalize();
    return error_code;
  }

  // Dividir la cantidad de archivos entre los procesos
  uint64_t num_files_per_process = txt_file.num_of_zip_files / size;
  uint64_t start_entry = rank * num_files_per_process;
  uint64_t end_entry = (rank == size - 1) ? txt_file.num_of_zip_files
                                          : start_entry + num_files_per_process;

  // Search for passwords

  for (uint64_t i = start_entry; i < end_entry; i++) {
    error_code = search_zip_passwords(number_threads, &txt_file, i);
  }

  // Recopilar los resultados de todos los procesos
  MPI_Barrier(MPI_COMM_WORLD);
  system("clear");
  if (rank == 0) {
    for (uint64_t i = 0; i < txt_file.num_of_zip_files; i++) {
     if(txt_file.zip_passwords[i]){
      printf("%s %s\n", txt_file.zip_files_directions[i], txt_file.zip_passwords[i]);
     }else{
      printf("%s\n", txt_file.zip_files_directions[i]);
     }
    }
  }

  for (uint64_t i = 0; i < txt_file.num_of_zip_files; i++) {
    if (txt_file.zip_files_directions[i]) {
      free(txt_file.zip_files_directions[i]);
    }
    if(txt_file.zip_passwords[i]){
      free(txt_file.zip_passwords[i]);
    }
  }
  if (txt_file.zip_files_directions) {
    free(txt_file.zip_files_directions);
  }

  if (txt_file.alphabet) {
    free(txt_file.alphabet);
  }

  // Finalizar MPI
  MPI_Finalize();

  return error_code;
}

void thread_test_passwords(struct thread_test_passwords* test_passwords) {
  bool* pass_is_found = malloc(sizeof(*pass_is_found));
  *pass_is_found = false;
  struct thread_pass_test* pass_test = malloc(sizeof(*pass_test));
  pass_test->mutex_pass = test_passwords->mutex_pass;
  pass_test->pass_is_found = pass_is_found;
  pass_test->password_file = NULL;
  pass_test->stat = test_passwords->stat;
  pass_test->zip_file_dir = test_passwords->zip_file_dir;
  pass_test->passwords = test_passwords->passwords;
  pass_test->pass_counter = test_passwords->pass_counter;
  pass_test->total_passwords = test_passwords->total_passwords;
  // uint64_t password_counter = test_passwords->start_index;
  open_file(pass_test);

  if (*pass_test->pass_is_found) {
#pragma omp critical
    {
      *test_passwords->thread_id_found_pass = test_passwords->thread_id;
      test_passwords->password_file =
          malloc(strlen(pass_test->password_file) + 1);
      memcpy(test_passwords->password_file, pass_test->password_file,
             strlen(pass_test->password_file) + 1);
      *test_passwords->pass_is_found = true;
    }
  }

  if (pass_test->password_file) {
    free(pass_test->password_file);
  }
  free(pass_is_found);
  free(pass_test);
}
