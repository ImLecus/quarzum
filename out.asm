.data
.global _s0
_s0: .string "Hello world!"
.bss
.text
.global _start
_start:
call main
movq $60, %rax
movq $0, %rdi
syscall
main:
movq $_s0,%rdi
call out
leave
ret
