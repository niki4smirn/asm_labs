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

ComputeFn:          ; x in rdi, y in rsi
                    mov rax, rsi
                    imul rsi
                    mov rcx, rax
                    ; y^2 in rcx

                    mov rax, rdi
                    imul rdi
                    mov r8, rax
                    ; x^2 in r8

                    mov rax, rcx
                    imul rsi
                    mov r9, 0xFFFFFFFFFFFFFFFF
                    xor rax, r9
                    add rax, 1
                    ; -y^3 in rax

                    add r8, rax
                    add r8, 2
                    ; (2 + x^3 - y^3) in r8

                    mov rax, rsi
                    imul rsi
                    add rax, 2
                    imul rax
                    ; (y^2 + 2)^2 in rax

                    imul r8
                    ; (2 + x^3 - y^3)(y^2 + 2)^2 in rax

                    sub rdi, rcx
                    ; x-y^2 in rdi

                    idiv rdi
                    ret

Clock:              ; YOUR_CODE_HERE
                    ret

Polynom:            ; YOUR_CODE_HERE
                    ret