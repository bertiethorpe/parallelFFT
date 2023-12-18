
#ifndef FFT_1D
#define FFT_1D

// Serial and OpenMP Radix-2 Cooley-Tukey iterative FFT using precomputed 
// twiddle factors and bit-reversed indices.
void fft1d(double complex data[], int N, double complex* twiddleFactors, int par);

// function required for both pure MPI and hybrid MPI/OpenMP distributed
// memory 1D FFT
void fft1d_MPI(
    double complex data[], int local_size, int start_stage, int end_stage, 
    int N, double complex* twiddleFactors, int par, MPI_Comm comm);

#endif