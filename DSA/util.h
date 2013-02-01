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
  int rows;
  int columns;

  //Number of non-zero entries
  int quantity;

  //Content of matrix
  int *val;
  int *col_ind;
  int *row_ptr;
} Matrix;

//Count number of lines in a non-empty file
int numberOfLines( FILE *file );

//Free the memory after matrix structure
void freeMatrixMemory( Matrix *mx );

//Check whether memory was alloccated prroperly
void checkMem( void *check );

//Get ints from string of chars
void fillWithNo(int *tmpint, char *tmpstr);

//Prepare data structure for matrix with specified number of non-zero elements
Matrix *makeDataStructure(int n, int *tempint, char c);

//Put data from file into data structure in organised maner
void organiseData( Matrix *matrix, FILE *file, char c);

//Write the matrix to the file
void writeMatrixInFile( Matrix *matrix, char *fileName );

//Acces the matrix data - print it to stdout
//Element - elements of row - elements of column
void print( Matrix *matrix, int r, int c );

//Sort elements between indeces a and b
void insertSort( Matrix *mx, int a, int b );

//Check whether file was opened without any error
bool checkFile( FILE *file );

//Prepare file to be read
void initializeReading( FILE *file, int *num, int *tempint, char *tempstr );

//Transpose matrix or may also serve as transformer from compresed row form to
//compresed column form 
//To calculate transpose we convert it to Compressed Column form and reed it as
//compressed row form
Matrix *transposeMatrix( Matrix *matrix );

//Check whether dimmensions for addition are correct
bool sumDim( Matrix *A, Matrix *B );

//Add matrices A and B
Matrix *add( Matrix *A, Matrix *B );

int parseEntry(char **string, int *dims);
int countLines(char* string);
int compareMatrices(const Matrix* a, const Matrix* b);
Matrix* newMatrixFromString(char* str);
Matrix* newMatrix();
#endif
