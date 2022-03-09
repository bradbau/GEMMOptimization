#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sys/time.h"
#include <stdbool.h>
#include "kernels.h"
#include "mkl.h"



void print_matrix(const double *A, int m, int n);
void print_vector(double *vec, int n);

void randomize_matrix(double *A, int m, int n);
double get_sec();
void copy_matrix(double *src, double *dest, int n);
bool verify_matrix(double *mat1, double *mat2, int n);
void test_kernel(int kernel_num,int m,int n,int k,double alpha,double *A,double *B,double beta,double *C);

#endif // _UTILS_H_