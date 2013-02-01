#include "util.h"
#include <ctype.h>

int numberOfLines(FILE *file)
{
  //As max value of int is 65536 and there are three of these values plus
  //commas and spaces it's enough to have buffer of length 25
  char temp[BUFFSIZE];
  int counter = 0;

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
  if( ( id != 2 ) && ( id != 3 ) )
  {
    fprintf( stderr, "Bad file formatting.\n" );
    exit( EXIT_FAILURE );
  }
}

Matrix* makeDataStructure(int n, int *tempint, char c)
{
  Matrix *sparseMx = calloc( 1, sizeof( Matrix ) );
  checkMem( sparseMx );
  sparseMx->rows = tempint[0];
  sparseMx->columns = tempint[1];
  sparseMx->quantity = n;
  sparseMx->val = calloc( 1, sparseMx->quantity * sizeof( int ) );//+1 remember about '\0'
  checkMem( sparseMx->val );
  if ( c == 'r' ) //row compressed form
  {
    sparseMx->row_ptr = calloc( 1, ( sparseMx->rows + 1 ) * sizeof( int ) );
    sparseMx->col_ind = calloc( 1, sparseMx->quantity * sizeof( int ) );
  } else if( c == 'c' ) { //column compressed form
    sparseMx->row_ptr = calloc( 1, sparseMx->quantity * sizeof( int ) );
    sparseMx->col_ind = calloc( 1, ( sparseMx->columns + 1 ) * sizeof( int ) );
  } else if( c == 'T' ) {
    sparseMx->row_ptr = calloc( 1, ( sparseMx->columns + 1 ) * sizeof( int ) );
    sparseMx->col_ind = calloc( 1, sparseMx->quantity * sizeof( int ) );
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
  int cn = 0;
  //Previous value of elements in row
  int on = 0;

  int i;

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
    
    int b = 0;
    int a = 0;
    
    //Overflow?
    for (int i = 0; i < matrix->rows; ++i)
    {
      b = matrix->row_ptr[i + 1] - 1;
      a = matrix->row_ptr[i];
      for (int j = a; j <= b; j++)
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

//sort including a excluding b
void insertSort( Matrix *mx, int a, int b )
{
  int colToIns;
  int valToIns;
  int valPos;
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

void initializeReading(FILE *file, int *num, int *tempint, char *tempstr)
{
  //Check number of non-empty lines in file
  *num = numberOfLines(file);
  rewind(file);

  //Read the firs valid line of file to know the dimmensions of matrix
  if( fscanf( file, "%s\n", tempstr ) != EOF )
  {
    //Rearange first read line
    fillWithNo(tempint, tempstr);
  } else {
    fprintf ( stderr, "File empty.\n" );
    exit (EXIT_FAILURE);
  }
}



Matrix* transposeMatrix( Matrix *matrix )
{
  //allocate space
  int *workspace = calloc( 1, matrix->columns * sizeof( int ) );
  int temp[2] = {( matrix->columns ), ( matrix->rows )};
  Matrix *transpose = makeDataStructure( matrix->quantity, temp, 'T');

  //calculate collumn counts - compress columns | cumulative sum
  //
  //quantity of each element
  for ( int i = 0; i < matrix->quantity; i++ )
  {
    (workspace[matrix->col_ind[i]])++;
  }

  //for (int i = 0; i <= matrix->columns; i++) printf("# of elements %d\n", workspace[i]);
  //for (int i = 0; i < matrix->quantity; i++) printf("# of elements %d | val:%d\n", matrix->col_ind[i],matrix->val[i]);
  //printf("%d\n", matrix->quantity);

  //cumulative sum
  int v = 0;
  for ( int i = 0; i < matrix->columns; i++ )
  {
    transpose->row_ptr[i] = v;
    v += workspace[i];

    //Make a copy of cumulative sum in workspace
    workspace[i] = transpose->row_ptr[i];
  }
  transpose->row_ptr[matrix->columns] = v;

  //for (int i = 0; i <= matrix->columns; i++) printf("%d | %d\n", workspace[i], transpose->row_ptr[i]);

  //fill transpose with values and row indeces
  int w = 0;
  for ( int i = 0; i < matrix->rows; i++ )
  {
    for ( int j = matrix->row_ptr[i]; j < matrix->row_ptr[i + 1]; j++ )
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
  int temp[2] = {( A->rows ), ( A->columns )};
  int y = 0;

  //flag that indicates that something summed up to 0
  bool zeroSum = false;
  int zeroQuantity = 0;

  Matrix *sum = makeDataStructure( A->quantity + B->quantity, temp, 'r');
  int *workspaceA = calloc( A->columns, sizeof( int ) );
  int *workspaceB = calloc( B->columns, sizeof( int ) );

  for (int i = 0; i < A->rows; i++)
  {
    sum->row_ptr[i] = y;

    for (int a = A->row_ptr[i]; a < A->row_ptr[i + 1]; a++)
    {
      //Mark that column of index A->col_ind[a] has been read on this row
      workspaceA[A->col_ind[a]] = i + 1;
      sum->col_ind[y] = A->col_ind[a];
      y++;
      workspaceB[A->col_ind[a]] = A->val[a];

    }

    for (int b = B->row_ptr[i]; b < B->row_ptr[i + 1]; b++)
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


    for (int j = sum->row_ptr[i]; j < y; j++)
    {
      //when sum up to 0
      if (workspaceB[sum->col_ind[j]]  == 0)
      {
        zeroSum = true;
        zeroQuantity++;
      }
      sum->val[j] = workspaceB[sum->col_ind[j]];
    }

    if (zeroSum)
    {

      for (int g = sum->row_ptr[i]; g < y; ++g)
      {
        printf("O LOL:%d | %d | %d | #0 = %d\n", i, sum->col_ind[g], sum->val[g], zeroQuantity);
      }
      y -= zeroQuantity;

      zeroQuantity = 0;
      zeroSum = false;
    }

  }
  sum->row_ptr[A->columns] = y;

  free( workspaceA );
  free( workspaceB );
  //remove extra space in sum matrix

  return sum;
}

// Adding additional features
/**
 * Create an empty matrix
 */
Matrix* newMatrix() {
    Matrix* new = malloc(sizeof(Matrix));
    new->col_ind = NULL;
    new->row_ptr = NULL;
    new->val = NULL;
    new->columns = 0;
    new->quantity = 0;
    new->rows = 0;
    
    return new;
}

/**
 * Create a matrix with given dimensions
 */
Matrix* newMatrixWithDim(int columns, int rows) {
    Matrix* new = newMatrix();
    new->rows = rows;
    new->columns = columns;
    
    return new;
}

int parseEntry(char **string, int *dims)
{
    //How many arguments were read
    int id = 0;
    char *start = *string;
    char *header = NULL;
    
    
    // Splitting the string in two
    header = strtok_r(start, "\n", string);
    
    id = sscanf(header, "%d,%d,%d/n", &dims[0], &dims[1], &dims[2]);
    
    // Need to return the error

    return id;
}

/**
 * Compaing two matrices, assuming they are sorted
 */
int compareMatrices(const Matrix* a, const Matrix* b)
{
    if (a->rows != b->rows) return 0;
    if (a->columns != b->columns) return 0;
    if (a->quantity != b->quantity) return 0;
    int i;
    for (i = 0; i < a->quantity; i++) {
        if (a->col_ind[i] != b->col_ind[i]) return 0;
        if (a->val[i] != b->val[i]) return 0;
    }
    for (i = 0; i < a->rows + 1; i++) {
        if (a->row_ptr[i] != b->row_ptr[i]) return 0;
    }
    return 1;
}

int countLines(char* string)
{
    int i = 0;
    while (string != NULL)
    {
        if (*string == '\0')
        {
            break;
        }
        if (*string == '\n')
        {
            string++;
            continue;
        }
        else {
            i++;
        }
        string = strchr(string, '\n');
    }
    return i;
}

/**
 * Create a matrix from a string
 * 
 * It is also possible to create the matrix for a triplet of arrays
 */
Matrix* newMatrixFromString(char* str)
{
    Matrix* m = newMatrix();
    char* head = str;
    int dims[] = {0, 0, 0, 0};
    int result = parseEntry(&head, dims);
    if (result != 2) {
        free(m);
        return NULL;
    }
    m->rows = dims[0];
    m->columns = dims[1];
    // Here we count the number of new lines to allocate the elements
    m->quantity = countLines(head);
    
    // Allocating the memory for the arrays
    m->col_ind = calloc(m->quantity, sizeof(int));
    m->row_ptr = calloc(m->rows + 1, sizeof(int));
    m->val = calloc(m->quantity, sizeof(int));
    
    // Temporary array for sorting
    int *row_ind = calloc(m->quantity, sizeof(int));
    
    // Ignore malformed entries for now
    size_t j = 0;
    while (head[0] != '\0') {
        // Getting the numbers
        result = parseEntry(&head, dims);
        if (result != 3) {
            continue;
        }
        
        row_ind[j] = dims[0];
        m->col_ind[j] = dims[1];
        m->val[j] = dims[2];
        
        j += 1;
    }
    
    // Sorting the arrays
    
    int *arr = row_ind;
    int elements = m->quantity;
#define  MAX_LEVELS  64
    
    int  piv, piv_v, piv_c, beg[MAX_LEVELS], end[MAX_LEVELS], L, R, swap ;
    int i = 0;
    
    beg[0]=0; end[0]=elements;
    while (i >= 0) {
        L = beg[i];
        R = end[i]-1;
        
        if (L < R) {
            piv = arr[L];
            piv_v = m->val[L];
            piv_c = m->col_ind[L];
            
            while (L < R) {
                while (arr[R] >= piv && L < R)
                    R--;
                if (L<R) {
                    // Moving additional data around
                    m->val[L] = m->val[R];
                    m->col_ind[L] = m->col_ind[R];
                    arr[L++] = arr[R];
                }
                while (arr[L] <= piv && L < R)
                    L++;
                if (L<R) {
                    m->val[R] = m->val[L];
                    m->col_ind[R] = m->col_ind[L];
                    arr[R--] = arr[L];
                }
            }
            
            arr[L] = piv;
            m->val[L] = piv_v;
            m->col_ind[L] = piv_c;
            
            beg[i+1] = L+1;
            end[i+1] = end[i];
            end[i++] = L;
            
            if (end[i] - beg[i] > end[i-1] - beg[i-1]) {
                swap = beg[i];
                beg[i] = beg[i-1];
                beg[i-1] = swap;
                swap = end[i];
                end[i] = end[i-1];
                end[i-1] = swap;
            }
        }
        else {
            i--;
        }
    }
    // The arrays are sorted row wise
    // Compressing the rows
    int cur_row = 0;
    int* row_zero = &(m->row_ptr[1]);
    for (i = 0; i < m->quantity; i++) {
        while (cur_row < row_ind[i])
        {
            row_zero[cur_row++] = i;
        }
    }
    // Padding the rest
    while (cur_row < m->rows) {
        row_zero[cur_row++] = i;
    }
    
    // Writing to the last one
    
    // Freeing up the temporary array for sorting
    free(row_ind);
    return m;
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
