all:
	mpicxx -std=c++0x -o prog main.cpp
	mpicxx -std=c++0x -fopenmp -o prog_omp main_omp.cpp