#!/bin/bash

for n in {3, 6, 8} do
  for p in {1..8} do
    sbatch -n $p --partition=RT --job-name="task1-$p-$n" --comment="task1" --output="out/out-$p-$n.txt" --error="error/error-$p-$n.txt" --wrap="mpiexec ./prog $p $n"
  done
done
