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

//Structure that represents a single matrix stored in 'Compressed sparse row'
typedef struct MatrixTag {
  //Size of matrix
  unsigned int rows;
  unsigned int columns;

  //Number of non-zero entries
  unsigned int quantity;

  //Content of matrix
  int *val;
  unsigned int *col_ind;
  unsigned int *row_ptr;
} Matrix;

//Free the memory after matrix structure
void freeMatrixMemory( Matrix *mx );

//Write the matrix to the file
void writeMatrixInFile( Matrix *matrix, char *fileName );

//Check whether dimmensions for addition are correct
bool sumDim( Matrix *A, Matrix *B );

//Add matrices A and B
Matrix *add( Matrix *A, Matrix *B );

//Acces the matrix data - print it to stdout
//Element - elements of row - elements of column
void print( Matrix *matrix, unsigned int r, unsigned int c );

//Transpose matrix may also serve as transformer from compresed row form to
//compresed column form. To calculate transpose we convert it to Compressed
//Column form and reed it as compressed row form
Matrix *transposeMatrix( Matrix *matrix );

//Check dimmensions for multiplication
bool productDim( Matrix *mxA, Matrix *mxB );

//Multiply matrices
Matrix *multiply( Matrix *mxA, Matrix *mxB );

//Sort in decreasing order
void backwardinsertSort( Matrix *mx, unsigned int a, unsigned int b );





//Get ints from string of chars
void fillWithNo( unsigned int *tmpint, int *val, char *tmpstr );

//Prepare data structure for matrix with specified number of non-zero elements
Matrix *makeDataStructure( unsigned int n, unsigned int *tempint, char c );

//Put data from file into data structure in organised maner
void organiseData( Matrix *matrix, FILE *file, char c );

//Sort elements between indeces a and b
void insertSort( Matrix *mx, unsigned int a, unsigned int b );

//Check whether file was opened without any error
bool checkFile( FILE *file );

//Prepare file to be read
void initializeReading( FILE *file, unsigned int *num, unsigned int *tempint,
	char *tempstr );

//search for element in matrix
bool checkOnMx( Matrix *matrix, unsigned int row, unsigned int column );

//Realloc matrix if too much or too little space left
void reallocMatrix( Matrix *A, Matrix *B, char c );

//Matrix *chainMultiply( Matrix *A, Matrix *B, Matrix *product )
void chainMultiply( Matrix *A, Matrix *B, Matrix *product, unsigned int *count );

void loadMatrix( Matrix *destination, Matrix *source );

Matrix *duplicate( Matrix *m );

int parseEntry( char **string, int *dims );

int countLines( char *string );

int compareMatrices( Matrix *a, Matrix *b );

Matrix *newMatrixFromString( char* str );

Matrix *newMatrix(  );

#endif
