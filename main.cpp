#include "mpi.h"
#include <iostream>

double func(double x) {
  return 4. / (1 + x * x);
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int world_size = 0, world_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  size_t p = atoi(argv[1]);
  size_t n = atoi(argv[2]);
  std::cout << world_rank << " " << p << " " << n << std::endl;

  MPI_Finalize();
  return 0;
}
