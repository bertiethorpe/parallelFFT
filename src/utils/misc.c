

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <complex.h>
#include <omp.h>
#include <mpi.h>

#include "utils.h"

int isPowerOfTwo(int N) {
    return (N > 1) && ((N & (N - 1)) == 0);
}

void printArray(int *array, int size, int rank) {
    printf("Rank %d: ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void printComplexArray(double complex *array, int size, int rank) {
    printf("Rank %d: ", rank);
    for (int i = 0; i < size; i++) {
        printf("(%f + %fi) ", creal(array[i]), cimag(array[i]));
    }
    printf("\n");
}

// Function to find the in-place transpose of a square matrix
void transpose(double complex *data2d, int N, int par) {

    for (int i = 0; i < N; ++i) {
        #pragma omp parallel for if(par)
        for (int j = i + 1; j < N; ++j) {
            // Swap elements across the main diagonal
            int temp = data2d[i * N + j];
            data2d[i * N + j] = data2d[j * N + i];
            data2d[j * N + i] = temp;
        }
    }
}

void transposeMPI(double complex *local_data, int local_rows, int N, MPI_Comm comm) {
    // Assuming local_matrix is a 1D array representing a 2D matrix
    complex double *transposed = (complex double *)malloc(local_rows * N * sizeof(complex double));

    // forgoes fast inplace transpose due to the local data not being square
    for (int i = 0; i < local_rows; ++i) {
        for (int j = 0; j < N; ++j) {
            transposed[j * local_rows + i] = local_data[i * N + j];
        }
    }

    // Copy transposed data back to the original matrix
    for (int i = 0; i < local_rows * N; ++i) {
        local_data[i] = transposed[i];
    }

    free(transposed);
}