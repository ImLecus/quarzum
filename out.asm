.data
.global pi
pi: .byte 3.14
.global e
e: .byte 2.7
.global .s0
.s0: .string "a"
.bss
.text
.global _start
_start:
call main
movq $60, %rax
movq $0, %rdi
syscall
out:
leave
ret
input:
leave
ret
error:
leave
ret
main:
.b0:
cmpb $1, true
jne .c0
movq $.s0,%rdi
call out
jmp .b0
.c0:
leave
ret
