#include "matrix.h"

/*
Perform stage 1:

- given a matrix X  stored in a file  named by X_name,
- inspect X, i.e., print the element, row and column at row r, column c.
*/

void stage1( char* X_name, int r, int c ) {


  //Read the file; "r"-read only
  FILE *file = fopen( X_name, "r" );
    if ( file == NULL ) {
      fprintf ( stderr, "Could not open the file\n" );
    }



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
