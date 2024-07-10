.data   
    _newline: .byte '\n'
    .global true
    true: .byte 1
    .global false
    false: .byte 0
    O_RDONLY = 0x0000
    O_WRONLY = 0x0001
    O_RDWR = 0x0002
    O_CREAT = 0x0040
    O_TRUNC = 0x0200
.bss
    _input_buffer: .space 256
    _timeval: .space 16
.text

.global len
.global out
.global input
.global time
.global strcat

#
#   uint len(char* string)
#   Returns the length of the string
#
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

#
#   char[256] input();
#   Reads the console and stores the content inside _input_buffer
#
input:
    enter $0, $0
    push %rsi
    push %rdx
    push %rdi
    xorq %rax, %rax
    xorq %rdi, %rdi
    leaq _input_buffer, %rsi
    movq $256, %rdx
    syscall
    leaq _input_buffer, %rax
    pop %rdx
    pop %rsi
    pop %rdi
    leave
    ret

#
# uint time();
# Returns the number of seconds since 1/1/1970
#
time:
    enter $0, $0

    movq $96, %rax
    leaq _timeval, %rdi
    xorq %rsi, %rsi
    syscall
    leaq _timeval, %rax

    leave
    ret
#
# function strcat(string str1, string str2);
# Modifies the first string, adding the second.
#
strcat:
    enter $0, $0
    push %rcx
    call len
    addq %rax, %rdi
    strcat.add:
    movb (%rsi), %cl
    movb %cl, (%rdi)
    cmpb $0, %cl
    je strcat.end
    incq %rdi
    incq %rsi
    jmp strcat.add
    strcat.end:
    pop %rcx
    leave
    ret
