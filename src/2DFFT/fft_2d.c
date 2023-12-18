#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#include "../utils/utils.h"
#include "../1DFFT/fft_1d.h"
#include "fft_2d.h"

// Serial and OpenMP 2D FFT. Performs 1D FFT over rows and then again on columns.
void fft2d(
    double complex data2d[], int N, int* bitReversedIndices, 
    double complex *twiddleFactors, int par) {
    // 1. Slice 2D input array
    #pragma omp parallel for if(par)
    for (int i = 0; i < N; i++) {
        // Get a pointer to the beginning of the current row
        double complex *row = &data2d[i * N];
        // 2. Bit reverse swap each slice
        // 3. 1D FFT each slice
        bitReversal(row, N, bitReversedIndices, 0);
        fft1d(row, N, twiddleFactors, 0);
    }
    // 4. Transpose 2D array
    transpose(data2d, N, par);
    // 5. repeat (1) - (4)

    #pragma omp parallel for if(par)
    for (int j = 0; j < N; j++) {
        double complex* column = &data2d[j * N];
        bitReversal(column, N, bitReversedIndices, 0);
        fft1d(column, N, twiddleFactors, 0);
    }
    transpose(data2d, N, par);
}

void fftRows(
    double complex *global_data, int N, int local_size, int local_rows, 
    int* bitReversedIndices, double complex *twiddleFactors, int par, 
    MPI_Comm comm) {
    
    double complex *local_data = (double complex*)malloc(N * local_rows * sizeof(double complex));

    MPI_Scatter(
        global_data, local_size, MPI_C_DOUBLE_COMPLEX, local_data, local_size, 
        MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD
    );

    #pragma omp parallel for if(par)
    for (int i = 0; i < local_rows; i++) {
        double complex *row = &local_data[i * N];
        bitReversal(row, N, bitReversedIndices, 0);
        fft1d(row, N, twiddleFactors, 0);
    }
    
    MPI_Gather(
        local_data, local_size, MPI_C_DOUBLE_COMPLEX, global_data, local_size, 
        MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD
    );
    free(local_data);
}