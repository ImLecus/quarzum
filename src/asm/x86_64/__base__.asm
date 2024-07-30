.data
_newline: .byte '\n'
.global true
true: .byte 1
.global false
false: .byte 0

.text
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
