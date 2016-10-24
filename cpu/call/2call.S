section call_test vstart=0x900
    mov word [addr], near_proc
    call [addr]

jmp $

addr dd 4

near_proc:
    mov ax, 0x1234
    ret
