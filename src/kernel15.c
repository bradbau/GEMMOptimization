//cache blocking
#include "kernels.h"

#define M_BLOCKING 512
#define N_BLOCKING 256
#define K_BLOCKING 64

#define KERNEL_15_fmadd_8x8 \
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm0;"\
    "leaq (%%r11, %3, 8), %%r11;" \
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm1;"\
    "leaq (%%r11, %3, 8), %%r11;"\
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm2;"\
    "leaq (%%r11, %3, 8), %%r11;"\
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm3;"\
    "leaq (%%r11, %3, 8), %%r11;"\
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm4;"\
    "leaq (%%r11, %3, 8), %%r11;"\
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm5;"\
    "leaq (%%r11, %3, 8), %%r11;"\
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm6;"\
    "leaq (%%r11, %3, 8), %%r11;"\
    "vbroadcastsd (%%r11), %%zmm9;"\
    "vfmadd231pd     %%zmm9, %%zmm8, %%zmm7;"


#define KERNEL_15_8x8_asm \
    "vmovupd (%1), %%zmm8;"/*load vector B to zmm8 and mul alpha*/ \
    "leaq (%1, %4, 8), %1;" /*b_ptr=b_ptr+ldb*8, get b_ptr always squals to &B(k, j) */\
    "vmulpd %%zmm16, %%zmm8, %%zmm8;"\
    "leaq (%0, %%r10, 8), %%r11;" /*load a_ptr+k*8 to r11, get &A(i, k)*/\
    KERNEL_15_fmadd_8x8 \
    "addq $1, %%r10;"


void macro_kernel_k15(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double *C, int LDC){
    int i, j, k;
    int M8= M&-8, N8 = N&-8, K4=K&-4;
    long LDA64=LDA, LDB64=LDB, LDC64=LDC, K64=K;
    __m512d valpha = _mm512_set1_pd(alpha);
    for(i=0; i<M8; i+=8){
        for(j=0;j<N8; j+=8){
            double* a_ptr, *b_ptr, *c_ptr;
            a_ptr=&A(i, 0);b_ptr=&B(0, j); c_ptr=&C(i, j); 
 
            __asm__ __volatile__(//

                //"movq %6, %%r10;"

                "vbroadcastsd %7, %%zmm16;" //valpha
                
                //initialize 
                "vpxorq %%zmm0,%%zmm0,%%zmm0;"
                "vpxorq %%zmm1,%%zmm1,%%zmm1;"
                "vpxorq %%zmm2,%%zmm2,%%zmm2;"                              
                "vpxorq %%zmm3,%%zmm3,%%zmm3;"
                "vpxorq %%zmm4,%%zmm4,%%zmm4;"
                "vpxorq %%zmm5,%%zmm5,%%zmm5;"
                "vpxorq %%zmm6,%%zmm6,%%zmm6;"
                "vpxorq %%zmm7,%%zmm7,%%zmm7;"
                
                "movq %6, %%r13;"
                "andq $-4, %%r13;" //K4 in r13
                "xor %%r10, %%r10;" // k in r10 register
                "LOOP_K4:"

                KERNEL_15_8x8_asm
                KERNEL_15_8x8_asm
                KERNEL_15_8x8_asm
                KERNEL_15_8x8_asm

                
                
                //"addq $4, %%r10;"
                "cmpq %%r13, %%r10;"
                
                "jb LOOP_K4;"

                "cmpq %%r13, %6;"
                "je STOREC;"
                "movq %%r13, %%r10;"
                "jmp LOOP_KRES;"

                "LOOP_KRES:"

                //load vector B to zmm8 and mul alpha 
                "vmovupd (%1), %%zmm8;"

                "leaq (%1, %4, 8), %1;" //b_ptr=b_ptr+ldb*8, get b_ptr always squals to &B(k, j) 
                "vmulpd %%zmm16, %%zmm8, %%zmm8;"

                //load a_ptr+k*8 to r11, get &A(i, k)
                "leaq (%0, %%r10, 8), %%r11;" 

                KERNEL_15_fmadd_8x8
                
                "addq $1, %%r10;"
                "cmpq %6, %%r10;"
                "jb LOOP_KRES;"


                "STOREC:"
                //store c 
                "vmovupd (%2), %%zmm10;" 
                "vaddpd %%zmm10, %%zmm0, %%zmm0; vmovupd  %%zmm0, (%2);"
                "leaq (%2, %5, 8), %2;"
                "vaddpd (%2), %%zmm1, %%zmm1; vmovupd  %%zmm1, (%2);"
                "leaq (%2, %5, 8), %2;"
                "vaddpd (%2), %%zmm2, %%zmm2; vmovupd  %%zmm2, (%2);"
                "leaq (%2, %5, 8), %2;"
                "vaddpd (%2), %%zmm3, %%zmm3; vmovupd  %%zmm3, (%2);"
                "leaq (%2, %5, 8), %2;"
                "vaddpd (%2), %%zmm4, %%zmm4; vmovupd  %%zmm4, (%2);"
                "leaq (%2, %5, 8), %2;"
                "vaddpd (%2), %%zmm5, %%zmm5; vmovupd  %%zmm5, (%2);"
                "leaq (%2, %5, 8), %2;"
                "vaddpd (%2), %%zmm6, %%zmm6; vmovupd  %%zmm6, (%2);"
                "leaq (%2, %5, 8), %2;"
                "vaddpd (%2), %%zmm7, %%zmm7; vmovupd  %%zmm7, (%2);"

               
                :
                "+r" (a_ptr),  //0 double *
                "+r" (b_ptr),  //1
                "+r" (c_ptr),  //2
                "+r" (LDA64), //3 int
                "+r" (LDB64), //4
                "+r" (LDC64), //5
                "+r" (K64) //6
                :
                "m" (alpha) //7
                
                :
                "cc",
                "%r10", "%r11", "%r12","%r13",
                "%zmm0", "%zmm1", "%zmm2", "%zmm3", "%zmm4",
                "%zmm5", "%zmm6", "%zmm7", "%zmm8", "%zmm9", "%zmm10", "%zmm16"
            );
        }
    }
    if(M8!=M){
        kernel1(M-M8, N, K, alpha, &A(M8, 0), LDA, B, LDB, 1, &C(M8, 0), LDC);
    }
    if(N8!=N){
        kernel1(M8, N-N8, K, alpha, A, LDA, B+N8, LDB, 1, C+N8, LDC);
    }
}



void kernel15(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){

      
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
                macro_kernel_k15(m_inc, n_inc, k_inc, alpha, &A(m_count,k_count), LDA,  &B(k_count,n_count), LDB, &C(m_count, n_count), LDC);
            }
        }
    }
}