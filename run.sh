#!/bin/bash

for p in {1..4} 
do
  sbatch -n $p --partition=RT --job-name="task1-p=$p" --comment="task1" --output="out$p.txt" --error="error$p.txt" --wrap="mpiexec ./prog $p 1000"
done
