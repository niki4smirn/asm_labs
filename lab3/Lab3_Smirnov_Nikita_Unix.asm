                    global AsmBitCount
                    global AsmFactorial
                    global AsmIsSquare
                    global AsmRemoveDigits
                    global AsmFormula
                    global AsmBankDeposit
                    global AsmEvenDivisors
                    global AsmInfiniteManipulations

                    section .text

AsmBitCount:        ; x in rdi
                    xor rax, rax
.loop_begin:
                    cmp rdi, 0
                    je .return
                    mov rdx, 1
                    and rdx, rdi
                    add rax, rdx
                    shr rdi, 1
                    jmp .loop_begin
.return:
                    ret

AsmFactorial:       ; x in rdi
                    mov rcx, 1
                    mov rax, 1
.loop_begin:
                    cmp rax, rdi
                    jg .return

                    inc rcx
                    mul rcx

                    jmp .loop_begin
.return:
                    ret

AsmIsSquare:        ; x in rdi
                    ; l in r8, mid in r9, r in r10
                    mov r8, 1
                    mov r10, rdi
.loop_begin:
                    mov r9, r8
                    add r9, r10
                    shr r9, 1

                    cmp r8, r10
                    jge .loop_end

                    mov r11, 0xffffffff
                    cmp r9, r11
                    jg .dec_branch

                    mov rax, r9
                    mul rax

                    cmp rax, rdi
                    jl .inc_branch

.dec_branch:
                    mov r10, r9
                    jmp .after_branches
.inc_branch:
                    mov r8, r9
                    inc r8
.after_branches:
                    jmp .loop_begin

.loop_end:
                    mov rax, r9
                    mul rax
                    cmp rax, rdi
                    jne .false
.true:
                    mov rax, 1
                    jmp .return
.false:
                    xor rax, rax
.return:
                    ret

AsmRemoveDigits:    ; ans in r8
                    xor r8, r8
                    movsx rax, edi
                    ; x in rax
                    mov r10, 10
                    mov r11, 1
                    ; multiplier in r11

                    ; sign flag in rcx
                    xor rcx, rcx
                    cmp rax, 0
                    jg .loop_begin

                    neg rax
                    mov rcx, 1
.loop_begin:
                    cmp rax, 0
                    je .loop_end

                    cqo
                    div r10

                    mov r9, 1
                    and r9, rdx

                    cmp r9, 0
                    je .loop_begin

                    imul rdx, r11
                    add r8, rdx

                    imul r11, r10
                    jmp .loop_begin

.loop_end:
                    mov rax, r8

                    cmp rcx, 1
                    jne .return

                    neg rax
.return:
                    ret

AsmFormula:
                    ; YOUR_CODE_HERE
                    ret

AsmBankDeposit:
                    ; YOUR_CODE_HERE
                    ret

AsmEvenDivisors:
                    ; YOUR_CODE_HERE
                    ret

AsmInfiniteManipulations:
                    ; YOUR_CODE_HERE
                    ret