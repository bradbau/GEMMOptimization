//add cache blocking compared to kernel9
#include "kernels.h"

void kernel11(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){

      
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);

    int m_count, n_count, k_count;
    int m_inc, n_inc, k_inc;

    for(n_count=0; n_count<N; n_count+=n_inc){
        n_inc= (N-n_count>N_BLOCKING)?N_BLOCKING:N-n_count;
        for(k_count=0;k_count<K;k_count+=k_inc){
            k_inc = (K-k_count>K_BLOCKING)?K_BLOCKING:K-k_count;
            for(m_count=0;m_count<M;m_count+=m_inc){
                m_inc = (M-m_count>M_BLOCKING)?M_BLOCKING:N-m_count;
                kernel9(m_inc, n_inc, k_inc, alpha, &A(m_count,k_count), LDA,  &B(k_count,n_count), LDB, 1, &C(m_count, n_count), LDC);
            }
        }
    }


}

    

  
