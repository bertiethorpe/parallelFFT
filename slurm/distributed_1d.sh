#!/bin/bash
# ================ 
# distributed_1d.sh 
# ================

#SBATCH --job-name=distributed_1d
#SBATCH --partition=teach_cpu
#SBATCH --account=PHYS030385
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=0:5:00
#SBATCH --mem=0

# Load modules required for runtime e.g.
module load languages/intel/2020-u4

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK}

cd $SLURM_SUBMIT_DIR

N=$1

srun --mpi=pmi2 ../fft1Ddistributed $N

