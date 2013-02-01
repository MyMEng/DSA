//
//  file_load.c
//  DSA
//
#include "file_load.h"

#include <stdio.h>
#include "util.h"
// A set of simple file loading helper functions

Matrix* load_file(const char* filename) {
    char inputData[65536];
    Matrix* m = NULL;
    FILE* f;
    size_t rd;
    f = fopen(filename, "r");
    
    if (f != NULL)
    {
        rd = fread((void *) inputData, sizeof(char), sizeof(inputData) - 1, NULL);
        
        if (rd == 0) {
            if (!feof(f))
            {
                fprintf(stderr, "Error on file read");
            }
        }
        
        // Null terminating the string
        inputData[rd] = '\0';
        
        m = newMatrixFromString(inputData);
    } else
    {
        fprintf(stderr, "There was a problem with the file");
    }
    return m;
}