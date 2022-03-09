
#include "utils.h"

void print_vector(double *vec, int n){
    int i;
    for (i = 0; i < n; i++){
        printf(" %5.2f, ", vec[i]);
    }
    printf("\n");
}


void print_matrix(const double *A, int m, int n){
    int i;printf("[");
    for (i = 0; i < m * n; i++){
        if ((i + 1) % m == 0)
            printf("%5.2f ", A[i]);
        else
            printf("%5.2f, ", A[i]);
        if ((i + 1) % m == 0){
            if (i + 1 < m * n)
                printf(";\n");
        }
    }
    printf("]\n");
}

void copy_matrix(double *src, double *dest, int n){
    int i;
    for (i = 0; src + i && dest + i && i < n; i++){
        *(dest + i) = *(src + i);
    }
    if (i != n){
        printf("copy failed at %d while there are %d elements in total.\n", i, n);
    }
}

double get_sec(){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec + 1e-6 * time.tv_usec);
}


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

bool verify_matrix(double *mat1, double *mat2, int n){
    double diff = 0.0;
    int i;
    for (i = 0;  i < n; i++){
        diff = fabs(mat1[i] - mat2[i]);
        if (diff > 1e-2) {
            printf("error at %d: %5.2f,%5.2f\n",i,  mat1[i],mat2[i]);
            return false;
        }
    }
    return true;
}

void test_kernel(int kernel_num,int m,int n,int k,double alpha,double *A,double *B,double beta,double *C){
    switch (kernel_num){
        case 0:cblas_dgemm(CblasRowMajor, CblasNoTrans,CblasNoTrans,m,n,k,alpha,A,k ,B,n,beta,C,n);break; 
        case 1: kernel1(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 2: kernel2(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 3: kernel3(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 4: kernel4(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 5: kernel5(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 6: kernel6(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 7: kernel7(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 8: kernel8(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 9: kernel9(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 10: kernel10(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 11: kernel11(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 12: kernel12(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 13: kernel13(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 14: kernel14(m, n, k, alpha, A, k, B, n, beta, C, n);break;
        case 15: kernel15(m, n, k, alpha, A, k, B, n, beta, C, n);break;
    }
}


