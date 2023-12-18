#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#include "../utils/utils.h"
#include "fft_1d.h"

// Serial and OpenMP Radix-2 Cooley-Tukey iterative FFT using precomputed 
// twiddle factors and bit-reversed indices
void fft1d(double complex data[], int N, double complex* twiddleFactors, int par) {
    
    for (int stage = 1; stage <= (int)log2(N); stage++) {
        int stageSize = 1 << stage;
        int halfStageSize = stageSize / 2;
        int twiddleStep = N / stageSize;

        #pragma omp parallel for if(par)
        for (int offset = 0; offset < N; offset += stageSize) {
            for (int k = 0; k < halfStageSize; k++) {
                double complex t = twiddleFactors[twiddleStep * k] * data[offset + k + halfStageSize];
                data[offset + k + halfStageSize] = data[offset + k] - t;
                data[offset + k] = data[offset + k] + t;
            }
        }
    }
}

// function required for both pure MPI and hybrid MPI/OpenMP distributed
// memory 1D FFT
void fft1d_MPI(
    double complex data[], int local_size, int start_stage, int end_stage, 
    int N, double complex* twiddleFactors, int par, MPI_Comm comm) {
    
    for (int stage = start_stage; stage <= end_stage; stage++) {
        int stageSize = 1 << stage;
        int halfStageSize = stageSize / 2;
        int twiddleStep = N / stageSize;

        #pragma omp parallel for if(par)
        for (int offset = 0; offset < local_size; offset += stageSize) {
            for (int k = 0; k < halfStageSize; k++) {
                double complex t = twiddleFactors[twiddleStep * k] * data[offset + k + halfStageSize];
                data[offset + k + halfStageSize] = data[offset + k] - t;
                data[offset + k] = data[offset + k] + t;
            }
        }
    }
}