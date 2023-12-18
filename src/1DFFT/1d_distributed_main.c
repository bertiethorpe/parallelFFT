#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>
#include <omp.h>

#include "../utils/utils.h"
#include "fft_1d.h"

#define M_PI 3.14159265358979323846

int main(int argc, char* argv[]) {
    
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc > 2) {
        printf("FFT takes one argument: N, the size of the input.\n");
        return 1;
    }

    int N = atoi(argv[1]);

    int valid = isPowerOfTwo(N);
    if (valid == 0) {
        printf("N must be an integer greater than 1 and a power of two.\n");
        return 1;
    }
    int bits = (int) log2(N);
    // Assumes N is divisible by the number of ranks
    int local_size = N / size;
    double start_time = MPI_Wtime();

    int *local_indices = bitRevIndMPI(local_size, rank, bits, 1, MPI_COMM_WORLD);


    // Gather all local indices on process 0
    int* global_indices = NULL;
    if (rank == 0) {
        global_indices = (int*)malloc(N * sizeof(int));
    }

    MPI_Gather(
        local_indices, local_size, MPI_INT, global_indices, local_size, MPI_INT, 
        0, MPI_COMM_WORLD
    );

    // Use indices to bit reorder the input data
    double complex *global_data = NULL;
    if (rank == 0) {
        // printArray(global_indices, N, rank);
        // Allocate and initialise input data
        global_data = (double complex*)malloc(N * sizeof(double complex));
        for (int i = 0; i < N; i++) {
            global_data[i] = i * (1 + 1 * I);
        }
        // printComplexArray(global_data, N, rank);

        // bit reverse reorder the data
        bitReversal(global_data, N, global_indices, 1);
        // printComplexArray(global_data, N, rank);
        free(global_indices);
    }
    free(local_indices);

    double complex *local_twiddleFactors = twiddleFactorMPI(local_size, rank, N, 1, MPI_COMM_WORLD);
    // MPI_Barrier(MPI_COMM_WORLD);

    
    double complex *global_twiddleFactors = (double complex*)malloc(N * sizeof(double complex));
    
    MPI_Gather(
        local_twiddleFactors, local_size, MPI_C_DOUBLE_COMPLEX, 
        global_twiddleFactors, local_size, MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD
    );


    MPI_Bcast(global_twiddleFactors, N, MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD);
    // MPI_Barrier(MPI_COMM_WORLD);
    
    double complex* local_data = (double complex*)malloc(local_size * sizeof(double complex));
    
    MPI_Scatter(
        global_data, local_size, MPI_C_DOUBLE_COMPLEX, local_data, local_size, 
        MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD
    );
    // MPI_Barrier(MPI_COMM_WORLD);

    fft1d_MPI(
        local_data, local_size, 1, (int)log2(local_size), N, 
        global_twiddleFactors, 1, MPI_COMM_WORLD
    );
    // MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(
        local_data, local_size, MPI_C_DOUBLE_COMPLEX, global_data, local_size, 
        MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD
    );
    // MPI_Barrier(MPI_COMM_WORLD);
    free(local_data);

    if (rank == 0) {
        fft1d_MPI(
            global_data, N, ((int)log2(local_size)+1), ((int)log2(N)), 
            N, global_twiddleFactors, 1, MPI_COMM_WORLD
        );
        // printComplexArray(global_data, N, rank);
       
    }

    free(global_twiddleFactors);
    free(global_data);

    if (rank == 0) {
        double end_time = MPI_Wtime();
        double total_time = end_time - start_time;
        printf("%i, %i, %f\n", size, N, total_time);
    }

    MPI_Finalize();
    return 0;
}