#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <complex.h>
#include <omp.h>
#include <mpi.h>

#include "utils.h"

#define M_PI 3.14159265358979323846

// Precompute twiddle factors for FFT of size N
// For 1D fft this doesn't make sense but will be crucial for higher dimensions,
// where we will reuse the twiddles.
double complex *precomputeTwiddleFactors(int N, int par) {

    double complex* twiddleFactors = (double complex*)malloc(N * sizeof(double complex));
    if (twiddleFactors == NULL) {
        // Handle allocation error
        exit(1);
    }

    #pragma omp parallel for if(par)
    for (int k = 0; k < N; k++) {
        const double angle = -2.0 * M_PI * k / N;
        twiddleFactors[k] = cexp(I * angle);
    }

    return twiddleFactors;
}

double complex *twiddleFactorMPI(int local_size, int rank, int N, int par, MPI_Comm comm) {

    double complex* local_twiddleFactors = (double complex*)malloc(local_size * sizeof(double complex));
    #pragma omp parallel for if(par)
    for (int k = rank * local_size; k < (rank + 1) * local_size; k++) {
        const double angle = -2.0 * M_PI * k / N;
        local_twiddleFactors[k - rank * local_size] = cexp(I * angle);
    }
    return local_twiddleFactors;
}