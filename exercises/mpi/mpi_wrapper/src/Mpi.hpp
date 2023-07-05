#ifndef MPI_HPP
#define MPI_HPP

#include <mpi.h>

#include <stdexcept>
#include <string>

#include "MpiError.hpp"

/**
 * @class Mpi
 * @brief Wrapper class for MPI functionality.
 */
class Mpi {
 public:
  /**
   * @brief Initializes the MPI environment and retrieves process information.
   * @param argc The number of command-line arguments.
   * @param argv The array of command-line arguments.
   */
  Mpi(int& argc, char**& argv) {
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
      throw MpiError("Failed to initialize MPI environment");
    }

    if (MPI_Comm_rank(MPI_COMM_WORLD, &processNumber) != MPI_SUCCESS) {
      throw MpiError("Failed to get process number", *this);
    }

    if (MPI_Comm_size(MPI_COMM_WORLD, &processCount) != MPI_SUCCESS) {
      throw MpiError("Failed to get process count", *this);
    }

    char host[256];
    int hostLen;
    if (MPI_Get_processor_name(host, &hostLen) != MPI_SUCCESS) {
      throw MpiError("Failed to get hostname", *this);
    }
    hostname = host;
  }
  /**
   * @brief Finalizes the MPI environment.
   */
  ~Mpi() { MPI_Finalize(); }

  /**
   * @brief Returns the process number of the current process.
   * @return The process number.
   */
  inline int getProcessNumber() const { return processNumber; }
  /**
   * @brief Returns the total number of processes.
   * @return The process count.
   */
  inline int getProcessCount() const { return processCount; }
  /**
   * @brief Returns the hostname of the current process.
   * @return The hostname.
   */

  inline const std::string& getHostname() const { return hostname; }

  /**
   * @brief Returns the process number of the current process.
   * @return The process number.
   */
  inline int rank() const { return processNumber; }

  /**
   * @brief Sends a value to a specific process.
   * @tparam T The type of the value to send.
   * @param value The value to send.
   * @param toProcess The process number to send the value to.
   * @param tag The tag to attach to the message (default: 0).
   */
  template <typename T>
  void send(const T& value, int toProcess, int tag = 0) {
    MPI_Datatype datatype = map(T());
    MPI_Send(&value, 1, datatype, toProcess, tag, MPI_COMM_WORLD);
  }

  /**
   * @brief Receives a value from a specific process.
   * @tparam T The type of the value to receive.
   * @param value The variable to store the received value.
   * @param fromProcess The process number to receive the value from.
   * @param tag The tag to match the received message (default: MPI_ANY_TAG).
   */

  template <typename T>
  void receive(T& value, int fromProcess, int tag = MPI_ANY_TAG) {
    MPI_Status status;
    MPI_Datatype datatype = map(T());
    MPI_Recv(&value, 1, datatype, fromProcess, tag, MPI_COMM_WORLD, &status);
  }

  /**
   * @brief Maps a C++ type to its corresponding MPI datatype.
   * @tparam T The C++ type to map.
   * @return The MPI datatype corresponding to the C++ type.
   */
  static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
  static inline MPI_Datatype map(char) { return MPI_CHAR; }
  static inline MPI_Datatype map(unsigned char) { return MPI_UNSIGNED_CHAR; }
  static inline MPI_Datatype map(short) { return MPI_SHORT; }
  static inline MPI_Datatype map(unsigned short) { return MPI_UNSIGNED_SHORT; }
  static inline MPI_Datatype map(int) { return MPI_INT; }
  static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
  static inline MPI_Datatype map(long) { return MPI_LONG; }
  static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
  static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
  static inline MPI_Datatype map(unsigned long long) {
    return MPI_UNSIGNED_LONG_LONG;
  }
  static inline MPI_Datatype map(float) { return MPI_FLOAT; }
  static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
  static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }

 private:
  int processNumber;    /** The process number of the current process.*/
  int processCount;     /** The total number of processes.*/
  std::string hostname; /** The hostname of the current process.*/
};

#endif  // MPI_HPP
