#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <mpi.h>
#include "omp.h"

template <typename Func>
double Integrate(Func func, double* partition, size_t number_of_segments) {
  double result = 0;

  #pragma omp parallel for reduction (+:result)
  for (size_t i = 0; i < number_of_segments; ++i) {
    double dx = partition[i + 1] - partition[i];
    result += dx * (func(partition[i]) + func(partition[i + 1])) / 2;
  }
  return result;
}

int main(int argc, char** argv) {
  // MPI initialization
  MPI_Init(&argc, &argv);
  int world_size = 0, world_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  size_t p = atoi(argv[1]);
  size_t n_exp = atoi(argv[2]);
  size_t threads = atoi(argv[3]);
  size_t n = static_cast<size_t>(std::pow(10, n_exp));

  omp_set_num_threads(threads);
  
  auto func = [](double x) { return 4. / (1 + x * x); };
  size_t num_of_segments = n / p;
  double* partition = new double[n + 1];
  double* int_parts = new double[p];
  double result = 0, result_seq = 0;
  MPI_Status status;

  if (world_rank == 0) {
    // Partitioning the segment [0,1]
    for (size_t i = 0; i <= n; ++i) {
      partition[i] = static_cast<double>(i) / static_cast<double>(n);
    }

    // Distributed computation of the integral 
    double start_time = MPI_Wtime();
    for (size_t i = 0; i < p - 1; ++i) {
      MPI_Send(&partition[i * num_of_segments], num_of_segments + 1, 
        MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
    }
    int_parts[0] = Integrate(func, &partition[(p - 1) * num_of_segments], 
      n - (p - 1) * num_of_segments);
    for (size_t i = 1; i < p; ++i) {
      MPI_Recv(&int_parts[i], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
    }
    for (size_t i = 0; i < p; ++i) {
      result += int_parts[i];
    }
    double time = MPI_Wtime() - start_time;

    std::cout << "Часть интеграла, посчитанная процессом " << world_rank 
      << ": I_" << world_rank << " = " << int_parts[0] << std::endl;
    std::cout << "Значение интеграла, полученное сложением всех частей: I = " 
      << result << std::endl;

    // Logging time of computation
    std::ofstream log("logs/log-" + std::to_string(12 / threads) + ".txt");
    log << "k = " << 12 / threads << "\n" << "l = " << threads << "\n";
    log << "Time: " << time << "\n";
    log.close();
  } else {
    MPI_Recv(partition, num_of_segments + 1, MPI_DOUBLE, 0, 1, 
      MPI_COMM_WORLD, &status);
    int_parts[0] = Integrate(func, partition, num_of_segments);
    std::cout << "Часть интеграла, посчитанная процессом " << world_rank 
      << ": I_" << world_rank << " = " << int_parts[0] << std::endl;
    MPI_Send(int_parts, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }

  delete[] partition;
  delete[] int_parts;
  MPI_Finalize();
  return 0;
}

