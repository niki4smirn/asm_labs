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

AsmFactorial:
                    movsx rsi, edi
                    ; x in rsi
                    mov rcx, 1
                    mov rax, 1
.loop_begin:
                    cmp rax, rsi
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
                    cmp r8, r10
                    jge .loop_end

                    mov r9, r8
                    add r9, r10
                    shr r9, 1

                    mov r11, 0xffffffff
                    cmp r9, r11
                    jg .dec_branch

                    mov rax, r9
                    mul rax

                    cmp rax, rdi
                    jb .inc_branch

.dec_branch:
                    mov r10, r9
                    jmp .loop_begin
.inc_branch:
                    mov r8, r9
                    inc r8
                    jmp .loop_begin

.loop_end:
                    mov rax, r8
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

AsmFormula:         ; x in rdi, n in rsi
                    ; prev parentheses result in r8
                    mov r8, 1
                    ; result in rax
                    mov rax, 1
                    ; counter in rcx
                    mov rcx, 1
.loop_begin:
                    cmp rcx, rsi
                    jg .loop_end

                    inc rcx

                    imul r8, rdi
                    jo .overflow

                    mov r9, 1
                    and r9, rcx

                    cmp r9, 0
                    je .subtract

                    add r8, rcx
                    jo .overflow
                    jmp .after_subtract
.subtract:
                    sub r8, rcx
                    jo .overflow
.after_subtract:
                    imul rax, r8
                    jo .overflow

                    cmp rax, 0
                    jne .loop_begin
.loop_end:

                    jmp .return

.overflow:
                    mov rax, -1

.return:
                    ret

AsmBankDeposit:     ; x in rdi
                    movsx r8, esi
                    ; y in r8
                    movsx rcx, edx
                    ; z in rcx
.loop_begin:
                    cmp rcx, 0
                    je .loop_end

                    mov rax, rdi
                    mul r8

                    mov r10, 100
                    cqo
                    div r10
                    add rdi, rax

                    dec rcx

                    jmp .loop_begin

.loop_end:
                    mov rax, rdi
                    ret

AsmEvenDivisors:
                    movzx rdi, di
                    ; n in rdi
                    mov rcx, 1
                    ; m in rcx
                    xor r8, r8
                    ; ans in r8

.loop_begin:
                    cmp rcx, rdi
                    jge .loop_end

                    mov rax, rdi
                    cqo
                    div rcx

                    cmp rax, rdx
                    jne .after_add

                    inc r8
.after_add:
                    inc rcx
                    jmp .loop_begin
.loop_end:
                    mov rax, r8
                    ret

AsmInfiniteManipulations:
                    ; x in rdi
                    xor rax, rax
.loop_begin:
                    cmp rdi, 0
                    je .loop_end
                    mov rdx, 1
                    and rdx, rdi
                    add rax, rdx
                    shr rdi, 1
                    jmp .loop_begin
.loop_end:
                    mov rcx, rax
                    ; ones count in rcx
                    mov r8, 1
                    shl r8, cl
                    dec r8
                    ; min in r8

                    mov r9, r8
                    sub cl, 64
                    neg cl
                    shl r9, cl
                    ; max in r9

                    sub r9, r8
                    mov rax, r9
                    ret