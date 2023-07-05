#ifndef MPI_HPP
#define MPI_HPP

#include <mpi.h>

#include <stdexcept>
#include <string>
#include <vector>

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
   * @tparam DataType The type of the value to send.
   * @param value The value to send.
   * @param toProcess The destination process number.
   * @param tag The message tag (default: 0).
   */

  template <typename DataType>
  void send(const DataType& value, int toProcess, int tag = 0) {
    MPI_Datatype datatype = map(DataType());
    MPI_Send(&value, 1, datatype, toProcess, tag, MPI_COMM_WORLD);
  }

  /**
   * @brief Sends an array of values to a specific process.
   * @tparam DataType The type of the values to send.
   * @param values The array of values to send.
   * @param count The number of values in the array.
   * @param toProcess The destination process number.
   * @param tag The message tag (default: 0).
   */

  template <typename DataType>
  void send(const DataType* values, int count, int toProcess, int tag = 0) {
    MPI_Datatype datatype = map(DataType());
    MPI_Send(values, count, datatype, toProcess, tag, MPI_COMM_WORLD);
  }

  /**
   * @brief Sends a vector of values to a specific process.
   * @tparam DataType The type of the values to send.
   * @param values The vector of values to send.
   * @param toProcess The destination process number.
   * @param tag The message tag (default: 0).
   */
  template <typename DataType>
  void send(const std::vector<DataType>& values, int toProcess, int tag = 0) {
    int count = static_cast<int>(values.size());
    MPI_Send(&count, 1, MPI_INT, toProcess, tag, MPI_COMM_WORLD);
    MPI_Send(values.data(), count, map(DataType()), toProcess, tag,
             MPI_COMM_WORLD);
  }

  /**
   * @brief Sends a string to a specific process.
   * @param text The string to send.
   * @param toProcess The destination process number.
   * @param tag The message tag (default: 0).
   */

  void send(const std::string& text, int toProcess, int tag = 0) {
    int count = static_cast<int>(text.size());
    MPI_Send(text.data(), count, MPI_CHAR, toProcess, tag, MPI_COMM_WORLD);
  }

  /**
   * @brief Receives a value from a specific process.
   * @tparam DataType The type of the value to receive.
   * @param value The received value.
   * @param fromProcess The source process number (default: MPI_ANY_SOURCE).
   * @param tag The message tag (default: MPI_ANY_TAG).
   */

  template <typename DataType>
  void receive(DataType& value, int fromProcess = MPI_ANY_SOURCE,
               int tag = MPI_ANY_TAG) {
    MPI_Status status;
    MPI_Datatype datatype = map(DataType());
    MPI_Recv(&value, 1, datatype, fromProcess, tag, MPI_COMM_WORLD, &status);
  }

  /**
   * @brief Receives an array of values from a specific process.
   * @tparam DataType The type of the values to receive.
   * @param values The received array of values.
   * @param capacity The maximum capacity of the array.
   * @param fromProcess The source process number (default: MPI_ANY_SOURCE).
   * @param tag The message tag (default: MPI_ANY_TAG).
   */

  template <typename DataType>
  void receive(DataType* values, int capacity, int fromProcess = MPI_ANY_SOURCE,
               int tag = MPI_ANY_TAG) {
    MPI_Status status;
    MPI_Datatype datatype = map(DataType());
    MPI_Recv(values, capacity, datatype, fromProcess, tag, MPI_COMM_WORLD,
             &status);
  }

  /**
   * @brief Receives a vector of values from a specific process.
   * @tparam DataType The type of the values to receive.
   * @param values The received vector of values.
   * @param fromProcess The source process number (default: MPI_ANY_SOURCE).
   * @param tag The message tag (default: MPI_ANY_TAG).
   */

  template <typename DataType>
  void receive(std::vector<DataType>& values, int fromProcess = MPI_ANY_SOURCE,
               int tag = MPI_ANY_TAG) {
    MPI_Status status;
    int count;
    MPI_Recv(&count, 1, MPI_INT, fromProcess, tag, MPI_COMM_WORLD, &status);
    values.resize(count);
    MPI_Recv(values.data(), count, map(DataType()), fromProcess, tag,
             MPI_COMM_WORLD, &status);
  }

  /**
   * @brief Receives a string from a specific process.
   * @param text The received string.
   * @param fromProcess The source process number (default: MPI_ANY_SOURCE).
   * @param tag The message tag (default: MPI_ANY_TAG).
   */

  void receive(std::string& text, int fromProcess = MPI_ANY_SOURCE,
               int tag = MPI_ANY_TAG) {
    MPI_Status status;
    int count;
    MPI_Probe(fromProcess, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &count);
    text.resize(count);
    MPI_Recv(&text[0], count, MPI_CHAR, fromProcess, tag, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
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
