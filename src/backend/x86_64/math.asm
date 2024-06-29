.data
.text

.global abs
abs:
    enter $0, $0
    cmp $0, %rdi
    jg abs.end
    neg %rdi
    abs.end:
    leave
    ret