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
                    ; y^3 in rax

                    sub r8, rax
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

Clock:
                    ; h in edi
                    ; m in esi
                    ; f in edx
                    mov eax, edx
                    mov r8d, 120
                    mul r8d
                    mov r9d, eax
                    ; f * 120 in r9d

                    mov eax, esi
                    mov r8d, 660
                    mul r8d
                    mov esi, eax
                    ; m * 660 in esi

                    mov eax, edi
                    mov r8d, 3600
                    mul r8d
                    ; h * 3600 in eax

                    sub eax, esi
                    ; deg in eax

                    add eax, r9d
                    ; dist in eax

                    add eax, 43200
                    xor edx, edx
                    mov r8d, 43200
                    div r8d
                    mov eax, edx
                    xor edx, edx
                    mov r8d, 11
                    div r8d
                    ret

Polynom:            ; x in edi
                    movsx rdi, edi
                    mov rax, rdi
                    mov rcx, 2
                    mul rcx
                    sub rax, 3
                    ; (2x - 3) in rax

                    mul rdi
                    add rax, 4
                    ; (2x^2 - 3x + 4) in rax

                    mul rdi
                    sub rax, 5
                    ; (2x^3 - 3x^2 + 4x - 5) in rax

                    mul rdi
                    add rax, 6
                    ; (2x^4 - 3x^3 + 4x^2 - 5x + 6) in rax
                    ret
