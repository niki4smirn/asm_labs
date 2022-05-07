                    global AsmStrChr

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
