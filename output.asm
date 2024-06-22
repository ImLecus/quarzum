.data
	i: .long 0
	.v0: .string "i"
.text
.globl _start
_start:
.l0:
	mov i, %r10b
	cmp $5, %r10b
	setl %r10b
	cmpb $1, %r10b
	je .l1
	jmp .c0
.l1:
	movq $.v0, %rdi
	call out
	mov i, %r10d
	add $1, %r10d
	mov %r10d, i
	jmp .l0
.c0:
_main:
	movq $60, %rax
	movq $0, %rdi
	syscall
