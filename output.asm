.data
	name: .string "Quarzum"
.text
.globl _start
_start:
	movq $name, %rdi
	call out
_main:
	movq $60, %rax
	movq $0, %rdi
	syscall
