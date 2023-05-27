#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

struct shared_data_t {
  uint64_t thread_count;
};

struct private_data_t {
  uint64_t thread_number;
  std::shared_ptr<shared_data_t> shared_data;
};

void greet(private_data_t* data);
void create_threads(std::shared_ptr<shared_data_t> shared_data);

int main(int argc, char* argv[]) {
  try {
    uint64_t thread_count = std::thread::hardware_concurrency();

    if (argc == 2) {
      std::istringstream iss(argv[1]);
      if (!(iss >> thread_count)) {
        throw std::invalid_argument("Error: invalid thread count");
      }
    }

    std::shared_ptr<shared_data_t> shared_data =
        std::make_shared<shared_data_t>();
    shared_data->thread_count = thread_count;

    auto start_time = std::chrono::high_resolution_clock::now();

    create_threads(shared_data);

    auto finish_time = std::chrono::high_resolution_clock::now();
    double elapsed_time =
        std::chrono::duration_cast<std::chrono::duration<double>>(finish_time -
                                                                  start_time)
            .count();

    std::cout << "Execution time: " << elapsed_time << "s" << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception& ex) {
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
}

void greet(private_data_t* data) {
  private_data_t* private_data = data;
  shared_data_t* shared_data = private_data->shared_data.get();

  // Imprimir "Hello from secondary thread"
  std::cout << "Hello from secondary thread " << private_data->thread_number
            << " of " << shared_data->thread_count << std::endl;
}

void create_threads(std::shared_ptr<shared_data_t> shared_data) {
  std::vector<std::thread> threads;
  threads.reserve(shared_data->thread_count);
  std::vector<private_data_t> private_data(shared_data->thread_count);

  for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
       ++thread_number) {
    private_data[thread_number].thread_number = thread_number;
    private_data[thread_number].shared_data = shared_data;

    threads.emplace_back(greet, &private_data[thread_number]);
  }

  std::cout << "Hello from main thread" << std::endl;

  for (auto& thread : threads) {
    thread.join();
  }
}
