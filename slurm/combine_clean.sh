#!/bin/bash

# Extract and concatenate the third lines from all output files into a single CSV file
for file in slurm-*.out
do
    awk 'NR==3' "$file" >> cluster_hybrid_2d.csv
done

rm slurm-*.out