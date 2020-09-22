all:
	module add mpi/openmpi4-x86_64
	mpicxx -std=c++0x -o prog main.cpp