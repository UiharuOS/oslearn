section test_mov vstart=0
    mov ax, 0x1234
    mov ax, [var]

jmp $

section my_data
    var dd 1
