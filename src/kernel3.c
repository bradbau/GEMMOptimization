// 4*4 register blocking

#include "kernels.h"

void kernel3(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){
    
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);
    
    int M4= M&-4, N4 = N&-4;

    for(i=0; i<M4; i+=4){
        for(j=0;j<N4; j+=4){
            double c00 = C(i, j), c10= C(i+1, j), c20= C(i+2, j), c30= C(i+3, j);
            double c01 = C(i, j+1), c11= C(i+1, j+1), c21= C(i+2, j+1), c31= C(i+3, j+1);
            double c02 = C(i, j+2), c12= C(i+1, j+2), c22= C(i+2, j+2), c32= C(i+3, j+2);
            double c03 = C(i, j+3), c13= C(i+1, j+3), c23= C(i+2, j+3), c33= C(i+3, j+3);
            for(k=0; k<K; k++){
                double b0=alpha*B(k,j);
                double b1=alpha*B(k,j+1);
                double b2=alpha*B(k,j+2);
                double b3=alpha*B(k,j+3);
                double a0=A(i,k), a1=A(i+1,k), a2=A(i+2,k), a3=A(i+3,k);
                c00 += b0*a0;
                c10 += b0*a1;
                c20 += b0*a2;
                c30 += b0*a3;
                c01 += b1*a0;
                c11 += b1*a1;
                c21 += b1*a2;
                c31 += b1*a3;
                c02 += b2*a0;
                c12 += b2*a1;
                c22 += b2*a2;
                c32 += b2*a3;
                c03 += b3*a0;
                c13 += b3*a1;
                c23 += b3*a2;
                c33 += b3*a3;
            }
            C(i,j)=c00; C(i+1, j)=c10; C(i+2, j)=c20; C(i+3, j)=c30;
            C(i,j+1)=c01; C(i+1, j+1)=c11; C(i+2, j+1)=c21; C(i+3, j+1)=c31;
            C(i,j+2)=c02; C(i+1, j+2)=c12; C(i+2, j+2)=c22; C(i+3, j+2)=c32;
            C(i,j+3)=c03; C(i+1, j+3)=c13; C(i+2, j+3)=c23; C(i+3, j+3)=c33;
        }
    }
    if(M4!=M){
        kernel1(M-M4, N, K, alpha, &A(M4, 0), LDA, B, LDB, beta, &C(M4, 0), LDC);
    }
    if(N4!=N){
        kernel1(M4, N-N4, K, alpha, A, LDA, B+N4, LDB, beta, C+N4, LDC);
    }
}