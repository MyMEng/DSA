#include "util.h"

unsigned int numberOfLines(FILE *file)
{
  //As max value of int is 65536 and there are three of these values plus
  //commas and spaces it's enough to have buffer of length 25
  char temp[BUFFSIZE];
  unsigned int counter = 0;

  fscanf( file, "%s\n", temp );

  while ( fscanf( file, "%s\n", temp ) != EOF ) counter++;

  //printf("Lines: %d\n", counter);

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
  if( id != 3  )
  {
    fprintf( stderr, "Bad file formatting.\n" );
    exit( EXIT_FAILURE );
  }
}


void initializeReading(FILE *file, unsigned int *num, unsigned int *tempint,
  char *tempstr)
{
  //Check number of non-empty lines in file
  int id = 0;

  *num = numberOfLines(file);
  rewind(file);

  //Read the firs valid line of file to know the dimmensions of matrix
  if( fscanf( file, "%s\n", tempstr ) != EOF )
  {
    //Rearange first read line
    id = sscanf ( tempstr, "%d,%d/n", &tempint[0], &tempint[1]);
    if ( id != 2 )
    {
      fprintf ( stderr, "File empty.\n" );
      exit (EXIT_FAILURE);
    }
  } else {
    fprintf ( stderr, "File empty.\n" );
    exit (EXIT_FAILURE);
  }
}

Matrix* makeDataStructure( unsigned int n, unsigned int *tempint, char c )
{
  Matrix *sparseMx = calloc( 1, sizeof( Matrix ) );
  checkMem( sparseMx );
  sparseMx->rows = tempint[0];
  sparseMx->columns = tempint[1];
  sparseMx->quantity = n;
  sparseMx->val = calloc( sparseMx->quantity, sizeof( int ) );//+1 remember about '\0'
  checkMem( sparseMx->val );
  if ( c == 'r' ) //row compressed form
  {
    sparseMx->row_ptr = calloc( sparseMx->rows + 1, sizeof( unsigned int ) );
    sparseMx->col_ind = calloc( sparseMx->quantity, sizeof( unsigned int ) );
  } else if( c == 'c' ) { //column compressed form
    sparseMx->row_ptr = calloc( sparseMx->quantity, sizeof( unsigned int ) );
    sparseMx->col_ind = calloc( sparseMx->columns + 1, sizeof( unsigned int ) );
  } else if( c == 'T' ) {
    sparseMx->row_ptr = calloc( sparseMx->columns + 1, sizeof( unsigned int ) );
    sparseMx->col_ind = calloc( sparseMx->quantity, sizeof( unsigned int ) );
  }
  
  checkMem( sparseMx->row_ptr );
  checkMem( sparseMx->col_ind );
  return sparseMx;
  }

void organiseData( Matrix *matrix, FILE *file, char c )
{
  char tempchar[BUFFSIZE];
  int tempint[3];

  //Current element in row
  unsigned int cn = 0;
  //Previous value of elements in row
  unsigned int on = 0;

  unsigned int i;

  matrix->row_ptr[0] = 0;

  for( i = 0; i < matrix->rows; i++ )
  {
    on = cn;
    while( fscanf( file, "%s\n", tempchar ) != EOF )
    {
      fillWithNo( tempint, tempchar );
      //printf("%d | %d | %d\n", tempint[0], tempint[1], tempint[2]);

      if( tempint[0] == i )
      {
       matrix->col_ind[cn] = tempint[1];
       matrix->val[cn] = tempint[2];
       //printf("%d | %d\n", matrix->col_ind[cn], matrix->val[cn]);
       cn++;
      }
    }

    //Sort each row according to column number
    //You don't have to sort when you do a trnspose
    if( c != 'T' )
    {
      insertSort( matrix, on, cn );
    }

    matrix->row_ptr[i + 1] = cn;

    rewind( file );

    //Avoid reading matrix size more than one time
    fscanf( file, "%s\n", tempchar );
  }
}

void writeMatrixInFile( Matrix *matrix, char *fileName )
{
  FILE *file = fopen( fileName, "w" );
  if ( file == NULL || matrix == NULL ) {
    fprintf ( stderr, "Could not write the file.\n" );
    exit ( EXIT_FAILURE );
  } else {
    fprintf( file, "%d,%d\n", matrix->rows, matrix->columns );
    
    unsigned int b = 0;
    unsigned int a = 0;
    
    //Overflow?
    for ( unsigned int i = 0; i < matrix->rows; i++ )
    {
      b = matrix->row_ptr[i + 1];
      a = matrix->row_ptr[i];
      for ( unsigned int j = a; j < b; j++ )
      {

        //printf("Counter: %d | a: %d | b: %d\n", i, a, b);

        fprintf( file, "%d,%d,%d\n", i, matrix->col_ind[j], matrix->val[j] );
        //printf("%d\n", j);
        //printf("%d | %d | %d\n", i, matrix->col_ind[j], matrix->val[j]);
      }
    }

  }
  fclose( file );
}

