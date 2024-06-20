.data
	v0: .string "Hello world!"
.text
.globl _start
_start:
	mov $v0, %rdi
	call out
_main:
