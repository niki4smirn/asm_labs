                    global AsmStrChr
                    global AsmStrCpy
                    global AsmStrNCpy
                    global AsmStrCat
                    global AsmStrStr

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
                    cld
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

                    cld
                    rep movsb

                    pop r14
                    pop r13
                    pop r12

                    ret

AsmStrCat:
                    push r12
                    push r13
                    push r14

                    mov r12, rdi
                    mov r13, rsi
                    mov r14, rdi

                    call GetLen

                    add r12, rax

                    mov rdi, r13

                    call GetLen

                    mov ecx, eax
                    mov rax, r14

                    mov rdi, r12
                    mov rsi, r13

                    cld
                    rep movsb

                    mov BYTE [rdi], 0

                    pop r14
                    pop r13
                    pop r12
                    ret

AsmStrStr:
                    push r12
                    push r13
                    push r14

                    mov r12, rdi
                    mov r13, rsi

                    call GetLen

                    mov r14, rax

                    mov rdi, r13

                    call GetLen

                    xor r8, r8
                    sub r14, rax
                    inc eax
.loop_begin:
                    cmp r8, r14
                    jg .loop_end

                    mov ecx, eax
                    lea rdi, [r12 + r8]
                    mov rsi, r13

                    cld
                    repe cmpsb
                    jecxz .found

                    inc r8
                    jmp .loop_begin
.loop_end:
                    xor rax, rax
                    jmp .end
.found:
                    lea rax, [r12 + r8]
.end:
                    pop r14
                    pop r13
                    pop r12
                    ret
