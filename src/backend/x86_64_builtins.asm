#
#   uint len(char* string)
#   Returns the length of the string
#
.global len
len:
    enter $0, $0
    push %rsi
    xorq %rax, %rax
    movq %rdi, %rsi
    len_loop:
        cmpb $0, (%rsi, %rax,1)
        je len_end
        incq %rax
        jmp len_loop
    len_end:
    pop %rsi
    leave
    ret
#
#   function out(char* message)
#   Displays a message
#
.global out
out:
    enter $0, $0
    push %rdx
    push %rcx
    call len
    movq %rax, %rdx
    movq %rdi, %rcx
    movq $1, %rax
    movq $1, %rdi
    syscall

    pop %rcx
    pop %rdx
    leave
    ret

