call near near_proc  ; e8llhh
jmp $
addr dd 4
near_proc:
    ; function
    mov ax, 0x1234
        ret
