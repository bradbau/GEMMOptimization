# Implementation map to dgemm 

## test framework/drive

Using one main file to store the testing framework. All supporting funciton are implemented in utils module.

## benchmark

intel mkl


## analysis

- Use a 2 level memory model to analysis.

number of memory references of non-blocked gemm 
= n3 to read each column of B n times
+ n2 to read each row of A once
+ 2n2 to read and write each element of C once
= n3 + 3n2

number of memory references of blocked gemm, NxN blocks each of size bxb
= N\*n2 read each block of B N3 times (N3 \* b2 = N3 \* (n/N)2 = N\*n2)
+ N\*n2 read each block of A N3 times
+ 2n2 read and write each block of C once
= (2N + 2) \* n2




## optimization map

### register reuse

In order to decrease the memory access of A, B and C, we can use registers to store local result of C. This will decrase the store and load of C.

### blocking



- important papers to understand cache blocking
https://www.cs.utexas.edu/~flame/pubs/GotoTOMS_revision.pdf
http://jianyuhuang.com/papers/sc16.pdf

- the sequence we use in cache blocking 
ijk/ikj, jik/jki, kij/kji have different 

cache size of object machine intel 4216 Xeon processor is L1 - 1 MB, L2 - 16 MB, L3 - 22 MB。
So L1 cache can store 4096 fp64 data, L2 cache can store 65536 fp64 data, L3 cache can store 90112 fp64 data.

L1-32k, L2-1024k, L3-22528k.

In the process of calculating, the innermost loop would maintain a block of one matrix and keep substituting blocks from two matrix. All three blocks should be kept in L1 cache. One block of A occupys m_block*k_block, so as the other two matrixes.


**question:** Is there a difference for the loop sequence in blocking? Like the difference in keep block of A, B, or C in L1 cache for longest time?
 


### inline asm


When doing 8*8 blocking with avx512 intrinsics, we can observe tremendous amount of register spill.
```asm
 movq    -112(%rsp), %rax                # 8-byte Reload
        leaq    (%r15,%rax), %rdi
        movq    184(%rsp), %rax
        movq    %rdi, -8(%rsp)                  # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm3
        movq    -72(%rsp), %rdi                 # 8-byte Reload
        addq    %r15, %rdi
        movq    %rdi, -16(%rsp)                 # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm4
        movq    24(%rsp), %rdi                  # 8-byte Reload
        leaq    (%r15,%rdi), %rdi
        movq    %rdi, -24(%rsp)                 # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm5
        movq    16(%rsp), %rdi                  # 8-byte Reload
        leaq    (%r15,%rdi), %rdi
        movq    %rdi, -32(%rsp)                 # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm6
        movq    112(%rsp), %rdi                 # 8-byte Reload
        leaq    (%r15,%rdi), %rdi
        movq    %rdi, -80(%rsp)                 # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm7
        movq    104(%rsp), %rdi                 # 8-byte Reload
        leaq    (%r15,%rdi), %rdi
        movq    %rdi, -40(%rsp)                 # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm8
        movq    96(%rsp), %rdi                  # 8-byte Reload
        addq    %r15, %rdi
        movq    %rdi, -48(%rsp)                 # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm9
        movq    88(%rsp), %rdi                  # 8-byte Reload
        addq    %r15, %rdi
        movq    %rdi, -56(%rsp)                 # 8-byte Spill
        vmovupd (%rax,%rdi,8), %zmm10
```
The above code is corresponding to part of asm code from line 17 to 24 in kernel7 (compiled by x86 clang 11.2, it's similar on gcc). we can see that the load of element c is restricted to use of very few 64-bit registers. The base algorithm used for register allocation in clang/llvm or gcc compiler are both graph coloring algorithm. And it always need to strike a balance between less use of register for a block and less load within the block.

In this case, we can manually improve the situation by inline asm.

- hierarchical address load 


## misc

- broadcast for double to zmm*

