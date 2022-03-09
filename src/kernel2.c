// 4*1 register blocking

#include "kernels.h"

void kernel2(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){
    
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);
    
    int M4= M&-4;

    for(i=0; i<M4; i+=4){
        for(j=0;j<N; j++){
            double c0 = C(i, j), c1= C(i+1, j), c2= C(i+2, j), c3= C(i+3, j);
            for(k=0; k<K; k++){
                double b=alpha*B(k,j);
                c0 += b*A(i,k);
                c1 += b*A(i+1,k);
                c2 += b*A(i+2,k);
                c3 += b*A(i+3,k);
            }
            C(i, j)=c0;
            C(i+1, j)=c1;
            C(i+2, j)=c2;
            C(i+3, j)=c3;
        }
    }
    if(M4!=M){
        kernel1(M-M4, N, K, alpha, &A(M4, 0), LDA, B, LDB, beta, &C(M4, 0), LDC);
    }
}