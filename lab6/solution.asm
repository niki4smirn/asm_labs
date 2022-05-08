                    global AsmStrChr
                    global AsmStrCpy
                    global AsmStrNCpy

                    section .text
GetLen:
                    xor rax, rax
                    mov r8d, 0xFFFFFFFF
                    mov ecx, r8d
                    repne scasb
                    mov eax, r8d
                    sub eax, ecx
                    dec eax
                    ret

AsmStrChr:
                    push r12
                    push r13

                    mov r12b, sil
                    mov r13, rdi

                    call GetLen

                    mov r8d, eax
                    ; str size in r8d
                    mov ecx, r8d

                    mov al, r12b
                    mov rdi, r13
                    mov r9, rdi

                    pop r13
                    pop r12

                    cld
                    repne scasb
                    jnz .not_found
                    sub r8d, ecx
                    dec r8d
                    add r9, r8
                    mov rax, r9
                    jmp .return
.not_found:
                    mov rax, 0
.return:
                    ret

AsmStrCpy:
                    push r12
                    push r13

                    mov r12, rsi
                    mov r13, rdi

                    mov rdi, rsi

                    call GetLen

                    mov rsi, r12
                    mov rdi, r13

                    pop r13
                    pop r12

                    mov ecx, eax
                    rep movsb

                    ret

AsmStrNCpy:
                    push r12
                    push r13
                    push r14

                    mov r12, rdi
                    mov r13, rsi
                    mov r14d, edx

                    call GetLen

                    cmp eax, r14d
                    jge .no_end_zero1
                    mov BYTE [r12 + r14], 0

.no_end_zero1:
                    mov rdi, r13

                    call GetLen

                    cmp eax, r14d
                    jge .no_end_zero2
                    mov BYTE [r12 + rax], 0

.no_end_zero2:

                    mov rdi, r12
                    mov rsi, r13
                    mov ecx, r14d

                    cmp ecx, eax
                    jle .size_le
                    mov ecx, eax

.size_le:

                    rep movsb

                    pop r14
                    pop r13
                    pop r12

                    ret
