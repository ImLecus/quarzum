.data
	.v0: .string "..hello?"
	i: .long 44
	.v1: .string "...world!"
.text
.globl _start
_start:
	mov $.v0, %rdi
	call out
	mov $1, %r10
	cmp $1, %r10
	setz %r10b
	cmpb $1, %r10b
	je .l0
	jmp .c0
.l0:
	mov $.v1, %rdi
	call out
.c0:
_main:
	movq $60, %rax
	movq $0, %rdi
	syscall
