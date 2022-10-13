CalcSin:
  mulsd .LC1(%rip), %xmm0
  addq %rdi, %rdi
  divsd .LC2(%rip), %xmm0
  cmpq $1, %rdi
  jle .L4
  movq .LC3(%rip), %xmm4
  movapd %xmm0, %xmm1
  movl $1, %eax
  pxor %xmm2, %xmm2
.L3:
  addsd %xmm1, %xmm2
  mulsd %xmm0, %xmm1
  leaq 1(%rax), %rdx
  addq $2, %rax
  imulq %rax, %rdx
  pxor %xmm3, %xmm3
  cvtsi2sdq %rdx, %xmm3
  mulsd %xmm0, %xmm1
  xorpd %xmm4, %xmm1
  divsd %xmm3, %xmm1
  cmpq %rdi, %rax
  jl .L3
  movapd %xmm2, %xmm0
  ret
.L4:
  pxor %xmm2, %xmm2
  movapd %xmm2, %xmm0
  ret
.LC4:
  .string "Bad input. Enter x and n in command line"
.LC5:
  .string "%lf\n"
.LC7:
  .string "Time taken: %lf sec.\n"
main:
  pushq %rbp
  movl %edi, %ebp
  movl $4, %edi
  pushq %rbx
  movq %rsi, %rbx
  subq $56, %rsp
  leaq 16(%rsp), %rsi
  call clock_gettime
  cmpl $1, %ebp
  je .L15
  movq 8(%rbx), %rdi
  xorl %esi, %esi
  call strtod
  movq 16(%rbx), %rdi
  movl $10, %edx
  xorl %esi, %esi
  movsd %xmm0, 8(%rsp)
  call strtoll
  movsd 8(%rsp), %xmm0
  mulsd .LC1(%rip), %xmm0
  addq %rax, %rax
  divsd .LC2(%rip), %xmm0
  cmpq $1, %rax
  jle .L12
  movq .LC3(%rip), %xmm4
  movapd %xmm0, %xmm1
  movl $1, %edx
  pxor %xmm2, %xmm2
.L11:
  addsd %xmm1, %xmm2
  mulsd %xmm0, %xmm1
  leaq 1(%rdx), %rcx
  addq $2, %rdx
  imulq %rdx, %rcx
  pxor %xmm3, %xmm3
  cvtsi2sdq %rcx, %xmm3
  mulsd %xmm0, %xmm1
  xorpd %xmm4, %xmm1
  divsd %xmm3, %xmm1
  cmpq %rax, %rdx
  jl .L11
.L10:
  movapd %xmm2, %xmm0
  movl $.LC5, %edi
  movl $1, %eax
  call printf
  movl $4, %edi
  leaq 32(%rsp), %rsi
  call clock_gettime
  movq 40(%rsp), %rax
  pxor %xmm0, %xmm0
  subq 24(%rsp), %rax
  cvtsi2sdq %rax, %xmm0
  pxor %xmm1, %xmm1
  movq 32(%rsp), %rax
  subq 16(%rsp), %rax
  mulsd .LC6(%rip), %xmm0
  cvtsi2sdq %rax, %xmm1
  movl $.LC7, %edi
  movl $1, %eax
  addsd %xmm1, %xmm0
  call printf
.L9:
  addq $56, %rsp
  xorl %eax, %eax
  popq %rbx
  popq %rbp
  ret
.L15:
  movl $.LC4, %edi
  xorl %eax, %eax
  call printf
  jmp .L9
.L12:
  pxor %xmm2, %xmm2
  jmp .L10
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
