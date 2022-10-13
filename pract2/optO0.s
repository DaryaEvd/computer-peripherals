CalcSin:
  pushq %rbp
  movq %rsp, %rbp
  movsd %xmm0, -40(%rbp)
  movq %rdi, -48(%rbp)
  pxor %xmm0, %xmm0
  movsd %xmm0, -8(%rbp)
  movsd -40(%rbp), %xmm1
  movsd .LC1(%rip), %xmm0
  mulsd %xmm1, %xmm0
  movsd .LC2(%rip), %xmm1
  divsd %xmm1, %xmm0
  movsd %xmm0, -40(%rbp)
  movsd -40(%rbp), %xmm0
  movsd %xmm0, -16(%rbp)
  movq $1, -24(%rbp)
  jmp .L2
.L3:
  movsd -8(%rbp), %xmm0
  addsd -16(%rbp), %xmm0
  movsd %xmm0, -8(%rbp)
  movsd -16(%rbp), %xmm0
  mulsd -40(%rbp), %xmm0
  mulsd -40(%rbp), %xmm0
  movq .LC3(%rip), %xmm1
  xorpd %xmm1, %xmm0
  movq -24(%rbp), %rax
  leaq 1(%rax), %rdx
  movq -24(%rbp), %rax
  addq $2, %rax
  imulq %rdx, %rax
  pxor %xmm1, %xmm1
  cvtsi2sdq %rax, %xmm1
  divsd %xmm1, %xmm0
  movsd %xmm0, -16(%rbp)
  addq $2, -24(%rbp)
.L2:
  movq -48(%rbp), %rax
  addq %rax, %rax
  cmpq %rax, -24(%rbp)
  jl .L3
  movsd -8(%rbp), %xmm0
  movq %xmm0, %rax
  movq %rax, %xmm0
  popq %rbp
  ret
.LC4:
  .string "Bad input. Enter x and n in command line"
.LC5:
  .string "%lf\n"
.LC7:
  .string "Time taken: %lf sec.\n"
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $80, %rsp
  movl %edi, -68(%rbp)
  movq %rsi, -80(%rbp)
  leaq -48(%rbp), %rax
  movq %rax, %rsi
  movl $4, %edi
  call clock_gettime
  cmpl $1, -68(%rbp)
  jne .L6
  movl $.LC4, %edi
  movl $0, %eax
  call printf
  movl $0, %eax
  jmp .L8
.L6:
  movq -80(%rbp), %rax
  addq $8, %rax
  movq (%rax), %rax
  movq %rax, %rdi
  call atof
  movq %xmm0, %rax
  movq %rax, -8(%rbp)
  movq -80(%rbp), %rax
  addq $16, %rax
  movq (%rax), %rax
  movq %rax, %rdi
  call atoll
  movq %rax, -16(%rbp)
  movq -16(%rbp), %rdx
  movq -8(%rbp), %rax
  movq %rdx, %rdi
  movq %rax, %xmm0
  call CalcSin
  movq %xmm0, %rax
  movq %rax, -24(%rbp)
  movq -24(%rbp), %rax
  movq %rax, %xmm0
  movl $.LC5, %edi
  movl $1, %eax
  call printf
  leaq -64(%rbp), %rax
  movq %rax, %rsi
  movl $4, %edi
  call clock_gettime
  movq -64(%rbp), %rdx
  movq -48(%rbp), %rax
  subq %rax, %rdx
  pxor %xmm1, %xmm1
  cvtsi2sdq %rdx, %xmm1
  movq -56(%rbp), %rdx
  movq -40(%rbp), %rax
  subq %rax, %rdx
  pxor %xmm2, %xmm2
  cvtsi2sdq %rdx, %xmm2
  movsd .LC6(%rip), %xmm0
  mulsd %xmm2, %xmm0
  addsd %xmm0, %xmm1
  movq %xmm1, %rax
  movq %rax, %xmm0
  movl $.LC7, %edi
  movl $1, %eax
  call printf
  movl $0, %eax
.L8:
  leave
  ret
.LC1:
  .long 1413753926
  .long 1074340347
.LC2:
  .long 0
  .long 1080459264
.LC3:
  .long 0
  .long -2147483648
  .long 0
  .long 0
.LC6:
  .long -400107883
  .long 1041313291
