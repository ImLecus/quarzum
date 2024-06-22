.data
	.v0: .string "..hello?"
	i: .long 2
	.v1: .string "..world!"
.text
.globl _start
_start:
	movq $.v0, %rdi
	call out
	mov $2, %r10d
	add $1, %r10d
	cmp i, %r10b
	setnz %r10b
	cmpb $1, %r10b
	je .l0
	jmp .c0
.l0:
	movq $.v1, %rdi
	call out
.c0:
_main:
	movq $60, %rax
	movq $0, %rdi
	syscall
