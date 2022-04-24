                    global AsmFindNearest
                    global Metric
                    global AsmSummarizeRows
                    global AsmCountIfNot
                    global AsmGetMoreMagic
                    global AsmCopy
                    global AsmSequencesCount
                    global AsmSequencesCountRecursion

                    extern GetMagic
                    extern malloc
                    extern free

                    section .text

Metric:
                    cmp rdi, rdx
                    jae .no_swap1
                    xchg rdi, rdx
.no_swap1:
                    cmp rsi, rcx
                    jae .no_swap2
                    xchg rsi, rcx
.no_swap2:
                    sub rdi, rdx
                    mov rax, rdi
                    mul rax
                    mov r8, rax
                    sub rsi, rcx
                    mov rax, rsi
                    mul rax
                    add rax, r8
                    ret


AsmFindNearest:
                    cmp edx, 1
                    je .no_r9
                    mov r10d, r9d
                    push r10
.no_r9:
                    push rbx
                    push r12
                    push r13
                    push r14
                    push r15

                    sub rsp, 8

                    mov QWORD [rsp], 1

                    shl rdi, 32
                    shr rdi, 32

                    shl rsi, 32
                    shr rsi, 32

                    mov r12, rdi
                    ; x in r12
                    mov r13, rsi
                    ; y in r13
                    mov r14, rdx
                    ; n in r14

                    mov edx, ecx
                    mov ecx, r8d

                    call Metric

                    mov r15, rax
                    ; min_dist in r15

                    mov rbx, 2
                    ; cur_index in rbx
.loop_begin:
                    cmp rbx, r14
                    jg .loop_end

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
                    mov edx, DWORD [rdx]
                    mov ecx, DWORD [rcx]

                    call Metric

                    cmp rax, r15
                    jae .not_update
                    mov r15, rax
                    mov QWORD [rsp], rbx
.not_update:
                    inc rbx
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
                    ; rows in esi
                    ; cols in edx
                    ; &b in rcx

                    push r12
                    push r13
                    push r14
                    push r15

                    mov r12, rdi
                    ; &a in r12

                    movsx r13, esi
                    ; rows in r13
                    dec r13

                    movsx r14, edx
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

AsmCountIfNot:
                    ; &array in rdi
                    ; size in esi
                    ; &predicate in rdx

                    push rbx
                    push r12
                    push r13
                    push r15

                    mov r12, rdi
                    ; &array in r12

                    mov r13d, esi
                    ; size in r13
                    dec r13

                    mov r15, rdx
                    ; &predicate in r15

                    xor rbx, rbx
                    ; ans in rbx

.loop_begin:
                    cmp r13, 0
                    jl .loop_end

                    movzx rdi, WORD [r12 + 2 * r13]

                    call r15

                    cmp al, 0
                    jne .not_update
                    inc rbx

.not_update:
                    dec r13
                    jmp .loop_begin
.loop_end:

                    mov rax, rbx
                    pop r15
                    pop r13
                    pop r12
                    pop rbx

                    ret

AsmGetMoreMagic:
                    mov rdi, 1
                    call GetMagic

                    sub rsp, 8
                    movzx rax, al
                    mov QWORD [rsp], rax

                    mov rdi, 2
                    call GetMagic
                    movzx rdi, al
                    call GetMagic

                    movzx rax, al

                    mul QWORD [rsp]
                    mov QWORD [rsp], rax

                    mov rdi, 3
                    call GetMagic
                    movzx rdi, al
                    call GetMagic
                    movzx rdi, al
                    call GetMagic

                    movzx rax, al

                    mul QWORD [rsp]

                    add rsp, 8

                    mul rax
                    ret

AsmCopy:
                    ; &array in rdi
                    ; size in esi
                    cmp esi, 0
                    jne .not_nullptr
                    xor rax, rax
                    ret
.not_nullptr:
                    push r12
                    push r13

                    mov r12, rdi
                    movsx r13, esi

                    mov rdi, r13
                    call malloc
                    dec r13
.loop_begin:
                    cmp r13, 0
                    jl .loop_end

                    mov r8b, BYTE [r12 + r13]
                    mov BYTE [rax + r13], r8b

                    dec r13
                    jmp .loop_begin
.loop_end:

                    pop r13
                    pop r12
                    ret

AsmSequencesCount:
                    push r12
                    push r13
                    push r14

                    mov r12, rdi
                    mov r13, rsi

                    inc rdi
                    call malloc
                    mov r14, rax

                    mov rdi, r12
                    inc rdi
                    shl rdi, 3

                    call malloc

                    xor rcx, rcx

.fill_loop_begin:
                    cmp rcx, r12
                    jg .fill_loop_end

                    mov BYTE [r14 + rcx], 0

                    inc rcx
                    jmp .fill_loop_begin
.fill_loop_end:

                    sub rsp, 8
                    mov QWORD [rsp], rax
                    mov rdi, r12
                    mov rsi, r13
                    mov rdx, rax
                    mov rcx, r14

                    call AsmSequencesCountRecursion

                    push rbx
                    mov rbx, rax

                    mov rdi, QWORD [rsp + 8]
                    call free

                    mov rdi, r14
                    call free

                    mov rax, rbx

                    pop rbx
                    add rsp, 8

                    pop r14
                    pop r13
                    pop r12
                    ret

AsmSequencesCountRecursion:
                    ; n in rdi
                    ; k in rsi
                    ; buffer in rdx
                    ; ready in rcx

                    cmp rdi, 0
                    jge .non_negative_n
                    xor rax, rax
                    ret

.non_negative_n:
                    cmp rdi, rsi
                    jge .not_easy
                    cmp rdi, 64
                    jge .zero
                    mov rax, 1
                    mov rcx, rdi
                    shl rax, cl
                    jmp .after_zero
.zero:
                    xor rax, rax
.after_zero:
                    ret

.not_easy:
                    push r12
                    push r13
                    push r14
                    push r15
                    push rbx

                    mov r12, rdi
                    mov r13, rsi
                    mov r14, rcx
                    mov r15, rdx

                    sub rsp, 8

                    mov QWORD [rsp], 0

                    mov rbx, r13

.loop_begin:
                    cmp rbx, 1
                    jl .loop_end

                    mov rdi, r12
                    sub rdi, rbx
                    mov rsi, r13
                    mov rdx, r15
                    mov rcx, r14

                    cmp rdi, 0
                    jl .call

                    cmp BYTE [r14 + rdi], 0
                    je .call
                    mov rax, QWORD [r15 + 8 * rdi]
                    jmp .after_call
.call:
                    call AsmSequencesCountRecursion

.after_call:
                    add QWORD [rsp], rax

                    dec rbx
                    jmp .loop_begin
.loop_end:

                    mov rax, QWORD [rsp]

                    add rsp, 8

                    mov QWORD [r15 + 8 * r12], rax
                    mov BYTE [r14 + r12], 1

                    pop rbx
                    pop r15
                    pop r14
                    pop r13
                    pop r12

                    ret
