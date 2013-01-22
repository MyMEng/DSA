#include "matrix.h"

/*
Perform stage 1:

- given a matrix X  stored in a file  named by X_name,
- inspect X, i.e., print the element, row and column at row r, column c.
*/

void stage1( char* X_name, int r, int c ) {

  char temp[BUFFSIZE];
  time_t t1, t2;
  t1 = clock(  );

  //number of non-0 elements in matrix
  int n = 0;

  //Read the file; "r"-read only
  FILE *file = fopen( X_name, "r" );
  if ( file == NULL ) {
    fprintf ( stderr, "Could not open the file.\n" );
    exit ( EXIT_FAILURE );
  } else {
    //Read the firs valid line of file to know the dimmensions of matrix
    if ( fscanf( file, "%s\n", temp ) != EOF )
    {
    } else {
      fprintf ( stderr, "File empty.\n" );
      exit (EXIT_FAILURE);
    }
    
  }

  //Check number of non-empty lines in file
  n = numberOfLines(file);

  //Allocate space for matrix
  Matrix *sparseMx = calloc( 1, sizeof( Matrix ) );
  checkMem( sparseMx );
  sparseMx->val = calloc( 1, n * sizeof( int ) ); //+1 remember about '\0'
  //sparseMx->col_ind;
  //sparseMx->row_ptr;

  t2 = clock(  );
  printf("number of non-empty lines: %i\nTime elapsed %.5fs\n",
    n, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

  //Read the matrix into the structure
  //
  //Write the matrix to the file
  //
  //Acces the matrix data

  //Free the memory after data structure
  freeMatrixMemory( sparseMx );

  fclose( file );



  // fill in this function with solution

}

/*
Perform stage 2:

- given a matrix X         stored   in a file  named by X_name,
- compute R = X^T, i.e., the transpose of X, 
- then                     store  R in a file  named by R_name.
*/

void stage2( char* R_name, char* X_name ) {

  // fill in this function with solution

}

/*
Perform stage 3:

- given   matrices X and Y stored   in   files named by X_name and Y_name,
- compute R = X + Y, i.e., the sum of X and Y,
- then                     store  R in a file  named by R_name.
*/

void stage3( char* R_name, char* X_name, char* Y_name ) {

  // fill in this function with solution

}

/*
Perform stage 4:

- given   matrices X and Y stored   in   files named by X_name and Y_name,
- compute R = X \cdot Y, i.e., the product of x and y,
- then                     store  R in a file  named by R_name.
*/

void stage4( char* R_name, char* X_name, char* Y_name ) {

  // fill in this function with solution

}

/*
Perform stage 5:

- given l matrices X_i     stored in     files named by each X_name[ i ],
- compute R = \prod_{i=0}^{i < l} X_i, i.e., the product of the matrices,
- then                     store  R in a file  named by R_name.
*/

void stage5( char* R_name, char* X_name[], int l ) {

  // fill in this function with solution

}

/*
The main function acts as a driver for the assignment by simply invoking
the correct function for the requested stage.
*/

int main( int argc, char* argv[] ) {
  if     ( !strcmp( argv[ 1 ], "stage1" ) ) {
    stage1( argv[ 2 ], atoi( argv[ 3 ] ), atoi( argv[ 4 ] ) );
  }
  else if( !strcmp( argv[ 1 ], "stage2" ) ) {
    stage2( argv[ 2 ], argv[ 3 ] );
  }
  else if( !strcmp( argv[ 1 ], "stage3" ) ) {
    stage3( argv[ 2 ], argv[ 3 ], argv[ 4 ] );
  }
  else if( !strcmp( argv[ 1 ], "stage4" ) ) {
    stage4( argv[ 2 ], argv[ 3 ], argv[ 4 ] );
  }
  else if( !strcmp( argv[ 1 ], "stage5" ) ) {
    stage5( argv[ 2 ], argv + 3, argc - 3 );
  }

  return 0;
}
