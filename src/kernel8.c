//transform kernel7 8*8 inline asm
#include "kernels.h"
void kernel8(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){
    
    long ml=M, nl=N, kl=K;
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);

    __asm__ (

        
        "andq $-8, %3;" // M8 
        "andq $-8, %4;" // N8 

        //set valpha to zmm16
        "vbroadcastsd %9, %%zmm16;"

        "xor %%r8, %%r8;" // i in r8 register
        "LOOP_I:"  //start of i< M8 loop


        //load &A+i*LDA to rcx
        "movq %6, %%rax;"
        "mulq %%r8;"
        "addq %0, %%rax;"
        "movq %%rax, %%rcx;"
        
        //load &C+i*LDC to rax
        "movq %8, %%rax;"
        "mulq %%r8;"
        "addq %2, %%rax;"
        



        "xor %%r9, %%r9;" // j in r9 register
        "LOOP_J:"
        //load c to zmm0-7 
        "movq %8, %%rsi;"
        "vmovupd (%%rax, %%r8, 8), %%zmm0;"
        "leaq (%%rax, %%rsi, 8), %%rbx;"  // rbx= C+(i+1)*LDC
        "vmovupd (%%rbx, %%r8, 8), %%zmm1;"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"// rbx= C+(i+2)*LDC
        "vmovupd (%%rbx, %%r8, 8), %%zmm2;"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"// rbx= C+(i+3)*LDC
        "vmovupd (%%rbx, %%r8, 8), %%zmm3;"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"// rbx= C+(i+4)*LDC
        "vmovupd (%%rbx, %%r8, 8), %%zmm4;"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"// rbx= C+(i+5)*LDC
        "vmovupd (%%rbx, %%r8, 8), %%zmm5;"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"// rbx= C+(i+6)*LDC
        "vmovupd (%%rbx, %%r8, 8), %%zmm6;"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"// rbx= C+(i+7)*LDC
        "vmovupd (%%rbx, %%r8, 8), %%zmm7;"


        //load address of &B+j to rdx
        "leaq (%1, %%r9, 8), %%rdx;"
        //load address of &A+i*LDA to rcx
        //"vmulpd %%r8, %6, %%rcx;"
        //"vaddpd %0, %%rcx, %%rcx;"

        "xor %%r10, %%r10;" // k in r10 register
        "LOOP_K:"

        //load B to zmm8 and mul alpha
        "vmovupd (%%rdx, %%r10, 8), %%zmm8;"
        "vmulpd %%zmm16, %%zmm8, %%zmm8;"
        
        
        //calculate c=a*b+c
        "xor %%rsi, %%rsi;"
        "mov %6, %%esi;"
        "vfmadd231pd     (%%rcx,%%r10,8), %%zmm8, %%zmm0;"  //&A+(i)*LDA +k
        "leaq (%%rcx, %%rsi, 8), %%rdi;" //get &A+(i+1)*LDA
        "vfmadd231pd     (%%rdi,%%r10,8), %%zmm8, %%zmm1;"
        "leaq (%%rdi, %%rsi, 8), %%rdi;" //get &A+(i+2)*LDA
        "vfmadd231pd     (%%rdi,%%r10,8), %%zmm8, %%zmm2;"
        "leaq (%%rdi, %%rsi, 8), %%rdi;" //get &A+(i+3)*LDA
        "vfmadd231pd     (%%rdi,%%r10,8), %%zmm8, %%zmm3;"
        "leaq (%%rdi, %%rsi, 8), %%rdi;" //get &A+(i+4)*LDA
        "vfmadd231pd     (%%rdi,%%r10,8), %%zmm8, %%zmm4;"
        "leaq (%%rdi, %%rsi, 8), %%rdi;" //get &A+(i+5)*LDA
        "vfmadd231pd     (%%rdi,%%r10,8), %%zmm8, %%zmm5;"
        "leaq (%%rdi, %%rsi, 8), %%rdi;" //get &A+(i+6)*LDA
        "vfmadd231pd     (%%rdi,%%r10,8), %%zmm8, %%zmm6;"
        "leaq (%%rdi, %%rsi, 8), %%rdi;" //get &A+(i+7)*LDA
        "vfmadd231pd     (%%rdi,%%r10,8), %%zmm8, %%zmm7;"
        
        "addq $1, %%r10;"
        "cmpq %5, %%r10;"
        "jle LOOP_K;"

        //store c

        //load &C+i*LDC to rax
        "movq %8, %%rax;"
        "mulq %%r8;"
        "addq %2, %%rax;"

        "movq %8, %%rsi;"
        "vmovupd  %%zmm0, (%%rax, %%r8, 8);"
        "leaq (%%rax, %%rsi, 8), %%rbx;"  // rbx= C+(i+1)*LDC
        "vmovupd  %%zmm1, (%%rbx, %%r8, 8);"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"  // rbx= C+(i+2)*LDC
        "vmovupd  %%zmm2, (%%rbx, %%r8, 8);"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"  // rbx= C+(i+3)*LDC
        "vmovupd  %%zmm3, (%%rbx, %%r8, 8);"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"  // rbx= C+(i+4)*LDC
        "vmovupd  %%zmm4, (%%rbx, %%r8, 8);"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"  // rbx= C+(i+5)*LDC
        "vmovupd  %%zmm5, (%%rbx, %%r8, 8);"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"  // rbx= C+(i+6)*LDC
        "vmovupd  %%zmm6, (%%rbx, %%r8, 8);"
        "leaq (%%rbx, %%rsi, 8), %%rbx;"  // rbx= C+(i+7)*LDC
        "vmovupd  %%zmm7, (%%rbx, %%r8, 8);"
        
        "addq $8, %%r9;" //j
        "cmpq %4, %%r9;"
        "jle LOOP_J;"

        "addq $8, %%r8;" //i
        "cmpq %3, %%r8;"
        "jle LOOP_I;"


  

        :
        "+r" (A),  //0 double *
        "+r" (B),  //1
        "+r" (C)  //2

        :
        "g" (ml),  //3 int 
        "g" (nl),  //4
        "g" (kl),  //5
        
        "g" (LDA), //6 int
        "g" (LDB), //7
        "g" (LDC), //8
        "g" (alpha) //9
        
        :
        "cc",
        "%rax", "%rbx", "%rcx", "%rdx", "%rsi", "%r8", "%r9", "%r10",
        "%zmm0", "%zmm1", "%xmm2", "%zmm3", "%zmm4",
        "%zmm5", "%zmm6", "%zmm7", "%zmm8", "%zmm16"
    );

    int M8=M&-8, N8=N&-8; 
    if(M&8!=M){
        kernel1(M-M8, N, K, alpha, &A(M8, 0), LDA, B, LDB, beta, &C(M8, 0), LDC);
    }
    if(N8!=N){
        kernel1(M8, N-N8, K, alpha, A, LDA, B+N8, LDB, beta, C+N8, LDC);
    }
    

}