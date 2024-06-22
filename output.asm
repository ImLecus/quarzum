.data
	name: .string "hola"
	.v0: .string "caracola"
	.v1: .long 5
.text
.globl _start
_start:
	movq $name, %rdi
	movq $.v0, %rsi
	call strcat
	movq $name, %rdi
	call out
	movq $.v1, %rdi
	call wait
_main:
	movq $60, %rax
	movq $0, %rdi
	syscall
