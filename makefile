CC = mpiicc
CFLAGS = -O3 -std=c99 -qopenmp
LIBS = -lm

SRC_DIR = src
BUILD_DIR = build

# List of source files for shared memory fft1d
SRC_FFT1D_S = $(SRC_DIR)/1DFFT/1d_shared_main.c $(SRC_DIR)/1DFFT/fft_1d.c
OBJ_FFT1D_S = $(SRC_DIR)/1DFFT/1d_shared_main.o $(SRC_DIR)/1DFFT/fft_1d.o
EXEC_FFT1D_S = fft1Dshared

# List of source files for shared memory fft2d
SRC_FFT2D_S = $(SRC_DIR)/2DFFT/2d_shared_main.c $(SRC_DIR)/2DFFT/fft_2d.c $(SRC_DIR)/1DFFT/fft_1d.c
OBJ_FFT2D_S = $(SRC_DIR)/2DFFT/2d_shared_main.o $(SRC_DIR)/2DFFT/fft_2d.o $(SRC_DIR)/1DFFT/fft_1d.o
EXEC_FFT2D_S = fft2Dshared

# List of source files for distributed memory fft1d
SRC_FFT1D_D = $(SRC_DIR)/1DFFT/1d_distributed_main.c $(SRC_DIR)/1DFFT/fft_1d.c
OBJ_FFT1D_D = $(SRC_DIR)/1DFFT/1d_distributed_main.o $(SRC_DIR)/1DFFT/fft_1d.o
EXEC_FFT1D_D = fft1Ddistributed

# List of source files for distributed memory fft2d
SRC_FFT2D_D = $(SRC_DIR)/2DFFT/2d_distributed_main.c $(SRC_DIR)/2DFFT/fft_2d.c $(SRC_DIR)/1DFFT/fft_1d.c
OBJ_FFT2D_D = $(SRC_DIR)/2DFFT/2d_distributed_main.o $(SRC_DIR)/2DFFT/fft_2d.o $(SRC_DIR)/1DFFT/fft_1d.o
EXEC_FFT2D_D = fft2Ddistributed

# List of source files for utils
SRC_UTILS = $(wildcard $(SRC_DIR)/utils/*.c)
# OBJ_UTILS = $(patsubst $(SRC_DIR)/utils/%.c,$(BUILD_DIR)/%.o,$(SRC_UTILS))
OBJ_UTILS = $(SRC_UTILS:.c=.o)

# Compile all programs
all: $(EXEC_FFT1D_S) $(EXEC_FFT2D_S) $(EXEC_FFT1D_D) $(EXEC_FFT2D_D)

# Compile the fft1d shared memory program
$(EXEC_FFT1D_S): $(OBJ_FFT1D_S) $(OBJ_UTILS)
	$(CC) $(CFLAGS) -o $(EXEC_FFT1D_S) $^ $(LIBS)

# Compile the fft2d shared memory program
$(EXEC_FFT2D_S): $(OBJ_FFT2D_S) $(OBJ_UTILS)
	$(CC) $(CFLAGS) -o $(EXEC_FFT2D_S) $^ $(LIBS)

# Compile the fft1d distributed memory program
$(EXEC_FFT1D_D): $(OBJ_FFT1D_D) $(OBJ_UTILS)
	$(CC) $(CFLAGS) -o $(EXEC_FFT1D_D) $^ $(LIBS)

# Compile the fft2d distributed memory program
$(EXEC_FFT2D_D): $(OBJ_FFT2D_D) $(OBJ_UTILS)
	$(CC) $(CFLAGS) -o $(EXEC_FFT2D_D) $^ $(LIBS)

# Compile individual source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean the generated files
clean:
	rm -f $(OBJ_FFT1D_S) $(OBJ_FFT2D_S) $(OBJ_UTILS) $(EXEC_FFT1D_S) $(EXEC_FFT2D_S) \
	$(OBJ_FFT1D_D) $(OBJ_FFT2D_D) $(EXEC_FFT1D_D) $(EXEC_FFT2D_D)


.PHONY: all clean