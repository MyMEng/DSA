#include "matrix.h"

#ifndef DSA_file_load_h
#define DSA_file_load_h

//Load a file into Matrix structure
Matrix *load_file( char *filename );

//Create a matrix from array created in load_file
Matrix *newMatrixFromArrays( unsigned int *rows, unsigned int *cols, int *vals,
	unsigned int rows_size, unsigned int cols_size, unsigned int quantity );

#endif
