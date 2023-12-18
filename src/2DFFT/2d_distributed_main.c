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
    
    MPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (argc > 2) {
        printf("FFT takes one argument: N, the size of the NxN square 2D input.\n");
        return 1;
    }

    int N = atoi(argv[1]);

    int valid = isPowerOfTwo(N);
    if (valid == 0) {
        printf("N must be greater than 1 and a power of two.\n");
        return 1;
    }
    int bits = (int) log2(N);
    int local_rows = N / num_procs;
    int local_size = N * local_rows;

    double complex *root_data = NULL;

    if (rank == 0) {
        root_data = (double complex*)malloc(N * N * sizeof(double complex));
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                root_data[i * N + j] = i * (1 + 1 * I);
            }
        }
    }
    double start_time = MPI_Wtime();

    int *global_indices = precomputeBitReverseIndices(N, bits, 1);
    double complex *global_twiddle = precomputeTwiddleFactors(N, 0);

    fftRows(
        root_data, N, local_size, local_rows, global_indices, global_twiddle, 1, 
        MPI_COMM_WORLD
    );
    
    if (rank == 0) {
        transpose(root_data, N, 1);
    }

    fftRows(
        root_data, N, local_size, local_rows, global_indices, global_twiddle, 1, 
        MPI_COMM_WORLD
    );
    free(global_indices);
    free(global_twiddle);
    
    if (rank == 0) {
        transpose(root_data, N, 1);
        // printComplexArray(root_data, N*N, 0);
        free(root_data);

        double end_time = MPI_Wtime();
        double total_time = end_time - start_time;
        printf("%i, %i, %f\n", num_procs*threads, N, total_time);

    }

    MPI_Finalize();
    return 0;
}