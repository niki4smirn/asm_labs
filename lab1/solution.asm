                    global Sum
                    global CheckOverflow
                    global ComputeFn
                    global Clock
                    global Polynom

                    section .text

Sum:                ; Data location:
                    ;           ( x,  y ) --> result
                    ; Unix/WSL: (EDI, SIL) --> RAX
                    ; Windows:  (ECX, DIL) --> RAX

                    movsx rax, edi
                    movzx rdx, sil
                    add rax, rdx
                    ret

CheckOverflow:      ; YOUR_CODE_HERE
                    ret

ComputeFn:          ; YOUR_CODE_HERE
                    ret

Clock:              ; YOUR_CODE_HERE
                    ret

Polynom:            ; YOUR_CODE_HERE
                    ret