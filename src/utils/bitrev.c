#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <complex.h>
#include <omp.h>
#include <mpi.h>

#include "utils.h"

// Function to reverse the bits of a given number
uint32_t reverseBits(uint32_t num, int bits) {

    uint32_t result = 0;
    for (int i = 0; i < bits; i++) {
        result = (result << 1) | (num & 1);
        num >>= 1;
    }
    return result;
}

// Precompute bit-reversed indices of N-sized array
// For 1D fft this doesn't make sense but will be crucial for higher dimensions,
// where we will reuse the indices.
int *precomputeBitReverseIndices(int N, int bits, int par) {

    int *indices = (int*)malloc(N * sizeof(int));

    #pragma omp parallel for if(par)
    for (int i = 0; i < N; i++) {
        indices[i] = reverseBits(i, bits);
    }
    return indices;
}

int* bitRevIndMPI(int local_size, int rank, int bits, int par, MPI_Comm comm) {

    int *local_indices = (int*)malloc(local_size * sizeof(int));
    #pragma omp parallel for if(par)
    for (int i = rank * local_size; i < (rank + 1) * local_size; i++) {
        local_indices[i - rank * local_size] = reverseBits(i, bits);
    }
    return local_indices;
}

// Perform bit-reversal using precomputed indices on an array of complex numbers                                                                 
void bitReversal(double complex data[], int N, int *indices, int par) {

    double complex *temp = (double complex*)malloc(N * sizeof(double complex));

    #pragma omp parallel for if(par)
    for (int i = 0; i < N; i++) {
        int j = indices[i];
        if (i < j) {
            double complex temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }
    free(temp);
}