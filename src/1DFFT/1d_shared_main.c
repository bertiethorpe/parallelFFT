#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#include "../utils/utils.h"
#include "fft_1d.h"

int main(int argc, char *argv[]) {
    
    int threads = omp_get_max_threads();

    if (argc > 2) {
        printf("FFT takes one argument: N, the size of the input.\n");
        return 1;
    }

    int N = atoi(argv[1]);

    int valid = isPowerOfTwo(N);
    if (valid == 0) {
        printf("N must be greater than 1 and a power of two.\n");
        return 1;
    }

    int bits = (int) log2(N);
    double start = omp_get_wtime();

    double complex *data = (double complex *)malloc(N * sizeof(double complex));
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    // Initialize data array...
    #pragma omp parallel for
    for(int i = 0; i < N; i++) {
        data[i] = i * ( 1 + (1.0 * I));
    }
    
    // printf("Input Data:\n");
    // for (int i = 0; i < N; i++) {
    //     printf("%f + %fi\n", creal(data[i]), cimag(data[i]));
    // }

    double start_twiddle = omp_get_wtime(); // Record the start time
    double complex* twiddleFactors = precomputeTwiddleFactors(N, 1);
    double end_twiddle = omp_get_wtime(); // Record the end time

    double start_ind = omp_get_wtime(); // Record the start time
    int* bitReversedIndices = precomputeBitReverseIndices(N, bits, 1);
    double end_ind = omp_get_wtime(); // Record the end time

    double start_bit = omp_get_wtime();
    bitReversal(data, N, bitReversedIndices, 1);
    double end_bit = omp_get_wtime();
    
    free(bitReversedIndices);
    
    double start_time = omp_get_wtime(); // Record the start time
    // Perform the FFT using precomputed twiddle factors and bit-reversed indices
    fft1d(data, N, twiddleFactors, 1);

    // Print the FFT results...
    // printf("Output Data:\n");
    // for (int i = 0; i < N; i++) {
    //     printf("%f + %fi\n", creal(data[i]), cimag(data[i]));
    // }

    // Free dynamically allocated memory
    free(twiddleFactors);
    free(data);
    double end_time = omp_get_wtime(); // Record the end time

    // Calculate the elapsed time in seconds
    double time_twiddle = end_twiddle - start_twiddle;
    double time_ind = end_ind - start_ind;
    double time_bit = end_bit - start_bit;
    double time_butterfly = end_time - start_time;
    double time_total = end_time - start;

    // printf("Serial FFT took %.6f seconds.\n", elapsed_time);
    // printf("Twiddle took %f seconds.\n", time_twiddle);
    // printf("Bit Reverse Indices took %f seconds.\n", time_ind);
    // printf("Bit Reverse Swap took %f seconds.\n", time_bit);
    // printf("FFT butterflies took %f seconds.\n", time_butterfly);
    // printf("Total FFT took %f seconds.\n", time_total);

    printf("%i, %i, %f, %f, %f, %f, %f\n", threads, N, time_twiddle, time_ind, time_bit, time_butterfly, time_total);

    return 0;
}