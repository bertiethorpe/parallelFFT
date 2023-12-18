#!/bin/bash
# ================ 
# distributed_2d.sh 
# ================

#SBATCH --job-name=distributed_2d
#SBATCH --partition=teach_cpu
#SBATCH --account=PHYS030385
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --time=0:5:00
#SBATCH --mem=0

# Load modules required for runtime e.g.
module load languages/intel/2020-u4

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK}

cd $SLURM_SUBMIT_DIR

N=$1

srun --mpi=pmi2 ../fft2Ddistributed $N

