#!/bin/bash

for ((bits=10; bits<=29; bits+=1))
do  
    N=$((2**bits))
    sbatch distributed_1d.sh "$N"
done

# Wait for all jobs to finish
wait

