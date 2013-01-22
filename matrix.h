#ifndef __MATRIX_H
#define __MATRIX_H

#include "util.h"

//Structure of matrix stored in 'Compressed sparse row'
typedef struct MatrixTag {
  int *val;
  int *col_ind;
  int *row_ptr;
} Matrix;

#endif
