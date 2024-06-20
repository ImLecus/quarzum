.data
	i: .byte 0
.text
.globl _start
_start:
	mov $4, %r8
	movb $60, %al
	xorq %rdi, %rdi
	syscall
_main:
