 //
//  file_load.c
//  DSA
//
#include "file_load.h"
#include "util.h"
#include <stdio.h>
#include "util.h"
// A set of simple file loading helper functions

Matrix *load_file( char *filename )
{
  char inputData[65536];
  char* head = inputData;
  Matrix* m = NULL;
  FILE* f;
  size_t rd;
  f = fopen( filename, "r" );
    int dims[] = {0, 0, 0, 0, 0};   //khgjhgjhgjhg????
    unsigned int rows, cols, quantity;
    rows = cols = quantity = 0;
    int header = 1;
    int capacity = 1024;

    unsigned int* row_array = NULL;
    unsigned int* col_array = NULL;
    int* val_array = NULL;
    int offset = 0;

    if (f != NULL)
    {
      row_array = calloc( capacity, sizeof( unsigned int ) );
      col_array = calloc( capacity, sizeof( unsigned int ) );
      val_array = calloc( capacity, sizeof( int ) );
      for ( ; ; )
      {
        rd = fread( ( void * ) &inputData[offset], sizeof( char ),
          ( sizeof( inputData ) / sizeof( char ) ) - ( 1 + offset ), f );

        if ( rd == 0 )
        {
          if ( !feof( f ) )
          {
            fprintf( stderr, "Error on file read" );
          }
                // Finishing the loop
          break;
        }
            // Null terminating the string
        inputData[rd + offset] = '\0';
        head = inputData;
            // Cleaning the offset

        int working = 1;
        while ( working )
        {
          if ( strchr( head, '\n' ) == NULL )
          {
            working = 0;
            continue;
          }
                // Copy the string to the beggining of the buffer
          parseEntry( &head, dims );
                //printf("Parsing %d %d %d\n", dims[0], dims[1], dims[2]);
          if ( header )
          {
            header = 0;
            rows = dims[0];
            cols = dims[1];
          } else {
                    // Resizing the array
            if ( quantity == capacity )
            {
                        // Double the arrays
              unsigned int *old_rows = row_array;
              unsigned int *old_cols = col_array;
              int *old_vals = val_array;

              capacity = capacity << 1;
              row_array = calloc( capacity, sizeof( unsigned int ) );
              col_array = calloc( capacity, sizeof( unsigned int ) );
              val_array = calloc( capacity, sizeof( int ) );

                        // Copying the old arrays into the new ones
              memcpy( row_array, old_rows, quantity * sizeof( unsigned int ) );
              memcpy( col_array, old_cols, quantity * sizeof( unsigned int ) );
              memcpy( val_array, old_vals, quantity * sizeof( int ) );

                        // Free the old ones
              free( old_rows );
              free( old_cols );
              free( old_vals );
            }
                    // Parsing the non header entries
            row_array[quantity] = dims[0];
            col_array[quantity] = dims[1];
            val_array[quantity] = dims[2];

            quantity++;
          }

        }

            // Ensuring the proper offset
        if (head) {
          offset = ( int )strlen( head );
            // Copying the offset to the beggining
          memcpy( inputData, head, sizeof( char ) * offset );
        }
        else
        {
          offset = 0;
          working = 0;
        }
      }

      fclose( f );

        // Sorting arrays


        // The parsing was finished, create the matrix
      m = newMatrixFromArrays( row_array, col_array, val_array, rows, cols, quantity );

        // Freeing the unecessary memmory
      free( row_array );
      free( col_array );
      free( val_array );
    }
    else {
      fprintf( stderr, "There was a problem with the file" );
    }
    return m;
  }


  Matrix* newMatrixFromArrays(unsigned int* rows,unsigned int* cols, int* vals,
    unsigned int rows_size, unsigned int cols_size,unsigned int quantity )
  {
    Matrix* m = newMatrix(  );
    m->rows = rows_size;
    m->columns = cols_size;
    // Here we count the number of new lines to allocate the elements
    m->quantity = quantity;

    // Allocating the memory for the arrays
    m->col_ind = calloc( quantity, sizeof( unsigned int ) );
    m->row_ptr = calloc( m->rows + 1, sizeof( unsigned int ) );
    m->val = calloc( quantity, sizeof( int ) );

    // Copying the data to the matrix arrays
    memcpy( m->col_ind, cols, quantity * sizeof( unsigned int ) );
    memcpy( m->val, vals, quantity * sizeof( int ) );

    // The arrays are sorted row wise
#define  MAX_LEVELS  64

    int  piv, piv_v, piv_c, beg[MAX_LEVELS], end[MAX_LEVELS], L, R, swap ;
    int i = 0;

    beg[0]=0; end[0]=quantity;
    while (i >= 0) {
      L = beg[i];
      R = end[i]-1;

      if (L < R) {
        piv = rows[L];
        piv_v = m->val[L];
        piv_c = m->col_ind[L];

        while (L < R) {
          while (rows[R] >= piv && L < R)
            R--;
          if (L<R) {
                    // Moving additional data around
            m->val[L] = m->val[R];
            m->col_ind[L] = m->col_ind[R];
            rows[L++] = rows[R];
          }
          while (rows[L] <= piv && L < R)
            L++;
          if (L<R) {
            m->val[R] = m->val[L];
            m->col_ind[R] = m->col_ind[L];
            rows[R--] = rows[L];
          }
        }

        rows[L] = piv;
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
    // Compressing the rows
    unsigned int cur_row = 0;
    unsigned int *row_zero = &( m->row_ptr[1] );
    for ( i = 0; i < m->quantity; i++ )
    {
      while ( cur_row < rows[i] )
      {
        row_zero[cur_row++] = i;
      }
    }
    // Padding the rest
    while ( cur_row < m->rows )
    {
      row_zero[cur_row++] = i;
    }

    return m;
  }
