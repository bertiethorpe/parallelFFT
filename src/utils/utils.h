
#ifndef UTILS
#define UTILS

int isPowerOfTwo(int N);

// Function to reverse the bits of a given number
uint32_t reverseBits(uint32_t num, int bits);

// Function to precompute array of bit-reversed indices
int *precomputeBitReverseIndices(int N, int bits, int par);

int *bitRevIndMPI(int local_size, int rank, int bits, int par, MPI_Comm comm);

// Function to perform bit-reversal on an array of complex numbers
void bitReversal(double complex data[], int N, int* indices, int par);

// Precompute twiddle factors to avoid needless recalculations
double complex *precomputeTwiddleFactors(int N, int par);

double complex *twiddleFactorMPI(
    int local_size, int rank, int N, int par, MPI_Comm comm);

// Transpose square 2D array in place
void transpose(double complex *data2d, int N, int par);

void transposeMPI(double complex *local_data, int local_rows, int N, MPI_Comm comm);

int isPowerOfTwo(int N);

void printArray(int *array, int size, int rank);

void printComplexArray(double complex *array, int size, int rank);

#endif