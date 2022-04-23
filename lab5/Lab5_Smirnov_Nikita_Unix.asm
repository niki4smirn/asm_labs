                    global AsmFindNearest
                    global Metric
                    global AsmSummarizeRows
                    global CalculateArraySum

                    section .text

Metric:
                    sub rdi, rdx
                    imul rdi, rdi
                    mov rax, rdi
                    sub rsi, rcx
                    imul rcx, rcx
                    add rax, rcx
                    ret


AsmFindNearest:
                    cmp rdx, 1
                    je .no_r9
                    push r9
.no_r9:
                    push rbx
                    push r12
                    push r13
                    push r14
                    push r15

                    sub rsp, 8

                    mov QWORD [rsp], 1

                    mov r12, rdi
                    ; x in r12
                    mov r13, rsi
                    ; y in r13
                    mov r14, rdx
                    ; n in r14

                    mov rdx, rcx
                    mov rcx, r8

                    call Metric

                    mov r15, rax
                    ; min_dist in r15
                    mov rbx, r14
                    ; cur_index in rbx
.loop_begin:
                    cmp rbx, 1
                    jle .loop_end

                    mov rdi, r12
                    mov rsi, r13

                    mov rax, rbx
                    shl rax, 1

                    lea rdx, [rsp + 24 + 8 * rax]
                    mov rcx, rdx
                    add rcx, 8

                    cmp rbx, 2
                    jne .no_fix
                    sub rdx, 8

.no_fix:
                    movsx rdx, DWORD [rdx]
                    movsx rcx, DWORD [rcx]

                    call Metric

                    cmp rax, r15
                    jge .not_update
                    mov r15, rax
                    mov QWORD [rsp], rbx
.not_update:
                    dec rbx
                    jmp .loop_begin
.loop_end:
                    mov rax, QWORD [rsp]

                    add rsp, 8

                    mov rdi, r14
                    ; n in rdi

                    pop r15
                    pop r14
                    pop r13
                    pop r12
                    pop rbx

                    cmp rdi, 1
                    je .no_r9_end
                    add rsp, 8

.no_r9_end:
                    ret

CalculateArraySum:
                    ; &array in rdi
                    ; array_size in rsi
                    xor rax, rax
                    ; ans in rax
                    xor rcx, rcx
                    ; counter in rcx
.loop_begin:
                    cmp rcx, rsi
                    jge .loop_end

                    add rax, QWORD [rdi + 8 * rcx]

                    inc rcx
                    jmp .loop_begin
.loop_end:
                    ret

AsmSummarizeRows:
                    ; &a in rdi
                    ; rows in rsi
                    ; cols in rdx
                    ; &b in rcx

                    push r12
                    push r13
                    push r14
                    push r15

                    mov r12, rdi
                    ; &a in r12

                    mov r13, rsi
                    ; rows in r13
                    dec r13

                    mov r14, rdx
                    ; cols in r14

                    mov r15, rcx
                    ; &b in r15

.loop_begin:
                    cmp r13, 0
                    jl .loop_end

                    mov rdi, QWORD [r12 + 8 * r13]
                    mov rsi, r14

                    call CalculateArraySum

                    mov QWORD [r15 + 8 * r13], rax

                    dec r13
                    jmp .loop_begin
.loop_end:

                    pop r15
                    pop r14
                    pop r13
                    pop r12

                    ret
