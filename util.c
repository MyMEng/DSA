#include "util.h"

//Count number of lines in a non-empty file
int numberOfLines(FILE *file)
{
  //As max value of int is 65536 and there are three of these values plus
  //commas and spaces it's enough to have buffer of length 25
  char temp[BUFFSIZE];
  int counter = 0;

  while ( fscanf( file, "%s\n", temp ) != EOF ) counter++;
  return counter;
}

//Free the memory after matrix structure
void freeMatrixMemory( Matrix *mx )
{
  free( mx->val );
  free( mx->col_ind );
  free( mx->row_ptr );
  free( mx );
}

//Check whether memory was alloccated prroperly
void checkMem( void *check )
{
  if( check == NULL ) {
    fprintf( stderr, "Out of memory!\n" );
    exit( EXIT_FAILURE );
  }
}

//Get ints from string of chars
void fillWithNo(int *tmpint, char *tmpstr)
{
  //How many arguments were read
  int id = 0;

  id = sscanf ( tmpstr, "%d,%d,%d/n", &tmpint[0], &tmpint[1], &tmpint[2]);

  //Accept ony if 2 or 3 elements have been read(valid file formatting) 
  if( ( id != 2 ) && ( id != 3 ) )
  {
    fprintf( stderr, "Bad file formatting.\n" );
    exit( EXIT_FAILURE );
  }
}

  Matrix* makeDataStructure(int n, int *tempint)
  {
    Matrix *sparseMx = calloc( 1, sizeof( Matrix ) );
    checkMem( sparseMx );
    sparseMx->rows = tempint[0];
    sparseMx->columns = tempint[1];
    sparseMx->quantity = n;
    sparseMx->val = calloc( 1, sparseMx->quantity * sizeof( int ) );//+1 remember about '\0'
    checkMem( sparseMx->val );
    sparseMx->col_ind = calloc( 1, sparseMx->columns * sizeof( int ) );
    checkMem( sparseMx->col_ind );
    sparseMx->row_ptr = calloc( 1, sparseMx->quantity * sizeof( int ) );
    checkMem( sparseMx->row_ptr );
    return sparseMx;
  }

void organiseData( Matrix *matrix, FILE *file)
{
  char tempchar[BUFFSIZE];
  int tempint[3];
  while( fscanf( file, "%s\n", tempchar ) != EOF )
  {
    fillWithNo( tempint, tempchar );
    //printf("%d , %d , %d \n", tempint[0], tempint[1], tempint[2]);
  }
}

//Write the matrix to the file
void writeMatrixInFile( Matrix *matrix, char *fileName )
{
  FILE *file = fopen( fileName, "a" );
  if ( file == NULL ) {
    fprintf ( stderr, "Could not open the file.\n" );
    exit ( EXIT_FAILURE );
  } else {
    
    int x, y, z;
    fprintf( file, "%d,%d,%d/n", x, y, z );

  }

  fclose( file );
}

//Acces the matrix data - print stdout
//Element - elements of row - elements of column
void print( Matrix *matrix, int c, int r)
{

}
