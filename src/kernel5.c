// 8*8 register blocking

#include "kernels.h"

void kernel5(int M, int N, int K, double alpha, double *A, int LDA, double *B, int LDB, double beta, double *C, int LDC){
    
    int i, j, k;
    if(beta != 1.0) scale_c(C,M,N,LDC,beta);
    
    int M8= M&-8, N8 = N&-8;

    for(i=0; i<M8; i+=8){
        for(j=0;j<N8; j+=8){
            double c00 = C(i, j), c10= C(i+1, j), c20= C(i+2, j), c30= C(i+3, j), c40= C(i+4, j), c50= C(i+5, j),c60= C(i+4, j), c70= C(i+5, j);
            double c01 = C(i, j+1), c11= C(i+1, j+1), c21= C(i+2, j+1), c31= C(i+3, j+1), c41= C(i+4, j+1), c51= C(i+5, j+1), c61= C(i+6, j+1), c71= C(i+7, j+1);
            double c02 = C(i, j+2), c12= C(i+1, j+2), c22= C(i+2, j+2), c32= C(i+3, j+2), c42= C(i+4, j+2), c52= C(i+5, j+2), c62= C(i+6, j+2), c72= C(i+7, j+2);
            double c03 = C(i, j+3), c13= C(i+1, j+3), c23= C(i+2, j+3), c33= C(i+3, j+3), c43= C(i+4, j+3), c53= C(i+5, j+3), c63= C(i+6, j+3), c73= C(i+7, j+3);
            double c04 = C(i, j+4), c14= C(i+1, j+4), c24= C(i+2, j+4), c34= C(i+3, j+4), c44= C(i+4, j+4), c54= C(i+5, j+4), c64= C(i+6, j+4), c74= C(i+7, j+4);
            double c05 = C(i, j+5), c15= C(i+1, j+5), c25= C(i+2, j+5), c35= C(i+3, j+5), c45= C(i+4, j+5), c55= C(i+5, j+5), c65= C(i+6, j+5), c75= C(i+7, j+5);
            double c06 = C(i, j+6), c16= C(i+1, j+6), c26= C(i+2, j+6), c36= C(i+3, j+6), c46= C(i+4, j+6), c56= C(i+5, j+6), c66= C(i+6, j+6), c76= C(i+7, j+6);
            double c07 = C(i, j+7), c17= C(i+1, j+7), c27= C(i+2, j+7), c37= C(i+3, j+7), c47= C(i+4, j+7), c57= C(i+5, j+7), c67= C(i+6, j+7), c77= C(i+7, j+7);
            for(k=0; k  <K; k++){
                double b0=alpha*B(k,j);
                double b1=alpha*B(k,j+1);
                double b2=alpha*B(k,j+2);
                double b3=alpha*B(k,j+3);
                double b4=alpha*B(k,j+4);
                double b5=alpha*B(k,j+5);
                double b6=alpha*B(k,j+6);
                double b7=alpha*B(k,j+7);
                double a0=A(i,k), a1=A(i+1,k), a2=A(i+2,k), a3=A(i+3,k), a4=A(i+4,k), a5=A(i+5,k), a6=A(i+6,k), a7=A(i+7,k);
                
                c00 += b0*a0;c10 += b0*a1;c20 += b0*a2;c30 += b0*a3;c40 += b0*a4;c50 += b0*a5; c60 += b0*a6; c70 += b0*a7; 
                c01 += b1*a0;c11 += b1*a1;c21 += b1*a2;c31 += b1*a3;c41 += b1*a4;c51 += b1*a5; c61 += b1*a6; c71 += b1*a7; 
                c02 += b2*a0;c12 += b2*a1;c22 += b2*a2;c32 += b2*a3;c42 += b2*a4;c52 += b2*a5; c62 += b2*a6; c72 += b2*a7; 
                c03 += b3*a0;c13 += b3*a1;c23 += b3*a2;c33 += b3*a3;c43 += b3*a4;c53 += b3*a5; c63 += b3*a6; c73 += b3*a7;
                c04 += b4*a0;c14 += b4*a1;c24 += b4*a2;c34 += b4*a3;c44 += b4*a4;c54 += b4*a5; c64 += b4*a6; c74 += b4*a7;
                c05 += b5*a0;c15 += b5*a1;c25 += b5*a2;c35 += b5*a3;c45 += b5*a4;c55 += b5*a5; c65 += b5*a6; c75 += b5*a7;
                c06 += b6*a0;c16 += b6*a1;c26 += b6*a2;c36 += b6*a3;c46 += b6*a4;c56 += b6*a5; c66 += b6*a6; c76 += b5*a7;
                c07 += b7*a0;c17 += b7*a1;c27 += b7*a2;c37 += b7*a3;c47 += b7*a4;c57 += b7*a5; c67 += b7*a6; c77 += b5*a7;
            }
            C(i,j)=c00;   C(i+1, j)=c10;   C(i+2, j)=c20;   C(i+3, j)=c30;  C(i+4, j)=c40;  C(i+5, j)=c50;  C(i+6, j)=c60;  C(i+7, j)=c70;
            C(i,j+1)=c01; C(i+1, j+1)=c11; C(i+2, j+1)=c21; C(i+3, j+1)=c31;C(i+4, j+1)=c41;C(i+5, j+1)=c51;C(i+6, j+1)=c61;C(i+7, j+1)=c71;
            C(i,j+2)=c02; C(i+1, j+2)=c12; C(i+2, j+2)=c22; C(i+3, j+2)=c32;C(i+4, j+2)=c42;C(i+5, j+2)=c52;C(i+6, j+2)=c62;C(i+7, j+2)=c72;
            C(i,j+3)=c03; C(i+1, j+3)=c13; C(i+2, j+3)=c23; C(i+3, j+3)=c33;C(i+4, j+3)=c43;C(i+5, j+3)=c53;C(i+6, j+3)=c63;C(i+7, j+3)=c73;
            C(i,j+4)=c04; C(i+1, j+4)=c14; C(i+2, j+4)=c24; C(i+3, j+4)=c34;C(i+4, j+4)=c44;C(i+5, j+4)=c54;C(i+6, j+4)=c64;C(i+7, j+4)=c74;
            C(i,j+5)=c05; C(i+1, j+5)=c15; C(i+2, j+5)=c25; C(i+3, j+5)=c35;C(i+4, j+5)=c45;C(i+5, j+5)=c55;C(i+6, j+5)=c65;C(i+7, j+5)=c75;
            C(i,j+6)=c06; C(i+1, j+6)=c16; C(i+2, j+6)=c26; C(i+3, j+6)=c36;C(i+4, j+6)=c46;C(i+5, j+6)=c56;C(i+6, j+6)=c66;C(i+7, j+6)=c76;
            C(i,j+7)=c07; C(i+1, j+7)=c17; C(i+2, j+7)=c27; C(i+3, j+7)=c37;C(i+4, j+7)=c47;C(i+5, j+7)=c57;C(i+6, j+7)=c67;C(i+7, j+7)=c77;

        }
    }
    if(M8!=M){
        kernel1(M-M8, N, K, alpha, &A(M8, 0), LDA, B, LDB, beta, &C(M8, 0), LDC);
    }
    if(N8!=N){
        kernel1(M8, N-N8, K, alpha, A, LDA, B+N8, LDB, beta, C+N8, LDC);
    }
}