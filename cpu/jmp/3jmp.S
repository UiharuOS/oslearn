section jmp_test vstart=0x900
mov ax, start
jmp near ax
times 128 db 0
start:
    mov ax, 0x1234
    jmp $
