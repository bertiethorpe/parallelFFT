#!/bin/bash

for ((bits=9; bits<=15; bits+=1))
do  
    N=$((2**bits))
    sbatch distributed_2d.sh "$N"

done

# Wait for all jobs to finish
wait

