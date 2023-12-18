#!/bin/bash
# =================
# shared_1d.sh 
# =================

#SBATCH --job-name=fft_shared_1d
#SBATCH --partition=teach_cpu
#SBATCH --account=PHYS030385
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=28
#SBATCH --time=0:1:30
#SBATCH --mem=0

# Load modules required for runtime e.g.
module load languages/intel/2020-u4

cd $SLURM_SUBMIT_DIR

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK}

N=$1
# Now run your program with the usual command
../fft1Dshared $N