                    global AsmProduct
                    global AsmSpecialSum
                    global AsmArrayFormula
                    global AsmCompare
                    global AsmSimpleModify
                    global AsmSetToSequence
                    global AsmRotateInGroups
                    global AsmRemoveIfSimilar

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

AsmCompare:         ; &array1 in rdi
                    ; size1 in rsi
                    ; &array2 in rdx
                    ; size2 in rcx
                    ; ans in rax
                    mov rax, rsi
.ext_loop_begin:
                    cmp rsi, 0
                    je .ext_loop_end

                    dec rsi
                    ; array2_pos in r8
                    xor r8, r8
                    ; was in r9
                    xor r9, r9

                    lea r10, [rdi + 8 * rsi]
                    mov r10, [r10]

.int_loop_begin:
                    cmp r8, rcx
                    je .int_loop_end

                    lea r11, [rdx + 8 * r8]
                    mov r11, [r11]

                    cmp r10, r11
                    jne .bad
                    mov r9, 1
.bad:
                    inc r8

                    jmp .int_loop_begin
.int_loop_end:

                    sub rax, r9

                    jmp .ext_loop_begin

.ext_loop_end:
                    ret

AsmSimpleModify:    ; &array in rdi
                    movsx rsi, esi
                    dec rsi
                    ; cur_pos in rsi
.loop_begin:
                    cmp rsi, 0
                    jl .loop_end

                    lea rax, [rdi + 4 * rsi]
                    mov eax, DWORD [rax]
                    ; cur_element in eax

                    mov r10d, 5

                    cdq
                    idiv r10d

                    cmp edx, 0
                    jg .after_add
                    je .first_case
                    add edx, r10d
.after_add:
                    and edx, 1
                    cmp edx, 0
                    je .second_case
                    jmp .third_case
                    ; case_ans in r9d
.first_case:
                    mov r9d, 0
                    jmp .after_cases
.second_case:
                    mov r9d, 1
                    jmp .after_cases
.third_case:
                    mov r9d, -1
                    jmp .after_cases
.after_cases:
                    lea rax, [rdi + 4 * rsi]
                    mov DWORD [rax], r9d

                    dec rsi
                    jmp .loop_begin
.loop_end:
                    ret

AsmSetToSequence:   ; &array in rdi
                    movsx rsi, esi
                    dec rsi
                    ; cur_pos in rsi
                    mov rcx, QWORD [rdi]
                    ; min in rcx
                    mov rdx, QWORD [rdi]
                    ; max in rdx
                    xor r8, r8
                    ; min_pos in r8
                    xor r9, r9
                    ; max_pos in r9
.minmax_loop_begin:
                    cmp rsi, 0
                    jl .minmax_loop_end

                    lea rax, [rdi + 8 * rsi]
                    mov rax, QWORD [rax]
                    ; cur_element in rax

                    cmp rax, rcx
                    jl .update_min

                    cmp rax, rdx
                    jg .update_max

                    jmp .after_updates

.update_min:
                    mov rcx, rax
                    mov r8, rsi
                    jmp .after_updates

.update_max:
                    mov rdx, rax
                    mov r9, rsi
                    jmp .after_updates
.after_updates:
                    dec rsi
                    jmp .minmax_loop_begin
.minmax_loop_end:

                    cmp r8, r9
                    jle .after_xchg
                    xchg r8, r9

.after_xchg:
                    mov r10, r8
                    ; r10 - cur_pos
                    mov rax, r9
.fill_loop_begin:
                    cmp r10, r9
                    jg .fill_loop_end

                    lea rax, [rdi + 8 * r10]
                    ; &cur_element in rax
                    mov QWORD [rax], r10
                    sub QWORD [rax], r8
                    inc QWORD [rax]

                    inc r10
                    jmp .fill_loop_begin
.fill_loop_end:
                    ret

AsmRotateInGroups:  ; &array in rdi
                    movsx rsi, esi
                    ; size in rsi
                    movsx rdx, edx
                    ; k in rdx

                    xor r8, r8
                    ; i in r8

.group_loop_begin:
                    cmp r8, rsi
                    jnl .group_loop_end

                    mov r10, r8
                    add r10, rdx
                    cmp r10, rsi
                    jng .after_decrease
                    mov r10, rsi
.after_decrease:
                    ; bound in r10
                    lea rax, [rdi + 8 * r8]
                    mov r11, QWORD [rax]
                    ; first in r11

                    mov r9, r8
                    ; j in r9
.rotate_loop_begin:
                    cmp r9, r10
                    jnl .rotate_loop_end

                    lea rax, [rdi + 8 * r9 + 8]
                    mov rcx, QWORD [rax]

                    lea rax, [rdi + 8 * r9]
                    mov QWORD [rax], rcx

                    inc r9
                    jmp .rotate_loop_begin
.rotate_loop_end:

                    dec r9
                    lea rax, [rdi + 8 * r9]
                    mov QWORD [rax], r11

                    add r8, rdx
                    jmp .group_loop_begin
.group_loop_end:
                    ret

AsmRemoveIfSimilar: ; &array in rdi
                    movsx rsi, esi
                    ; size in rsi
                    ; x in rdx
                    ; d in rcx
                    xor r9, r9
                    ; insert_pos in r9
                    xor r10, r10
                    ; cur_pos in r10
.loop_begin:
                    cmp r10, rsi
                    jge .loop_end

                    lea r11, [rdi + 8 * r10]
                    mov r11, QWORD [r11]
                    ; cur_val in r11
                    cmp r11, 0
                    jl .insert

                    mov r8, r11
                    add r8, rcx
                    jo .after_second_check
                    cmp r8, rdx
                    jo .after_second_check
                    jl .insert

.after_second_check:
                    mov r8, r11
                    sub r8, rcx
                    jo .after_third_check
                    cmp r8, rdx
                    jo .after_third_check
                    jg .insert

.after_third_check:
                    mov r8, r11
                    and r8, 1
                    cmp r8, 0
                    je .insert

                    jmp .after_insert
.insert:
                    lea rax, [rdi + 8 * r9]
                    mov QWORD [rax], r11
                    inc r9

.after_insert:
                    inc r10
                    jmp .loop_begin
.loop_end:
                    mov rax, r9
                    ret
