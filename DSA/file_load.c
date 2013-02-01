//
//  file_load.c
//  DSA
//
#include "file_load.h"
#include "util.h"
#include <stdio.h>
#include "util.h"
// A set of simple file loading helper functions

Matrix* load_file(const char* filename) {
    char inputData[65536];
    char* head = inputData;
    Matrix* m = NULL;
    FILE* f;
    size_t rd;
    f = fopen(filename, "r");
    int dims[] = {0, 0, 0, 0, 0};
    int rows, cols, quantity;
    rows = cols = quantity = 0;
    int header = 1;
    int capacity = 1024;
    
    int* row_array = NULL;
    int* col_array = NULL;
    int* val_array = NULL;
    int offset = 0;
    
    if (f != NULL)
    {
        row_array = calloc(capacity, sizeof(int));
        col_array = calloc(capacity, sizeof(int));
        val_array = calloc(capacity, sizeof(int));
        for (;;)
        {
            rd = fread((void *) &inputData[offset], sizeof(char), (sizeof(inputData) / sizeof(char)) - (1 + offset), f);
            
            if (rd == 0) {
                if (!feof(f))
                {
                    fprintf(stderr, "Error on file read");
                }
                // Finishing the loop
                break;
            }
            // Null terminating the string
            inputData[rd + offset] = '\0';
            // Cleaning the offset
            
            // Parsing
            // Then about incomplete strings
            // Here we need to detect if there was incomplete read.
            // if there is no new line there is a risk of an incomplete read
            // Therefore redo the read
            int working = 1;
            while (working)
            {
                if (strchr(head, '\n') == NULL) {
                    working = 0;
                    if (feof(f)) {
                        break;
                    }
                }
                // Copy the string to the beggining of the buffer
                parseEntry(&head, dims);
                //printf("Parsing %d %d %d\n", dims[0], dims[1], dims[2]);
                if (header) {
                    header = 0;
                    rows = dims[0];
                    cols = dims[1];
                } else
                {
                    // Resizing the array
                    if (quantity == capacity) {
                        // Double the arrays
                        int *old_rows = row_array;
                        int *old_cols = col_array;
                        int *old_vals = val_array;
                        
                        capacity = capacity << 1;
                        row_array = calloc(capacity, sizeof(int));
                        col_array = calloc(capacity, sizeof(int));
                        val_array = calloc(capacity, sizeof(int));
                        
                        // Copying the old arrays into the new ones
                        memcpy(row_array, old_rows, quantity * sizeof(int));
                        memcpy(col_array, old_cols, quantity * sizeof(int));
                        memcpy(val_array, old_vals, quantity * sizeof(int));
                        
                        // Free the old ones
                        free(old_rows);
                        free(old_cols);
                        free(old_vals);
                    }
                    // Parsing the non header entries
                    row_array[quantity] = dims[0];
                    col_array[quantity] = dims[1];
                    val_array[quantity] = dims[2];
                    
                    quantity++;
                }
 
            }
            
//            // Ensuring the proper offset
            offset = (int)strlen(head);
//            // Copying the offset to the beggining
            memcpy(inputData, head, sizeof(char) * offset);
            
        }
        
        fclose(f);
        
        // The parsing was finished, create the matrix
        m = newMatrixFromArrays(row_array, col_array, val_array, rows, cols, quantity);
        
        // Freeing the unecessary memmory
        free(row_array);
        free(col_array);
        free(val_array);
    }
    else
    {
        fprintf(stderr, "There was a problem with the file");
    }
    return m;
}



// Assume they are sorted
Matrix* newMatrixFromArrays(int* rows, int* cols, int* vals, int rows_size, int cols_size, int quantity)
{
    Matrix* m = newMatrix();
    m->rows = rows_size;
    m->columns = cols_size;
    // Here we count the number of new lines to allocate the elements
    m->quantity = quantity;
    
    // Allocating the memory for the arrays
    m->col_ind = calloc(quantity, sizeof(int));
    m->row_ptr = calloc(m->rows + 1, sizeof(int));
    m->val = calloc(quantity, sizeof(int));
    
    // Copying the data to the matrix arrays
    memcpy(m->col_ind, cols, quantity * sizeof(int));
    memcpy(m->val, vals, quantity * sizeof(int));
    
    // The arrays are sorted row wise
    // Compressing the rows
    int cur_row = 0;
    int* row_zero = &(m->row_ptr[1]);
    int i;
    for (i = 0; i < m->quantity; i++) {
        while (cur_row < rows[i])
        {
            row_zero[cur_row++] = i;
        }
    }
    // Padding the rest
    while (cur_row < m->rows) {
        row_zero[cur_row++] = i;
    }
    
    return m;
}