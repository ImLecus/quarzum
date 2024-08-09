.data
.global j
j: .byte 0
.global .s0
.s0: .string "Hello world from Quarzum!"
.bss
.text
.global _start
_start:
call main
movq $60, %rax
movq $0, %rdi
syscall
mov %r10,1
add %r10,3
main:
enter $4, $0
movq $.s0,%rdi
call out
movq $1,%rdi
call exit
leave
ret
