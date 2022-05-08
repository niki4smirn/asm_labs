                    global AsmStrChr
                    global AsmStrCpy
                    global AsmStrNCpy
                    global AsmStrCat
                    global AsmStrStr
                    global AsmStrToInt64
                    global AsmIntToStr64
                    global AsmSafeStrToUInt64

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
                    mov BYTE [rdi], 0

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


AsmStrToInt64:
                    xor rax, rax
                    mov r10, 10
                    xor rsi, rsi
                    cmp BYTE [rdi], '-'
                    jne .loop_begin
                    mov rsi, 1
                    inc rdi

.loop_begin:
                    cmp BYTE [rdi], 0
                    je .loop_end
                    movzx r8, BYTE [rdi]
                    sub r8, '0'

                    mul r10
                    add rax, r8

                    inc rdi
                    jmp .loop_begin
.loop_end:
                    cmp rsi, 1
                    jne .return
                    neg rax

.return:
                    ret

ReverseStr:
                    push r12
                    mov r12, rdi
                    call GetLen

                    xor r8, r8
                    mov r9, rax
                    dec r9
                    ; l in r8, r in r9
.loop_begin:
                    cmp r8, r9
                    jge .loop_end

                    xchg BYTE [r12 + r8], al
                    xchg BYTE [r12 + r9], al
                    xchg BYTE [r12 + r8], al

                    inc r8
                    dec r9

                    jmp .loop_begin
.loop_end:
                    pop r12
                    ret

IntToChar:
                    cmp dil, 10
                    jl .number

                    sub dil, 10
                    mov al, dil
                    add al, 'A'

                    jmp .return
.number:
                    mov al, dil
                    add al, '0'
.return:
                    ret

IsCorrectBase:
                    xor al, al

                    cmp edi, 2
                    je .correct

                    cmp edi, 8
                    je .correct

                    cmp edi, 10
                    je .correct

                    cmp edi, 16
                    je .correct


                    jmp .return
.correct:
                    mov al, 1
.return:
                    ret

AsmIntToStr64:
                    push r12
                    push r13
                    push r14
                    push r15
                    push rbx

                    mov r12, rdi
                    movsx r13, esi
                    mov r14, rdx
                    mov rbx, rdx

                    xor r15, r15
                    cmp r12, 0
                    jge .non_neg
                    mov r15, 1
                    neg r12
.non_neg:
                    mov edi, r13d

                    call IsCorrectBase

                    cmp al, 0
                    je .incorrect_base

.loop_begin:
                    cmp r12, 0
                    jle .loop_end

                    mov rax, r12
                    cqo
                    div r13
                    mov r12, rax
                    mov rdi, rdx
                    call IntToChar
                    mov BYTE [r14], al
                    inc r14

                    jmp .loop_begin
.loop_end:
                    cmp r14, rbx
                    jne .non_empty
                    mov BYTE [r14], '0'
                    inc r14
.non_empty:
                    cmp r15, 1
                    jne .non_neg2
                    mov BYTE [r14], '-'
                    inc r14
.non_neg2:
                    mov BYTE [r14], 0
                    mov rdi, rbx
                    call ReverseStr
                    jmp .return
.incorrect_base:
                    mov BYTE [r14], 0

.return:
                    pop rbx
                    pop r15
                    pop r14
                    pop r13
                    pop r12
                    ret

AsmSafeStrToUInt64:
                    xor rax, rax
                    mov r10, 10
                    cmp BYTE [rdi], '-'
                    je .bad

.loop_begin:
                    cmp BYTE [rdi], 0
                    je .loop_end

                    mul r10
                    jc .bad

                    movzx r8, BYTE [rdi]
                    sub r8, '0'

                    cmp r8, 0
                    jl .bad

                    cmp r8, 9
                    jg .bad

                    add rax, r8
                    jc .bad

                    inc rdi
                    jmp .loop_begin
.loop_end:
                    mov QWORD [rsi], rax
                    mov rax, 1
                    jmp .return
.bad:
                    xor rax, rax
.return:
                    ret
