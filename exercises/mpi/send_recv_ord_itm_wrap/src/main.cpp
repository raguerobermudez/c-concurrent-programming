#include <iostream>

#include "Mpi.hpp"

int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);
    int processNumber = mpi.getProcessNumber();
    int processCount = mpi.getProcessCount();
    std::string hostname = mpi.getHostname();

    std::cout << "Process number: " << processNumber << std::endl;
    std::cout << "Process count: " << processCount << std::endl;
    std::cout << "Hostname: " << hostname << std::endl;

    //values
    int valueToSend = 42;
    int receivedValue;

    if (processNumber == 0) {
      mpi.send(valueToSend, 1);
    } else if (processNumber == 1) {
      mpi.receive(receivedValue, 0);
      std::cout << "Received value: " << receivedValue << std::endl;
    }

    //vectors
    if (processNumber == 0) {
      std::vector<int> vectorToSend = {8, 9, 10, 11, 12};
      mpi.send(vectorToSend, 1);
    } else if (processNumber == 1) {
      std::vector<int> receivedVector;
      mpi.receive(receivedVector, 0);
      std::cout << "Received vector: ";
      for (int value : receivedVector) {
        std::cout << value << " ";
      }
      std::cout << std::endl;
    }

    //strings
    if (processNumber == 0) {
      std::string textToSend = "Hello";
      mpi.send(textToSend, 1);
    } else if (processNumber == 1) {
      std::string receivedText;
      mpi.receive(receivedText, 0);
      std::cout << "Received text: " << receivedText << std::endl;
    }

  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
