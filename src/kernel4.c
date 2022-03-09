// 6*6 register blocking

#include "kernels.h"

void kernel4(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){
    
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);
    
    int M6= M&-6, N6 = N&-6;

    for(i=0; i<M6; i+=6){
        for(j=0;j<N6; j+=6){
            double c00 = C(i, j), c10= C(i+1, j), c20= C(i+2, j), c30= C(i+3, j), c40= C(i+4, j), c50= C(i+5, j);
            double c01 = C(i, j+1), c11= C(i+1, j+1), c21= C(i+2, j+1), c31= C(i+3, j+1), c41= C(i+4, j+1), c51= C(i+5, j+1);
            double c02 = C(i, j+2), c12= C(i+1, j+2), c22= C(i+2, j+2), c32= C(i+3, j+2), c42= C(i+4, j+2), c52= C(i+5, j+2);
            double c03 = C(i, j+3), c13= C(i+1, j+3), c23= C(i+2, j+3), c33= C(i+3, j+3), c43= C(i+4, j+3), c53= C(i+5, j+3);
            double c04 = C(i, j+4), c14= C(i+1, j+4), c24= C(i+2, j+4), c34= C(i+3, j+4), c44= C(i+4, j+4), c54= C(i+5, j+4);
            double c05 = C(i, j+5), c15= C(i+1, j+5), c25= C(i+2, j+5), c35= C(i+3, j+5), c45= C(i+4, j+5), c55= C(i+5, j+5);
            for(k=0; k<K; k++){
                double b0=alpha*B(k,j);
                double b1=alpha*B(k,j+1);
                double b2=alpha*B(k,j+2);
                double b3=alpha*B(k,j+3);
                double b4=alpha*B(k,j+4);
                double b5=alpha*B(k,j+5);
                double a0=A(i,k), a1=A(i+1,k), a2=A(i+2,k), a3=A(i+3,k), a4=A(i+4,k), a5=A(i+5,k);
                
                c00 += b0*a0;c10 += b0*a1;c20 += b0*a2; c30 += b0*a3;c40 += b0*a4;c50 += b0*a5;
                c01 += b1*a0;c11 += b1*a1;c21 += b1*a2; c31 += b1*a3;c41 += b1*a4;c51 += b1*a5;
                c02 += b2*a0;c12 += b2*a1;c22 += b2*a2; c32 += b2*a3;c42 += b2*a4;c52 += b2*a5;
                c03 += b3*a0;c13 += b3*a1;c23 += b3*a2; c33 += b3*a3;c43 += b3*a4;c53 += b3*a5;
                c04 += b4*a0;c14 += b4*a1;c24 += b4*a2; c34 += b4*a3;c44 += b4*a4;c54 += b4*a5;
                c05 += b5*a0;c15 += b5*a1;c25 += b5*a2; c35 += b5*a3;c45 += b5*a4;c55 += b5*a5;
            }
            C(i,j)=c00; C(i+1, j)=c10; C(i+2, j)=c20; C(i+3, j)=c30;C(i+4, j)=c40;C(i+5, j)=c50;
            C(i,j+1)=c01; C(i+1, j+1)=c11; C(i+2, j+1)=c21; C(i+3, j+1)=c31;C(i+4, j+1)=c41;C(i+5, j+1)=c51;
            C(i,j+2)=c02; C(i+1, j+2)=c12; C(i+2, j+2)=c22; C(i+3, j+2)=c32;C(i+4, j+2)=c42;C(i+5, j+2)=c52;
            C(i,j+3)=c03; C(i+1, j+3)=c13; C(i+2, j+3)=c23; C(i+3, j+3)=c33;C(i+4, j+3)=c43;C(i+5, j+3)=c53;
            C(i,j+4)=c04; C(i+1, j+4)=c14; C(i+2, j+4)=c24; C(i+3, j+4)=c34;C(i+4, j+4)=c44;C(i+5, j+4)=c54;
            C(i,j+5)=c05; C(i+1, j+5)=c15; C(i+2, j+5)=c25; C(i+3, j+5)=c35;C(i+4, j+5)=c45;C(i+5, j+5)=c55;

        }
    }
    if(M6!=M){
        kernel1(M-M6, N, K, alpha, &A(M6, 0), LDA, B, LDB, beta, &C(M6, 0), LDC);
    }
    if(N6!=N){
        kernel1(M6, N-N6, K, alpha, A, LDA, B+N6, LDB, beta, C+N6, LDC);
    }
}