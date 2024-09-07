.text 

.global openFile
openFile:
    enter $0, $0
    movq $2, %rax
    syscall
    leave
    ret

.global closeFile
closeFile:
    enter $0, $0
    movq $3, %rax
    syscall
    leave
    ret