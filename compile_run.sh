#!/bin/bash
module add mpi/openmpi4-x86_64
mpicxx -std=c++0x main.cpp
sbatch ./run_sbatch_config.sh
