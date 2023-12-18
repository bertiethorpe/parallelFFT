# parallelFFT
Parallel Fast Fourier Transforms

# Overview

This program is an implementation of the radix-2 Cooley-Tukey Fast Fourier 
Transform (FFT) algorithm, parallelised to run on both shared memory multicore, 
and distributed memory multisocket architectures. Development and testing was
completed on the University of Bristol's Blue Crystal Phase 4 supercomputer. As
such, it is not designed for universal application and mileage will vary.

For further information on Blue Crystal, consult the ACRC pages here:

https://www.acrc.bris.ac.uk/acrc/phase4.htm

The program is divided into two strands:

1. Shared memory systems with single or dual socket CPUs.
2. Distributed memory systems across multiple sockets/nodes.

Both strands support both 1D and 2D FFTs, and four 
executables are provided to cover various scenarios:

1. Shared Memory OpenMP 1D FFT (`fft1Dshared`)
2. Shared Memory OpenMP 2D FFT (`fft2Dshared`)
3. Distributed Memory 1D FFT (`fft1Ddistributed`)
    - Both pure MPI and Hybrid MPI/OpenMP
4. Distributed Memory 2D FFT (`fft2Ddistributed`)
    - Both pure MPI and Hybrid MPI/OpenMP

The purpose of the program is to benchmark parallelisation of only the most 
rudimentary Cooley-Tukey algorithm so for simplification, only works with square 
inputs of power-of-2 sizes. I may extend it in the future.

# Compilation

Compile the program using the provided Makefile. Ensure the directory hierarchy
is not changed as this will mess up the make process.

.obj files are output at the location of the source files and linked to each of
the 4 executables. Each of the 1D and 2D FFT have their own unique dependencies.
All implementations have a dependency of the fft_1d.c file and all the utility 
files.

# Structure

To understand the structure of the source files, insight into the FFT algorithm
is useful. There are many different versions, each with their own pros and cons.

The 1D FFT algorithm deployed here is made up of key steps:

1. Bit reversal of the input array
2. Precomputation of the twiddle factors
3. iterative radix-2 butterfly operations using twiddle factors

The 2D FFT has some extra steps:

1. Precomputation of the bitreversed indices of N sized array
2. Precomputation of the twiddle factors
3. Indexing of the input array into rows
4. Bit-reverse reordering of the input rows using precomputed indices
5. Butterfly operations on each of these rows
6. Recombination of the 2D input 
7. Transpose the 2D array.
7. Repeat steps 3. - 6.

# SLURM

Inside the slurm directory are all the scripts needed to benchmark the FFT 
executables on BlueCrystal. From these scripts the tuning of MPI nodes/tasks 
and OMP threads is done. Note that due to the limitations of the MPI 
implementation, only power of 2 amount of ranks are possible to implement, 
equating to 16 cores being useable on BlueCrystal. For the hybrid 
implementation, it is important not to oversubscribe the amount of tasks, 
nodes x ntasks x num_cpus should not exceed the total amount of cores available 
on the system partition.
