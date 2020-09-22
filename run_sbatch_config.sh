#!/bin/bash
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --partition=RT
#SBATCH --job-name=task1
#SBATCH --comment="task 1"
#SBATCH --output=out.txt
#SBATCH --error=error.txt
mpiexec ./a.out 4 1000
