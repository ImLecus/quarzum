.data
	.v0: .string "Goodbye world"
	.v1: .string "Hello world!"
.text
.globl _start
_start:
	cmpq $1, false
	je .l0
	mov $.v0, %rdi
	call out
	jmp .c0
.l0:
	mov $.v1, %rdi
	call out
.c0:
_main:
