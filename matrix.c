//Backup 1

  /*
    0,001
    0,002
    0,003
    0,010
  */

#include "matrix.h"

/*
Perform stage 1:

- given a matrix X  stored in a file  named by X_name,
- inspect X, i.e., print the element, row and column at row r, column c.
*/

void stage1( char* X_name, int r, int c ) {

  char tempstr[BUFFSIZE];
  int tempint[3];
  time_t t1, t2;
  t1 = clock(  );

  //number of non-0 elements in matrix
  int n = 0;

  //Read the file; "r"-read only
  FILE *file = fopen( X_name, "r" );
  if( checkFile( file ) )
  {
   initializeReading( file, &n, tempint, tempstr ); 
  }

  //Allocate space for matrix | 'r' = row compressed form
  Matrix *matrixInstance = makeDataStructure(n, tempint, 'r');

  //Read data into a structure and sort them
  organiseData( matrixInstance, file, 'N' );

  //Data rerieval
  print(matrixInstance, r, c);

  //writeMatrixInFile( matrixInstance, "LOL.txt" );

  //Free the memory after data structure
  freeMatrixMemory( matrixInstance );

  fclose( file );

  t2 = clock(  );
  printf("number of non-empty lines: %i\nTime elapsed %.5fs\n",
    n, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

}

/*
Perform stage 2:

- given a matrix X         stored   in a file  named by X_name,
- compute R = X^T, i.e., the transpose of X, 
- then                     store  R in a file  named by R_name.
*/

void stage2( char* R_name, char* X_name )
{
  char tempstr[BUFFSIZE];
  int tempint[3];
  time_t t1, t2;
  t1 = clock(  );

  //number of non-0 elements in matrix
  int n = 0;

  //Read the file; "r"-read only
  FILE *file = fopen( R_name, "r" );
  if ( checkFile( file ) )
  {
    initializeReading( file, &n, tempint, tempstr );
  }

  //Allocate space for matrix | 'r' = row compressed form
  Matrix *matrixInstance = makeDataStructure(n, tempint, 'r');

  //Read data into a structure and sort them
  organiseData(matrixInstance, file, 'T');

  fclose( file );

  //Transpose read matrix
  Matrix *transpose = transposeMatrix( matrixInstance );

  //Free the memory after data structure
  freeMatrixMemory( matrixInstance );


  //print(transpose, 0, 0);


  //write transpose to a file
  writeMatrixInFile( transpose, X_name );

  //Free the memory after data structure
  freeMatrixMemory( transpose );

  t2 = clock(  );

  printf("number of non-empty lines: %i\nTime elapsed %.5fs\n",
    n, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

//MOZNA WCZYTAC DO STORADZA PIERDOLAC REWINDOWANIE FEJLA - RAZ A POZADNIE
//jak czytam linie z pliku to nie bawic sie w lapanie stringa tylko po prostu "\n"



}

/*
Perform stage 3:

- given   matrices X and Y stored   in   files named by X_name and Y_name,
- compute R = X + Y, i.e., the sum of X and Y,
- then                     store  R in a file  named by R_name.
*/

void stage3( char* R_name, char* X_name, char* Y_name ) {

  char tempstrX[BUFFSIZE];
  int tempintX[3];
  char tempstrY[BUFFSIZE];
  int tempintY[3];
  time_t t1, t2;
  t1 = clock(  );

  //number of non-0 elements in matrix
  int nX = 0;
  int nY = 0;

  //Read the file; "r"-read only
  FILE *fileX = fopen( X_name, "r" );
  FILE *fileY = fopen( Y_name, "r" );
  if ( checkFile( fileX ) && checkFile( fileY ) )
  {
    initializeReading( fileX, &nX, tempintX, tempstrX );
    initializeReading( fileY, &nY, tempintY, tempstrY );
  }

  //Allocate space for matrix | 'r' = row compressed form
  Matrix *mxA = makeDataStructure( nX, tempintX, 'r' );
  Matrix *mxB = makeDataStructure( nY, tempintY, 'r' );

  //Read data into a structure and sort them
  organiseData( mxA, fileX, 'N' );
  organiseData( mxB, fileY, 'N' );

  fclose( fileX );
  fclose( fileY );


  //If dimmensions of matrices are correct sum them
  Matrix *sum = NULL;
  if( sumDim( mxA, mxB ) )
  {
    sum = add( mxA, mxB );
  }

  //writeMatrixInFile( mxA, "A.txt" );
  //writeMatrixInFile( mxB, "B.txt" );


  //write transpose to a file
  writeMatrixInFile( sum, R_name );

  //Free the memory after data structure
  freeMatrixMemory( sum );
  freeMatrixMemory( mxA );
  freeMatrixMemory( mxB );

  t2 = clock(  );

  printf("number of non-empty lines: X=%d | Y=%d\nTime elapsed %.5fs\n",
    nX, nY, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

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
