.data
	v0: .string "Quarzum works!!!"
	i: .byte 0
	v1: .string "newQuarzum.qz"
	v2: .string "File created"
.text
.globl _start
_start:
	mov $v0, %rdi
	call out
	mov %r10, i
	call input
	call time
	mov $v1, %rdi
	call createFile
	mov $v2, %rdi
	call out
	mov %rax, %rdi
	mov $60, %rax
	syscall
_main:
