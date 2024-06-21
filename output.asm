.data
	.v0: .string "Hello"
.text
.globl _start
_start:
.l0:
	cmpq $1, false
	je .l1
	jmp .c0
.l1:
	mov $.v0, %rdi
	call out
	jmp .l0
.c0:
	call input
_main:
