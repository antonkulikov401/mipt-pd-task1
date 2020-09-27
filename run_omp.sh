#!/bin/bash

for k in 2 3 4 6 
do
  l=$((12 / $k))
  sbatch -n $k --partition=RT --job-name="task1-$k" --comment="task1-omp" --output="out/out-$k.txt" --error="error/error-$k.txt" --wrap="mpiexec ./prog_omp $k 8 $l"
done
