#ifndef MPI_ERROR_HPP
#define MPI_ERROR_HPP

#include <stdexcept>
#include <string>


class Mpi;
/**
 * @class MpiError
 * @brief Exception class for MPI errors.
 */

class MpiError : public std::runtime_error {
 public:
  /**
   * @brief Constructs an MpiError object with the given error message.
   * @param message The error message.
   */
  explicit MpiError(const std::string& message) : std::runtime_error(message) {}
  
  /**
   * @brief Constructs an MpiError object with the given error message and MPI
   * context.
   * @param message The error message.
   * @param mpi The MPI context where the error occurred.
   */

  MpiError(const std::string& message, const Mpi& mpi);
  /**
   * @brief Constructs an MpiError object with the given error message, MPI
   * context, and thread number.
   * @param message The error message.
   * @param mpi The MPI context where the error occurred.
   * @param threadNumber The thread number where the error occurred.
   */
  MpiError(const std::string& message, const Mpi& mpi, const int threadNumber);
};

#endif  // MPI_ERROR_HPP
