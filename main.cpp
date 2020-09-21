#include <iostream>
#include <stdlib.h>
#include <mpi.h>

template <typename Func>
double Integrate(Func func, double from, double to, size_t n) {
  double dx = (to - from) / n;
  double integral = 0;
  for (size_t i = 0; i < n - 1; ++i) {
    integral += dx * (func(from + i * dx) + func(from + (i + 1) * dx)) / 2;
  }
  return integral;
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int world_size = 0, world_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  size_t p = atoi(argv[1]);
  size_t n = atoi(argv[2]);
  
  size_t start_point = world_rank * n / p;
  size_t end_point = (world_rank == p - 1 ? n : start_point + n / p);
  

  if (world_rank == 0) {
    double integral_0 = Integrate([](double x) { return 4. / (1 + x * x); }, 0, 1, n);
    std::cout << "I = " << integral << std::endl;
    std::cout << "I_0 = " << integral_0 << std::endl;
  }

  MPI_Finalize();
  return 0;
}
