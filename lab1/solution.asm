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

CheckOverflow:      mov r8, 0
                    mov rcx, 0
                    mov rax, rdi

                    ; x^2
                    imul rdi
                    seto cl
                    or r8, rcx

                    ; x^3
                    imul rdi
                    seto cl
                    or r8, rcx

                    ; x^4
                    imul rdi
                    seto cl
                    or r8, rcx

                    mov rax, rsi
                    ; y^2
                    imul rsi
                    seto cl
                    or r8, rcx

                    mov rax, r8
                    ret

ComputeFn:          ; YOUR_CODE_HERE
                    ret

Clock:              ; YOUR_CODE_HERE
                    ret

Polynom:            ; YOUR_CODE_HERE
                    ret