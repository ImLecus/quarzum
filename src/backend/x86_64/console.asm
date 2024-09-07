.data
_newline: .byte '\n'

.text

.global out
out:
    enter $0, $0
    push %rdx
    push %rsi
    call len
    movq %rax, %rdx
    movq %rdi, %rsi
    movq $1, %rax
    movq $1, %rdi
    syscall

    movq $1, %rax
    movq $1, %rdi
    leaq _newline, %rsi
    movq $1, %rdx
    syscall

    pop %rsi
    pop %rdx
    leave
    ret

.global input
input:
    enter $0, $0

    leave
    ret
