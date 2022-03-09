
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sys/time.h"
#include <stdbool.h>
#include "mkl.h"

void randomize_matrix(double *A, int m, int n){
    srand(time(NULL));
    int i, j;
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            A[i * m + j] = (double)(rand() % 100) + 0.01 * (rand() % 100);
            if (rand() % 2 == 0) A[i * m + j] *= 1.0;
        }
    }
}

double get_sec(){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec + 1e-6 * time.tv_usec);
}

int main(int argc, char *argv[]){
    

    int SIZE[30]={8, 10, 100,200,300,400,500,600,700,800,900,1000,1100,\
                1200,1300,1400,1500,1600,1700,1800,1900,2000,\
                2100,2200,2300,2400,2500,2600,2700,2800,2900,3000};//testing 100-3000 square matrices
    int m, n, k,max_size=3000;
    int n_count,N=10,upper_limit=30;

    
    double *A=NULL,*B=NULL,*C=NULL;
    double alpha = 2.0, beta = 0.;//two arbitary input parameters
    double t0,t1;
    A=(double *)malloc(sizeof(double)*max_size*max_size);
    B=(double *)malloc(sizeof(double)*max_size*max_size);
    C=(double *)malloc(sizeof(double)*max_size*max_size);

    randomize_matrix(A,max_size,max_size);randomize_matrix(B,max_size,max_size);randomize_matrix(C,max_size,max_size);
    int  i_count;
    for ( i_count=0;i_count<upper_limit;i_count++){
        m=n=k=SIZE[i_count];
        printf("\nM=N=K=%d:\n",m);
        t0=get_sec();
        for (n_count=0;n_count<N;n_count++){
            cblas_dgemm(CblasRowMajor, CblasNoTrans,CblasNoTrans,m,n,k,alpha,A,m,B,k,beta,C,m);
        }
        t1=get_sec();
        printf("%f %f \n", (t1-t0)/N,2.*1e-9*N*m*n*k/(t1-t0));
        
    }
    free(A);free(B);free(C);
    return 0;
}