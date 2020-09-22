#!/bin/bash

for p in {1..4} 
do
  sbatch -n $p --partition=RT --job-name="task1-p=$p" --output="out$p.txt" --error="error$p.txt" ./exec.sh $p 1000
done
