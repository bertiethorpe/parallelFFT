#ifndef FFT_2D
#define FFT_2D

// Serial and OpenMP 2D FFT. Performs 1D FFT over rows and then again on columns.
void fft2d(
    double complex data2d[], int N, int* bitReversedIndices, 
    double complex *twiddleFactors, int par);

void fftRows(
    double complex *global_data, int N, int local_size, int local_rows, 
    int* bitReversedIndices, double complex *twiddleFactors, int par, 
    MPI_Comm comm);
    
#endif