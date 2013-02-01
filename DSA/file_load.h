//
//  file_load.h
//  DSA
//

#include "matrix.h"

#ifndef DSA_file_load_h
#define DSA_file_load_h

Matrix* load_file(const char* filename);
Matrix* newMatrixFromArrays(int* rows, int* cols, int* vals, int rows_size, int cols_size, int quantity);
#endif
