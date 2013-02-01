//
//  file_load.h
//  DSA
//

#include "matrix.h"

#ifndef DSA_file_load_h
#define DSA_file_load_h

Matrix *load_file( char *filename );

Matrix *newMatrixFromArrays( unsigned int *rows, unsigned int *cols, int *vals,
	unsigned int rows_size, unsigned int cols_size, unsigned int quantity );
#endif
