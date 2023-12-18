#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#include "../utils/utils.h"
#include "../1DFFT/fft_1d.h"
#include "fft_2d.h"

int main(int argc, char* argv[]) {

    int threads = omp_get_max_threads();

    if (argc > 2) {
        printf("FFT takes one argument: N, the size of the NxN 2D input.\n");
        return 1;
    }

    int N = atoi(argv[1]);

    int valid = isPowerOfTwo(N);
    if (valid == 0) {
        printf("N must be greater than 1 and a power of two.\n");
        return 1;
    }
    int bits = (int) log2(N);

    double complex *data2d = (double complex *)malloc(N * N * sizeof(double complex));
    if (data2d == NULL) {
        fprintf(stderr, "Memory alloction failed\n");
        return 1;
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            data2d[i * N + j] = i + i * I;
        }
    }

    // Print the contents of the 2D complex array
    // printf("FFT input:\n");
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("(%d, %d): %f + %fi\n", i, j, creal(data2d[i * N + j]), cimag(data2d[i * N + j]));
    //     }
    // }

    double start_twiddle = omp_get_wtime(); // Record the start time
    double complex* twiddleFactors = precomputeTwiddleFactors(N, 1);
    double end_twiddle = omp_get_wtime(); // Record the end time

    double start_ind = omp_get_wtime(); // Record the start time
    int* bitReversedIndices = precomputeBitReverseIndices(N, bits, 1);
    double end_ind = omp_get_wtime(); // Record the end time

    double start_time = omp_get_wtime(); // Record the start time
    // Perform the FFT using precomputed twiddle factors and bit-reversed indices
    fft2d(data2d, N, bitReversedIndices, twiddleFactors, 1);
    double end_time = omp_get_wtime(); // Record the end time

    // Print the contents of the 2D fourier transformed array
    // printf("FFT output:\n");
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("(%d, %d): %f + %fi\n", i, j, creal(data2d[i * N + j]), cimag(data2d[i * N + j]));
    //     }
    // }

    free(twiddleFactors);
    free(bitReversedIndices);
    free(data2d);

    // Calculate the elapsed time in seconds
    double time_twiddle = end_twiddle - start_twiddle;
    double time_ind = end_ind - start_ind;
    double time_fft = end_time - start_time;
    double time_total = end_time - start_twiddle;

    // printf("Serial FFT took %.6f seconds.\n", elapsed_time);
    // printf("Twiddle took %f seconds.\n", elapsed_time_twiddle);
    // printf("Bit Reverse Indices took %f seconds.\n", elapsed_time_ind);

    // printf("FFT bitrev swap and butterflies took %f seconds.\n", elapsed_time);
    // printf("Total FFT took %f seconds.\n", total_time);

    printf("%i, %i, %f, %f, %f, %f\n", threads, N, time_twiddle, time_ind, time_fft, time_total);


    return 0;
}
