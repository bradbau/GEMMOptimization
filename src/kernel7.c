//change kernel5 to avx512 intrinsic
// 8*8 register blocking
//use 2 less instruction for each element in inner loop compared to kernel6

#include "kernels.h"

void kernel7(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){
    
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);
    
    int M8= M&-8, N8 = N&-8;
    __m512d valpha = _mm512_set1_pd(alpha);
    for(i=0; i<M8; i+=8){
        for(j=0;j<N8; j+=8){
            __m512d c0, c1, c2, c3, c4, c5, c6, c7;
            c0=_mm512_loadu_pd(&C(i, j));
            c1=_mm512_loadu_pd(&C(i+1, j));
            c2=_mm512_loadu_pd(&C(i+2, j));
            c3=_mm512_loadu_pd(&C(i+3, j));
            c4=_mm512_loadu_pd(&C(i+4, j));
            c5=_mm512_loadu_pd(&C(i+5, j));
            c6=_mm512_loadu_pd(&C(i+6, j));
            c7=_mm512_loadu_pd(&C(i+7, j));
              
            for(k=0; k  <K; k++){
                __m512d a0 = _mm512_set1_pd(A(i,k));
                __m512d a1 = _mm512_set1_pd(A(i+1,k));
                __m512d a2 = _mm512_set1_pd(A(i+2,k));
                __m512d a3 = _mm512_set1_pd(A(i+3,k));
                __m512d a4 = _mm512_set1_pd(A(i+4,k));
                __m512d a5 = _mm512_set1_pd(A(i+5,k));
                __m512d a6 = _mm512_set1_pd(A(i+6,k));
                __m512d a7 = _mm512_set1_pd(A(i+7,k));
                __m512d b = _mm512_mul_pd(valpha, _mm512_loadu_pd(&B(k,j)));
                c0 = _mm512_fmadd_pd(a0, b, c0);
                c1 = _mm512_fmadd_pd(a1, b, c1);
                c2 = _mm512_fmadd_pd(a2, b, c2);
                c3 = _mm512_fmadd_pd(a3, b, c3);
                c4 = _mm512_fmadd_pd(a4, b, c4);
                c5 = _mm512_fmadd_pd(a5, b, c5);
                c6 = _mm512_fmadd_pd(a6, b, c6);
                c7 = _mm512_fmadd_pd(a7, b, c7);
            }
            _mm512_storeu_pd(&C(i, j),c0);
            _mm512_storeu_pd(&C(i+1, j),c1);
            _mm512_storeu_pd(&C(i+2, j),c2);
            _mm512_storeu_pd(&C(i+3, j),c3);
            _mm512_storeu_pd(&C(i+4, j),c4);
            _mm512_storeu_pd(&C(i+5, j),c5);
            _mm512_storeu_pd(&C(i+6, j),c6);
            _mm512_storeu_pd(&C(i+7, j),c7);
        }
    }
    if(M8!=M){
        kernel1(M-M8, N, K, alpha, &A(M8, 0), LDA, B, LDB, beta, &C(M8, 0), LDC);
    }
    if(N8!=N){
        kernel1(M8, N-N8, K, alpha, A, LDA, B+N8, LDB, beta, C+N8, LDC);
    }
}