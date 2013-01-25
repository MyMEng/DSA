#include "util.h"

int numberOfLines(FILE *file)
{
  //As max value of int is 65536 and there are three of these values plus
  //commas and spaces it's enough to have buffer of length 25
  char temp[BUFFSIZE];
  int counter = 0;

  while ( fscanf( file, "%s\n", temp ) != EOF ) counter++;
  return counter;
}

void freeMatrixMemory( Matrix *mx )
{
  free( mx->val );
  free( mx->col_ind );
  free( mx->row_ptr );
  free( mx );
}

void checkMem( void *check )
{
  if( check == NULL ) {
    fprintf( stderr, "Out of memory!\n" );
    exit( EXIT_FAILURE );
  }
}

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
  sparseMx->row_ptr = calloc( 1, ( sparseMx->rows + 1 ) * sizeof( int ) );
  checkMem( sparseMx->row_ptr );
  sparseMx->col_ind = calloc( 1, sparseMx->quantity * sizeof( int ) );
  checkMem( sparseMx->col_ind );
  return sparseMx;
  }

void organiseData( Matrix *matrix, FILE *file)
{
  char tempchar[BUFFSIZE];
  int tempint[3];

  //Current element in row
  int cn = 0;
  //Previous value of elements in row
  int on = 0;

  matrix->row_ptr[0] = 0;

  for(int i = 0; i < matrix->columns; i++)
  {
    on = cn;
    while( fscanf( file, "%s\n", tempchar ) != EOF )
    {
      fillWithNo( tempint, tempchar );

      if( tempint[0] == i )
      {
       matrix->col_ind[cn] = tempint[1];
       matrix->val[cn] = tempint[2];
       cn++;
      }
    }

    //Sort each row according to column number
    insertSort(matrix, on, cn);

    matrix->row_ptr[i + 1] = cn;

    rewind(file);

    //Avoid reading matrix size more than one time
    fillWithNo( tempint, tempchar );
  }

}

void writeMatrixInFile( Matrix *matrix, char *fileName )
{
  FILE *file = fopen( fileName, "w" );
  if ( file == NULL ) {
    fprintf ( stderr, "Could not write the file.\n" );
    exit ( EXIT_FAILURE );
  } else {
    fprintf( file, "%d,%d\n", matrix->rows, matrix->columns );
    
    int b;
    int a;
    
    //Overflow?
    for (int i = 0; i < matrix->rows; i++)
    {
      b = matrix->row_ptr[i + 1] - 1;
      a = matrix->row_ptr[i];
      for (int j = a; j <= b; j++)
      {
        fprintf( file, "%d,%d,%d\n", i, matrix->col_ind[j], matrix->val[j] );
      }
    }

  }
  fclose( file );
}

void print( Matrix *matrix, int r, int c )
{

  if ( r < 0 || r >= matrix->rows || c < 0 || c >= matrix->columns )
  {
      fprintf ( stderr, "Unexpected index.\n" );
      exit ( EXIT_FAILURE );
  }

  int b = ( matrix->row_ptr[r + 1] - 1 );
  int a = matrix->row_ptr[r];

  {//Print element (r, c)
    int temp = 0;
    for ( int i = a; i <= b; i++ )
    {
      if( c == matrix->col_ind[i] )
      {
        temp = matrix->val[i];
        continue;
      }
    }
    printf( "%d\n", temp );
  }

  {//Print row r
    int colId = a;

    if( 0 == matrix->col_ind[colId] )
    {
      printf( "%d", matrix->val[colId] );
      colId++;
    } else {
      printf( "0" );
    }

    for( int i = 1; i < matrix->columns; i++ )
    {
      if( i == matrix->col_ind[colId] && colId <= b )
      {
        printf( ",%d", matrix->val[colId] );
        colId++;
      } else {
        printf( ",0" );
      }
      
    }
    printf( "\n" );
  }

  {//Print column c
    int x;
    int y;
    int tempcol;

    //Overflow?
    
    tempcol = 0;
    for ( int j = matrix->row_ptr[0]; j <= ( matrix->row_ptr[1] - 1 ); j++ )
    {
          if( c == matrix->col_ind[j] )
          {
            tempcol = matrix->val[j];
            continue;
          }
    }
    printf( "%d", tempcol );
    
    for ( int i = 1; i < matrix->rows; i++ )
    {
      y = matrix->row_ptr[i + 1] - 1;
      x = matrix->row_ptr[i];
      tempcol = 0;
      for ( int j = x; j <= y; j++ )
      {
            if( c == matrix->col_ind[j] )
            {
              tempcol = matrix->val[j];
              continue;
            }
      }
      printf( ",%d", tempcol );
    }
    printf( "\n" );
  }

}

void insertSort( Matrix *mx, int a, int b )
{
  int colToIns;
  int valToIns;
  int valPos;
  for ( int i = ( a + 1 ); i < b; i++ )
  {
    valPos = i;
    colToIns = mx->col_ind[i];
    valToIns = mx->val[i];
    while( ( valPos > 0 ) && ( colToIns < mx->col_ind[valPos - 1] ) )
    {
      mx->col_ind[valPos] = mx->col_ind[valPos - 1];
      mx->val[valPos] = mx->val[valPos - 1];
      valPos--;
    }
    mx->col_ind[valPos] = colToIns;
    mx->val[valPos] = valToIns;
  }
}
