#ifndef __UTIL_H
#define __UTIL_H

#define BUFFSIZE 25

#include  <stdarg.h>
#include <stdbool.h>
#include  <stdint.h>
#include   <stdio.h>
#include  <stdlib.h>

#include  <string.h>
#include    <time.h>

//Structure that represents a single matrix stored in 'Compressed sparse column'
typedef struct MatrixTag {
  //Size of matrix
  int rows;
  int columns;

  //Number of non-zero entries
  int quantity;

  //Content of matrix
  int *val;
  int *col_ind;
  int *row_ptr;
} Matrix;

int numberOfLines( FILE *file );

void freeMatrixMemory( Matrix *mx );

void checkMem( void *check );

void fillWithNo(int *tmpint, char *tmpstr);

Matrix* makeDataStructure(int n, int *tempint);

void organiseData( Matrix *matrix, FILE *file);

#endif
