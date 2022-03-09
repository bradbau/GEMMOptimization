
#include "kernels.h"
#include "utils.h"

void kernel1(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);
    
    for(i=0; i<M; i++){
        for(j=0;j<N; j++){
            double tmp=C(i,j);
            for(k=0; k<K; k++){
                tmp += alpha*A(i,k)*B(k,j);
            }
            C(i, j)=tmp;
        }
    }
}  
