.data
.global name
name: .string "Quarzum"
.bss
.text
.global _start
_start:
call main
movq $60, %rax
movq $0, %rdi
syscall
main:
enter $0, $0
movq $name,%rdi
call out
leave
ret
