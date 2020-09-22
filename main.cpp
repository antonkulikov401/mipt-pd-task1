#include <iostream>
#include <stdlib.h>
#include <mpi.h>

template <typename Func>
double Integrate(Func func, double* partition, size_t number_of_segments) {
  double result = 0;
  for (size_t i = 0; i < number_of_segments; ++i) {
    double dx = partition[i + 1] - partition[i];
    result += dx * (func(partition[i]) + func(partition[i + 1])) / 2;
  }
  return result;
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int world_size = 0, world_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  size_t p = atoi(argv[1]);
  size_t n = atoi(argv[2]);
  
  auto func = [](double x) { return 4. / (1 + x * x); };
  size_t num_of_segments = n / p;
  double* partition = new double[n + 1];
  double* int_parts = new double[p];
  double result = 0, result_seq = 0;
  MPI_Status status;

  if (world_rank == 0) {
    for (size_t i = 0; i <= n; ++i) {
      partition[i] = static_cast<double>(i) / static_cast<double>(n);
    }
    for (size_t i = 0; i < p - 1; ++i) {
      MPI_Send(&partition[i * num_of_segments], num_of_segments + 1, MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
    }
    int_parts[0] = Integrate(func, &partition[(p - 1) * num_of_segments], n - (p - 1) * num_of_segments);
    std::cout << "I_" << world_rank << " = " << int_parts[0] << std::endl;
    for (size_t i = 1; i < p; ++i) {
      MPI_Recv(&int_parts[i], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
    }
    for (size_t i = 0; i < p; ++i) {
      result += int_parts[i];
    }
    result_seq = Integrate(func, partition, n);
    std::cout << "I = " << result << std::endl;
    std::cout << "I_seq = " << result_seq << std::endl;
  } else {
    MPI_Recv(partition, num_of_segments + 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    int_parts[0] = Integrate(func, partition, num_of_segments);
    std::cout << "I_" << world_rank << " = " << int_parts[0] << std::endl;
    MPI_Send(int_parts, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }

  delete[] partition;
  delete[] int_parts;
  MPI_Finalize();
  return 0;
}
