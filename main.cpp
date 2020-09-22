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
  double int_part = 0;
  MPI_Status status;

  if (world_rank == 0) {
    for (size_t i = 0; i <= n; ++i) {
      partition[i] = static_cast<double>(i) / static_cast<double>(n);
    }

    for (size_t i = 1; i < p; ++i) {
      MPI_Send(&partition[i * num_of_segments], num_of_segments + 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(partition, num_of_segments + 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    int_part = Integrate(func, partition, num_of_segments);
    std::cout << "I_" << world_rank << " = " << int_part << std::endl;
  }

  delete[] partition;
  MPI_Finalize();
  return 0;
}
