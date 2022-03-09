//cache blocking
#include "kernels.h"

#define M_BLOCKING 2048
#define N_BLOCKING 384
#define K_BLOCKING 192

#define KERNEL_13_8x8_intrinsics \
    a0 = _mm512_set1_pd(A(i,k));\
    a1 = _mm512_set1_pd(A(i+1,k));\
    a2 = _mm512_set1_pd(A(i+2,k));\
    a3 = _mm512_set1_pd(A(i+3,k));\
    a4 = _mm512_set1_pd(A(i+4,k));\
    a5 = _mm512_set1_pd(A(i+5,k));\
    a6 = _mm512_set1_pd(A(i+6,k));\
    a7 = _mm512_set1_pd(A(i+7,k));\
    b = _mm512_mul_pd(valpha, _mm512_loadu_pd(&B(k,j)));\
    c0 = _mm512_fmadd_pd(a0, b, c0);\
    c1 = _mm512_fmadd_pd(a1, b, c1);\
    c2 = _mm512_fmadd_pd(a2, b, c2);\
    c3 = _mm512_fmadd_pd(a3, b, c3);\
    c4 = _mm512_fmadd_pd(a4, b, c4);\
    c5 = _mm512_fmadd_pd(a5, b, c5);\
    c6 = _mm512_fmadd_pd(a6, b, c6);\
    c7 = _mm512_fmadd_pd(a7, b, c7);\
    k++; 

void macro_kernel_k13(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double *C, int LDC){
    int i, j, k;
    int M8= M&-8, N8 = N&-8, K4=K&-4;
    __m512d valpha = _mm512_set1_pd(alpha);
    for(i=0; i<M8; i+=8){
        for(j=0;j<N8; j+=8){
            __m512d a0, a1, a2, a3, a4, a5, a6, a7, b;
            __m512d c0, c1, c2, c3, c4, c5, c6, c7;
            c0=_mm512_loadu_pd(&C(i, j));
            c1=_mm512_loadu_pd(&C(i+1, j));
            c2=_mm512_loadu_pd(&C(i+2, j));
            c3=_mm512_loadu_pd(&C(i+3, j));
            c4=_mm512_loadu_pd(&C(i+4, j));
            c5=_mm512_loadu_pd(&C(i+5, j));
            c6=_mm512_loadu_pd(&C(i+6, j));
            c7=_mm512_loadu_pd(&C(i+7, j));
              
            for(k=0; k<K4; ){
                KERNEL_13_8x8_intrinsics
                KERNEL_13_8x8_intrinsics
                KERNEL_13_8x8_intrinsics
                KERNEL_13_8x8_intrinsics
            }
            for(k-K4; k<K;){
                KERNEL_13_8x8_intrinsics
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
        kernel1(M-M8, N, K, alpha, &A(M8, 0), LDA, B, LDB, 1, &C(M8, 0), LDC);
    }
    if(N8!=N){
        kernel1(M8, N-N8, K, alpha, A, LDA, B+N8, LDB, 1, C+N8, LDC);
    }
}



void kernel13(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){

      
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);

    int m_count, n_count, k_count;
    int m_inc, n_inc, k_inc;

    for(m_count=0;m_count<M;m_count+=m_inc){
        m_inc = (M-m_count>M_BLOCKING)?M_BLOCKING:N-m_count;
        for(n_count=0; n_count<N; n_count+=n_inc){
            n_inc= (N-n_count>N_BLOCKING)?N_BLOCKING:N-n_count;
            for(k_count=0;k_count<K;k_count+=k_inc){
                k_inc = (K-k_count>K_BLOCKING)?K_BLOCKING:K-k_count;
                macro_kernel_k13(m_inc, n_inc, k_inc, alpha, &A(m_count,k_count), LDA,  &B(k_count,n_count), LDB, &C(m_count, n_count), LDC);
            }
        }
    }


}