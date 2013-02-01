/*
  multiple entries  V
  reading with spaces,
  zastosuj regule wielu wejsc

  testy
  */

#include "matrix.h"
#include "file_load.h"
/*
Perform stage 1:

- given a matrix X  stored in a file  named by X_name,
- inspect X, i.e., print the element, row and column at row r, column c.
*/

void stage1( char* X_name, unsigned int r, unsigned int c )
{

  //char tempstr[BUFFSIZE];
  //unsigned int tempint[2];
  time_t t1, t2;
  t1 = clock(  );

  //number of non-0 elements in matrix
  //unsigned int n = 0;

//  //Read the file; "r"-read only
//  FILE *file = fopen( X_name, "r" );
//  if( checkFile( file ) )
//  {
//   initializeReading( file, &n, tempint, tempstr );
//  }
//
//  //Allocate space for matrix | 'r' = row compressed form
//  Matrix *matrixInstance = makeDataStructure(n, tempint, 'r');
//
//  //Read data into a structure and sort them
//  organiseData( matrixInstance, file, 'N' );

  Matrix* matrixInstance = load_file(X_name);
  //Data rerieval
  print(matrixInstance, r, c);

  //printf("%s\n", ( checkOnMx( matrixInstance, 1, 3 ) ) ? "true" : "false" );
  //printf("%s\n", ( checkOnMx( matrixInstance, 4, 1 ) ) ? "true" : "false" );
  //writeMatrixInFile( matrixInstance, "LOL.txt" );

  //Free the memory after data structure
  freeMatrixMemory( matrixInstance );

//  fclose( file );

  t2 = clock(  );
  printf("number of non-empty lines: %i\nTime elapsed %.5fs\n",
    matrixInstance->quantity, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

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
  unsigned int tempint[2];
  time_t t1, t2;
  t1 = clock(  );

  //number of non-0 elements in matrix
  unsigned int n = 0;

  //Read the file; "r"-read only
  FILE *file = fopen( X_name, "r" );
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
  writeMatrixInFile( transpose, R_name );

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

void stage3( char* R_name, char* X_name, char* Y_name )
{

  // char tempstrX[BUFFSIZE];
  // unsigned int tempintX[2];
  // char tempstrY[BUFFSIZE];
  // unsigned int tempintY[2];
  // time_t t1, t2;
  // t1 = clock(  );

  // //number of non-0 elements in matrix
  // unsigned int nX = 0;
  // unsigned int nY = 0;

  // //Read the file; "r"-read only
  // FILE *fileX = fopen( X_name, "r" );
  // FILE *fileY = fopen( Y_name, "r" );
  // if ( checkFile( fileX ) && checkFile( fileY ) )
  // {
  //   initializeReading( fileX, &nX, tempintX, tempstrX );
  //   initializeReading( fileY, &nY, tempintY, tempstrY );
  // }

  // //Allocate space for matrix | 'r' = row compressed form
  // Matrix *mxA = makeDataStructure( nX, tempintX, 'r' );
  // Matrix *mxB = makeDataStructure( nY, tempintY, 'r' );

  // //Read data into a structure and do not sort them
  // organiseData( mxA, fileX, 'T'/*'N'*/ );
  // organiseData( mxB, fileY, 'T'/*'N'*/ );

  // fclose( fileX );
  // fclose( fileY );

   Matrix* mxB = load_file(Y_name);
   Matrix* mxA = load_file(X_name);


  //If dimmensions of matrices are correct sum them
  Matrix *sum = NULL;
  if( sumDim( mxA, mxB ) )
  {
    sum = add( mxA, mxB );
  } else {
      fprintf ( stderr, "Uncompatible dimmensions.\n" );
      exit ( EXIT_FAILURE );
  }

  //writeMatrixInFile( mxA, "A.txt" );
  //writeMatrixInFile( mxB, "B.txt" );


  //write transpose to a file
  writeMatrixInFile( sum, R_name );

  //Free the memory after data structure
  freeMatrixMemory( sum );
  freeMatrixMemory( mxA );
  freeMatrixMemory( mxB );

  // t2 = clock(  );

  // printf("number of non-empty lines: X=%d | Y=%d\nTime elapsed %.5fs\n",
  //   nX, nY, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

}

/*
Perform stage 4:

- given   matrices X and Y stored   in   files named by X_name and Y_name,
- compute R = X \cdot Y, i.e., the product of x and y,
- then                     store  R in a file  named by R_name.
*/

void stage4( char* R_name, char* X_name, char* Y_name )
{

  // char tempstrX[BUFFSIZE];
  // unsigned int tempintX[2];
  // char tempstrY[BUFFSIZE];
  // unsigned int tempintY[2];
  time_t t1, t2;
  t1 = clock(  );

  // //number of non-0 elements in matrix
  // unsigned int nX = 0;
  // unsigned int nY = 0;

  // //Read the file; "r"-read only
  // FILE *fileX = fopen( X_name, "r" );
  // FILE *fileY = fopen( Y_name, "r" );
  // if ( checkFile( fileX ) && checkFile( fileY ) )
  // {
  //   initializeReading( fileX, &nX, tempintX, tempstrX );
  //   initializeReading( fileY, &nY, tempintY, tempstrY );
  // }

  // //Allocate space for matrix | 'r' = row compressed form
  // Matrix *mxA = makeDataStructure( nX, tempintX, 'r' );
  // Matrix *mxB = makeDataStructure( nY, tempintY, 'r' );

  // //Read data into a structure and do not sort them
  // organiseData( mxA, fileX, 'T'/*'N'*/ );
  // organiseData( mxB, fileY, 'T'/*'N'*/ );

  // fclose( fileX );
  // fclose( fileY );


  //printf("Finihed reading\n");
  //t1 = clock(  );

  //If dimmensions of matrices are correct sum them


  //od kuby
   Matrix* mxB = load_file(Y_name);
   Matrix* mxA = load_file(X_name);



  Matrix *product = NULL;
  if( productDim( mxA, mxB ) )
  {
    product = multiply( mxA, mxB );
  } else {
      fprintf ( stderr, "Uncompatible dimmensions.\n" );
      exit ( EXIT_FAILURE );
  }

  //write transpose to a file
  writeMatrixInFile( product, R_name );

  //Free the memory after data structure
  freeMatrixMemory( product );
  freeMatrixMemory( mxA );
  freeMatrixMemory( mxB );

  t2 = clock(  );

   printf("Time elapsed %.5fs\n",
    ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

  // printf("number of non-empty lines: X=%d | Y=%d\nTime elapsed %.5fs\n",
  //   nX, nY, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

}

/*
Perform stage 5:

- given l matrices X_i     stored in     files named by each X_name[ i ],
- compute R = \prod_{i=0}^{i < l} X_i, i.e., the product of the matrices,
- then                     store  R in a file  named by R_name.
*/

void stage5( char* R_name, char* X_name[], int l )
{

  if ( l < 2 )
  {
    fprintf ( stderr, "Wrong number of arguments.\n" );
    exit ( EXIT_FAILURE );
  }

  char tempstrX[BUFFSIZE];
  unsigned int tempintX[2];
  char tempstrY[BUFFSIZE];
  unsigned int tempintY[2];
  time_t t1, t2;
  t1 = clock(  );

  //number of non-0 elements in matrix
  unsigned int nX = 0;
  unsigned int nY = 0;
  unsigned int count = 0;

  //Final dimmension of matrix
  unsigned int totalRow = 0;
  unsigned int totalCol = 0;
  unsigned int totalElem = 0;


  //Check from the very begining whether it makes sens to start multiplying
  //Calculate final dimmension and create such matrix
  for (int i = 1; i < l; i++)
  {

    //Read the file; "r"-read only
    FILE *fileX = fopen( X_name[i - 1], "r" );
    FILE *fileY = fopen( X_name[i], "r" );
    if ( checkFile( fileX ) && checkFile( fileY ) )
    {
      initializeReading( fileX, &nX, tempintX, tempstrX );
      initializeReading( fileY, &nY, tempintY, tempstrY );
    }

    if( tempintX[1] != tempintY[0] )
    {
      fprintf ( stderr, "Uncompatible dimmensions.\n" );
      exit ( EXIT_FAILURE );
    }

    if (  i == 1 )
    {
      totalElem = nX + nY;
      totalRow = tempintX[0];
    } else if( i == l - 1 ) {
      totalCol = tempintY[1];
      totalElem += nY;
    } else {
      totalElem += nY;
    }

    fclose( fileX );
    fclose( fileY );

  }
  unsigned int tempint[2] = {totalRow, totalCol};
  Matrix *product = makeDataStructure( totalElem, tempint, 'r' );


  //fill product with matrix A
  Matrix* mOne = load_file( X_name[0] );
  loadMatrix( product, mOne );
  freeMatrixMemory( mOne );


  for( int i = 1; i < l; i++ )
  {
    Matrix* mp = load_file( X_name[i] );

    //Duplicate the product
    Matrix *productDuplicate = duplicate( product );
    

    chainMultiply( productDuplicate, mp, product, &count );
  
    freeMatrixMemory( mp );
    freeMatrixMemory( productDuplicate );

  }

  if ( count == product->row_ptr[product->rows] )
  {
    Matrix *shrinkedProduct = makeDataStructure( count, tempint, 'r');
    reallocMatrix( product, shrinkedProduct, '<' );
    freeMatrixMemory( product );

    //write transpose to a file
    writeMatrixInFile( shrinkedProduct, R_name );

    //Free the memory after data structure
    freeMatrixMemory( shrinkedProduct );

  } else {
    fprintf( stderr, "Error with multiplying. Wrong algorithm.\n" );
  }


  //Save effect of multiplying each pair in 'product' matrix,
  //  which has right dimmensions

  //   //Allocate space for matrix | 'r' = row compressed form
  //   Matrix *mxA = makeDataStructure( nX, tempintX, 'r' );
  //   Matrix *mxB = makeDataStructure( nY, tempintY, 'r' );

  //   //Read data into a structure and do not sort them
  //   organiseData( mxA, fileX, 'T'/*'N'*/ );
  //   organiseData( mxB, fileY, 'T'/*'N'*/ );

  //   fclose( fileX );
  //   fclose( fileY );

  //   //If dimmensions of matrices are correct sum them
  //   Matrix *product = NULL;
  //   if( productDim( mxA, mxB ) )
  //   {
  //     product = multiply( mxA, mxB );
  //   } else {
  //       fprintf ( stderr, "Uncompatible dimmensions.\n" );
  //       exit ( EXIT_FAILURE );
  //   }

  //   freeMatrixMemory( mxA );
  //   freeMatrixMemory( mxB );




  t2 = clock(  );


  printf("%dTime elapsed %.5fs\n",totalElem, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );
  // printf("number of non-empty lines: X=%d | Y=%d\nTime elapsed %.5fs\n",
  //   nX, nY, ( difftime( t2, t1 ) / CLOCKS_PER_SEC ) );

}

void sort( char* R_name, char* X_name )
{
     Matrix* mx = load_file(X_name);

    //double transpose is like sort
  //Transpose read matrix
    Matrix *transpose = transposeMatrix( mx );
    Matrix *srt = transposeMatrix( transpose );

  //Free the memory after data structure
  freeMatrixMemory( mx );
  freeMatrixMemory( transpose );


  //print(transpose, 0, 0);


  //write transpose to a file
  writeMatrixInFile( srt, R_name );

  //Free the memory after data structure
  freeMatrixMemory( srt );
}

/*
The main function acts as a driver for the assignment by simply invoking
the correct function for the requested stage.
*/

int main( int argc, char* argv[] ) {
  if     ( !strcmp( argv[ 1 ], "stage1" ) ) {
    if ( atoi( argv[3] ) < 0 || atoi( argv[4] ) < 0 )
    {
      fprintf ( stderr, "Uncompatible row or column.\n" );
      exit ( EXIT_FAILURE );
    }
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
  else if( !strcmp( argv[ 1 ], "sort" ) ) {
    sort( argv[ 2 ], argv[ 3 ] );
  }

  return 0;
}