void print( Matrix *matrix, unsigned int r, unsigned int c )
{

  if ( r < 0 || r >= matrix->rows || c < 0 || c >= matrix->columns )
  {
      fprintf ( stderr, "Unexpected index.\n" );
      exit ( EXIT_FAILURE );
  }

  unsigned int b = matrix->row_ptr[r + 1];
  unsigned int a = matrix->row_ptr[r];

  {//Print element (r, c)
    int temp = 0;
    for ( unsigned int i = a; i < b; i++ )
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
    unsigned int colId = a;

    if( 0 == matrix->col_ind[colId] )
    {
      printf( "%d", matrix->val[colId] );
      colId++;
    } else {
      printf( "0" );
    }

    for( unsigned int i = 1; i < matrix->columns; i++ )
    {
      if( i == matrix->col_ind[colId] && colId < b )
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
    unsigned int x;
    unsigned int y;
    int tempcol;

    //Overflow?
    
    tempcol = 0;
    for ( unsigned int j = matrix->row_ptr[0]; j < matrix->row_ptr[1]; j++ )
    {
          if( c == matrix->col_ind[j] )
          {
            tempcol = matrix->val[j];
            continue;
          }
    }
    printf( "%d", tempcol );
    
    for ( unsigned int i = 1; i < matrix->rows; i++ )
    {
      y = matrix->row_ptr[i + 1];
      x = matrix->row_ptr[i];
      tempcol = 0;
      for ( unsigned int j = x; j < y; j++ )
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

//sort including a excluding b
void insertSort( Matrix *mx, unsigned int a, unsigned int b )
{
  unsigned int colToIns;
  int valToIns;
  unsigned int valPos;
  //printf("a is: %d | b is : %d\n",a, b );
  for ( int i = ( a + 1 ); i < b; i++ )
  {
    valPos = i;
    colToIns = mx->col_ind[valPos];
    valToIns = mx->val[valPos];
    while( ( valPos > a ) && ( colToIns < mx->col_ind[valPos - 1] ) )
    {
      mx->col_ind[valPos] = mx->col_ind[valPos - 1];
      mx->val[valPos] = mx->val[valPos - 1];
      valPos--;
    }
    mx->col_ind[valPos] = colToIns;
    mx->val[valPos] = valToIns;
  }
}

bool checkFile(FILE *file)
{
  if ( file == NULL )
  {
    fprintf ( stderr, "Could not open the file.\n" );
    exit ( EXIT_FAILURE );
  }
  return true;
}

Matrix* transposeMatrix( Matrix *matrix )
{
  //allocate space
  unsigned int *workspace = calloc( matrix->columns, sizeof( unsigned int ) );
  unsigned int temp[2] = {( matrix->columns ), ( matrix->rows )};
  Matrix *transpose = makeDataStructure( matrix->quantity, temp, 'T');

  //calculate collumn counts - compress columns | cumulative sum
  //
  //quantity of each element
  for ( unsigned int i = 0; i < matrix->quantity; i++ )
  {
    ( workspace[matrix->col_ind[i]] )++;
  }

  //for (int i = 0; i <= matrix->columns; i++) printf("# of elements %d\n", workspace[i]);
  //for (int i = 0; i < matrix->quantity; i++) printf("# of elements %d | val:%d\n", matrix->col_ind[i],matrix->val[i]);
  //printf("%d\n", matrix->quantity);

  //cumulative sum
  unsigned int v = 0;
  for ( unsigned int i = 0; i < matrix->columns; i++ )
  {
    transpose->row_ptr[i] = v;
    v += workspace[i];

    //Make a copy of cumulative sum in workspace
    workspace[i] = transpose->row_ptr[i];
  }
  transpose->row_ptr[matrix->columns] = v;

  //for (int i = 0; i <= matrix->columns; i++) printf("%d | %d\n", workspace[i], transpose->row_ptr[i]);

  //fill transpose with values and row indeces
  unsigned int w = 0;
  for ( unsigned int i = 0; i < matrix->rows; i++ )
  {
    for ( unsigned int j = matrix->row_ptr[i]; j < matrix->row_ptr[i + 1]; j++ )
    {
      //Place matrix(i,j) into transpose(j,i)
      //w = workspace[matrix->col_ind[j]]++;
      transpose->col_ind[w = workspace[matrix->col_ind[j]]++] = i;
      transpose->val[w] = matrix->val[j];
    }
  }

  free( workspace );
  return transpose;
}



bool sumDim( Matrix *A, Matrix *B )
{
  if( ( A->rows == B->rows ) && ( A->columns == B-> columns ) )
  {
    return true;
  } else {
    return false;
  }
}

Matrix *add( Matrix *A, Matrix *B )
{
  unsigned int temp[2] = {( A->rows ), ( A->columns )};
  unsigned int y = 0;

  //flag that indicates that something summed up to 0
  bool zeroSum = false;
  unsigned int zeroQuantity = 0;

  Matrix *sum = makeDataStructure( A->quantity + B->quantity, temp, 'r');
  unsigned int *workspaceA = calloc( A->columns, sizeof( unsigned int ) );
  int *workspaceB = calloc( B->columns, sizeof( int ) );

  for ( unsigned int i = 0; i < A->rows; i++ )
  {
    sum->row_ptr[i] = y;

    for ( unsigned int a = A->row_ptr[i]; a < A->row_ptr[i + 1]; a++ )
    {
      //Mark that column of index A->col_ind[a] has been read on this row
      workspaceA[A->col_ind[a]] = i + 1;
      sum->col_ind[y] = A->col_ind[a];
      y++;
      workspaceB[A->col_ind[a]] = A->val[a];

    }

    for ( unsigned int b = B->row_ptr[i]; b < B->row_ptr[i + 1]; b++ )
    {
      //Check whether element already have been read
      if ( workspaceA[B->col_ind[b]] <= i )
      {
        workspaceA[B->col_ind[b]] = i + 1;
        sum->col_ind[y] = B->col_ind[b];
        y++;
        workspaceB[B->col_ind[b]] = B->val[b];
      } else {
        workspaceB[B->col_ind[b]] += B->val[b];
      }
    }


    for ( unsigned int j = sum->row_ptr[i]; j < y; j++ )
    {
      //when sum up to 0
      if ( workspaceB[sum->col_ind[j]]  == 0 )
      {
        zeroSum = true;
        zeroQuantity++;
      }
      sum->val[j] = workspaceB[sum->col_ind[j]];
    }

    if ( zeroSum )
    {
      // for (int g = sum->row_ptr[i]; g < y; ++g)
      // {
      //   printf("O LOL:%d | %d | %d | #0 = %d\n", i, sum->col_ind[g], sum->val[g], zeroQuantity);
      // }


      backwardinsertSort( sum, sum->row_ptr[i], y );

      // for (int g = sum->row_ptr[i]; g < y; ++g)
      // {
      //   printf("O LOL:%d | %d | %d | #0 = %d\n", i, sum->col_ind[g], sum->val[g], zeroQuantity);
      // }
      y -= zeroQuantity;

      zeroQuantity = 0;
      zeroSum = false;
    }

  }
  sum->row_ptr[A->rows] = y;

  //printf("tyle elementow ostatecznie: %d\n", sum->row_ptr[A->rows]);
  //remove extra space in sum matrix
  Matrix *shrinkedSum = makeDataStructure( y, temp, 'r');
  reallocMatrix( sum, shrinkedSum, '<' );
  //val[]//col-int[]


  free( workspaceA );
  free( workspaceB );
  freeMatrixMemory( sum );

  return shrinkedSum;
}

void reallocMatrix( Matrix *A, Matrix *B, char c )
{
  if ( c == '<' ) //decrease the size to fit exactly
  {
    memcpy( B->val, A->val, B->quantity * sizeof( int ) );
    memcpy( B->row_ptr, A->row_ptr, ( B->rows + 1 ) * sizeof( unsigned int ) );
    memcpy( B->col_ind, A->col_ind, B->quantity * sizeof( unsigned int ) );
  } else {  //increase size by 2
    //TROLLOLOLLOLOLOLO
  }

}

//sort including a excluding b
void backwardinsertSort( Matrix *mx, unsigned int a, unsigned int b )
{
  unsigned int colToIns;
  int valToIns;
  unsigned int valPos;
  //printf("a is: %d | b is : %d\n",a, b );
  for ( int i = ( a + 1 ); i < b; i++ )
  {
    valPos = i;
    colToIns = mx->col_ind[valPos];
    valToIns = mx->val[valPos];
    while( ( valPos > a ) && ( valToIns > mx->val[valPos - 1] ) )
    {
      mx->col_ind[valPos] = mx->col_ind[valPos - 1];
      mx->val[valPos] = mx->val[valPos - 1];
      valPos--;
    }
    mx->col_ind[valPos] = colToIns;
    mx->val[valPos] = valToIns;
  }
}

bool productDim( Matrix *A, Matrix *B )
{
  if( ( A->columns == B->rows ) )
  {
    return true;
  } else {
    return false;
  }
}

Matrix *multiply( Matrix *A, Matrix *B )
{
  unsigned int temp[2] = {( A->rows ), ( B->columns )};
  unsigned int y = 0;

  //flag that indicates that something summed up to 0
  bool zeroProd = false;
  int zeroQuantity = 0;

  Matrix *product = makeDataStructure( A->quantity + B->quantity, temp, 'r');
  unsigned int *workspaceA = calloc( A->columns, sizeof( unsigned int ) );
  int *workspaceB = calloc( B->columns, sizeof( int ) );
  
  for ( unsigned int i = 0; i < A->rows; i++ )
  {

    //
    //check for memory realocation
    //reallocMatrix( product, shrinkedProduct, '>' );
    //

    product->row_ptr[i] = y;

    for ( unsigned int j = A->row_ptr[i]; j < A->row_ptr[i + 1]; j++ )
    {
      for ( unsigned int k = B->row_ptr[A->col_ind[j]];
        k < B->row_ptr[A->col_ind[j] + 1]; k++ )
      {
        if ( workspaceA[B->col_ind[k]] <= i )
        {
          workspaceA[B->col_ind[k]] = i + 1;
          product->col_ind[y] = B->col_ind[k];
          y++;
          workspaceB[B->col_ind[k]] = A->val[j] * B->val[k];
        } else {
          workspaceB[B->col_ind[k]] += A->val[j] * B->val[k];
        }
      }
    }
    for (int l = product->row_ptr[i]; l < y; l++)
    {

      if ( workspaceB[product->col_ind[l]]  == 0 )
      {
        zeroProd = true;
        zeroQuantity++;
      }

      product->val[l] = workspaceB[product->col_ind[l]];
    }

    if ( zeroProd )
    {

      backwardinsertSort( product, product->row_ptr[i], y );

      y -= zeroQuantity;

      zeroQuantity = 0;
      zeroProd = false;
    }

    //printf("Y is: %d\n", y);

  }

  product->row_ptr[A->rows] = y;
  //for (int x = 0; x <= A->rows; ++x) printf("Rowptr: %d\n", product->row_ptr[x]);

  Matrix *shrinkedProduct = makeDataStructure( y, temp, 'r');
  reallocMatrix( product, shrinkedProduct, '<' );

  free( workspaceA );
  free( workspaceB );
  freeMatrixMemory( product );

  //for (int x = 0; x <= A->rows; ++x) printf("Rowptr: %d\n", shrinkedProduct->row_ptr[x]);  

  return shrinkedProduct;
}

bool checkOnMx( Matrix *matrix, unsigned int row, unsigned int column )
{
  for ( int i = matrix->row_ptr[row]; i < matrix->row_ptr[row + 1]; i++ )
  {
    if ( matrix->col_ind[i] == column )
    {
      fprintf ( stderr, "Double entrie in same row: %d/col: %d.\n", row, column );
      return true;
    }
  }
  return false;
}


// //It;s not transpose it's convert
// //
// //na hama cisnij to w stara strukture i bedzie git ... hyba?? 
// Matrix* transposeMatrix( Matrix *matrix )
// {
//   //allocate space
//   int *workspace = calloc( 1, matrix->columns * sizeof( int ) );
//   int temp[2] = {( matrix->columns ), ( matrix->rows )};
//   Matrix *transpose = makeDataStructure( matrix->quantity, temp, 'c');

//   //calculate collumn counts - compress columns | cumulative sum
//   //
//   //quantity of each element
//   for ( int i = 0; i < matrix->quantity; i++ )
//   {
//     (workspace[matrix->col_ind[i]])++;
//   }
//   //cumulative sum
//   int v = 0;
//   for ( int i = 0; i < matrix->columns; i++ )
//   {
//     transpose->col_ind[i] = v;
//     v += workspace[i];

//     //Make a copy of cumulative sum in workspace
//     workspace[i] = transpose->col_ind[i];
//   }
//   transpose->col_ind[matrix->columns] = v;

//   //  for (int i = 0; i < matrix->columns; ++i) printf("%d\n", workspace[i]);

//   //fill transpose with values and row indeces
//   int w = 0;
//   for ( int i = 0; i < matrix->rows; i++ )
//   {
//     for ( int j = matrix->row_ptr[i]; j < matrix->row_ptr[i + 1]; j++ )
//     {
//       //Place matrix(i,j) into transpose(j,i)
//       w = workspace[matrix->col_ind[j]]++;
//       transpose->row_ptr[w] = i;
//       transpose->val[w] = matrix->val[j];
//     }
//   }

//   free( workspace );
//   return transpose;
// }
