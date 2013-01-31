#ifndef __MATRIX_H
#define __MATRIX_H

#include "util.h"

int main( int argc, char* argv[] );

void stage1( char* X_name, unsigned int r, unsigned int c );

void stage2( char* R_name, char* X_name );

void stage3( char* R_name, char* X_name, char* Y_name );

void stage4( char* R_name, char* X_name, char* Y_name );

void stage5( char* R_name, char* X_name[], int l );

#endif
