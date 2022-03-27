                    global AsmProduct
                    global AsmSpecialSum
                    global AsmArrayFormula

                    section .text

AsmProduct:         ; &array in rdi
                    movsx rsi, esi
                    ; size in rsi
                    movsx rcx, edx
                    ; module in rcx
                    ; ans in rax
                    mov rax, 1
.loop_begin:
                    dec rsi
                    ; &cur_element in r8
                    lea r8, [rdi + 4 * rsi]
                    movsx r9, DWORD [r8]
                    imul r9
                    cqo
                    idiv rcx
                    mov rax, rdx

                    cmp rax, 0
                    jnl .after_add

                    add rax, rcx

.after_add:
                    cmp rsi, 0
                    jg .loop_begin

                    ret

AsmSpecialSum:      ; &array in rdi
                    ; size in rsi
                    mov rcx, rdx
                    ; module in rcx
                    ; ans in r10
                    xor r10, r10
.loop_begin:
                    dec rsi
                    ; &cur_element in r8
                    lea r8, [rdi + 8 * rsi]
                    mov r8, QWORD [r8]
                    ; cur_element in r8

                    ; mod3 and mod5 check
                    mov r9, 3
                    mov rax, r8
                    cqo
                    idiv r9
                    add rdx, r9
                    mov rax, rdx
                    cqo
                    idiv r9
                    and rdx, 1
                    cmp rdx, 0
                    je .not_special

                    mov r9, 5
                    mov rax, r8
                    cqo
                    idiv r9
                    add rdx, r9
                    mov rax, rdx
                    cqo
                    idiv r9
                    and rdx, 1
                    cmp rdx, 0
                    je .not_special

                    ; cur_element % module
                    mov rax, r8
                    cqo
                    idiv rcx

                    cmp rdx, 0
                    jnl .after_add_element
                    add rdx, rcx

.after_add_element:
                    mov r8, rdx

                    add r10, r8
                    mov rax, r10
                    cqo
                    idiv rcx
                    cmp rdx, 0
                    jnl .after_add_ans
                    add rdx, rcx
.after_add_ans:
                    mov r10, rdx
.not_special:
                    cmp rsi, 0
                    jg .loop_begin

                    mov rax, r10

                    ret

AsmArrayFormula:    ; &array in rdi
                    movsx rsi, esi
                    ; size in rsi
                    ; counter in rcx
                    xor rcx, rcx
                    ; ans in r8
                    xor r8, r8
.loop_begin:
                    cmp rcx, rsi
                    jge .loop_end
                    lea r9, [rdi + 4 * rcx]
                    movsx r10, DWORD [r9]
                    inc rcx
                    imul r10, rcx

                    add r9, 4
                    movsx r11, DWORD [r9]
                    inc rcx
                    imul r11, rcx

                    imul r10, r11

                    mov r11, rcx
                    and r11, 3
                    cmp r11, 0
                    jne .not_neg

                    neg r10
.not_neg:

                    add r8, r10

                    jmp .loop_begin
.loop_end:
                    mov rax, r8

                    ret
