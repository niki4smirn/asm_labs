                    global Positivity
                    global CheckOverflow
                    global Function
                    global IsInCircle
                    global TwoIntegers
                    global CountNumbers
                    global Switch
                    global MagicMetric

                    section .data

positive_number_response:
                    db 'Yeah) It', 39, 's positive :D', 0
negative_number_response:
                    db 'Ohh.. so much negative :(', 0
switch_first_response:
                    db 'That', 39, 's bad', 0
switch_second_response:
                    db 'No comments.', 0
switch_third_response:
                    db 'Not bad...', 0
switch_fourth_response:
                    db 'Nice :)', 0
switch_fifth_response:
                    db 'Perfecto!!!', 0
switch_sixth_response:
                    db 'Ooops', 0

                    section .text

Positivity:         ; x in rdi
                    xor rax, rax
                    cmp rdi, 0
                    je .negative
                    shr rdi, 63
                    cmp rdi, 1
                    je .negative
                    jmp .positive
.positive:
                    mov rax, positive_number_response
                    jmp .return
.negative:
                    mov rax, negative_number_response
.return:
                    ret

CheckOverflow:      ; x in rdi, y in rsi
                    mov r8, 0
                    mov rcx, 0
                    mov rax, rdi

                    ; x^2
                    imul rdi
                    seto cl
                    cmp cl, 1
                    je .overflow
                    mov r9, rax
                    ; x^2 in r9

                    mov rax, rsi
                    imul rdi
                    add rax, rax
                    ; 2xy in rax

                    mov r10, rax
                    add r10, r9
                    ; x^2 + 2xy in r10

                    mov rax, rsi
                    ; y^2
                    imul rsi
                    seto cl
                    cmp cl, 1
                    je .overflow

                    add rax, r10
                    seto cl
                    cmp cl, 1
                    je .overflow

                    cmp rax, 0
                    je .overflow

                    mov rax, r9
                    ; x^3
                    imul rdi
                    seto cl
                    cmp cl, 1
                    je .overflow

                    ; x^4
                    imul rdi
                    seto cl
                    cmp cl, 1
                    je .overflow
                    xor rax, rax
                    jmp .return
.overflow:
                    mov rax, 1
.return:
                    ret

Function:           ; x in rdi
                    cmp rdi, -1
                    jl .first_branch

                    cmp rdi, 17
                    jle .second_branch

                    jmp .third_branch
.first_branch:
                    mov rax, rdi
                    imul rax
                    add rax, rax
                    sub rax, 3
                    jmp .return
.second_branch:
                    mov r8, 10
                    sub r8, rdi
                    sub r8, rdi
                    sub r8, rdi
                    ; 10 - 3x in r8

                    mov r10, r8
                    ; in r10 stores value to make ceil
                    cmp r10, 0
                    jge .skip_abs
                    neg r10
.skip_abs:
                    dec r10

                    mov rax, rdi
                    imul rax
                    mov r9, 36
                    sub r9, rax
                    ; 36 - x^2 in r9

                    cmp r9, 0
                    jl .sub
                    jmp .add
.sub:
                    sub r9, r10
                    jmp .end_if
.add:
                    add r9, r10
.end_if:
                    mov rax, r9
                    cqo
                    idiv r8

                    jmp .return
.third_branch:
                    mov rax, rdi
                    imul rdi
                    imul rdi

                    add rax, 2
                    neg rax

                    jmp .return
.return:
                    ret

IsInCircle:         ; x in rdi, y in rsi, d in rdx
                    xor rax, rax
                    mov r8, 0x80000000
                    cmp rdx, r8
                    jge .inside

                    imul rdi, rdi
                    imul rsi, rsi
                    add rdi, rsi

                    mov r10, 4
                    imul rdi, r10

                    imul rdx, rdx

                    cmp rdi, rdx
                    jg .return
.inside:
                    mov rax, 1
.return:
                    ret

TwoIntegers:        ; x in rdi, y in rsi
                    xor rdx, rdx
                    mov rax, rdi
                    div rsi

                    cmp rdx, 0
                    je .success

                    xor rdx, rdx
                    mov rax, rsi
                    div rdi

                    cmp rdx, 0
                    je .success

                    xor rax, rax
                    jmp .return
.success:
                    mov rax, 1
.return:
                    ret

CountNumbers:       ; a in rdi, b in rsi, c in rdx
                    mov r8, rdx
                    xor rdx, rdx
                    ; c in r8
                    mov r9, rdi
                    cmp rsi, r9
                    jl .mov_from_rsi_to_r9
                    jmp .after_rsi_mov
.mov_from_rsi_to_r9:
                    mov r9, rsi
.after_rsi_mov:
                    cmp r8, r9
                    jl .mov_from_r8_to_r9
                    jmp .after_r8_mov
.mov_from_r8_to_r9:
                    mov r9, r8
.after_r8_mov:
                    ; min in r9
                    xor r10, r10
                    ; ans in r10

                    mov rax, rdi
                    idiv r9

                    mov r11, 1
                    and r11, rdx
                    add r10, r11
                    xor rdx, rdx

                    mov rax, rsi
                    idiv r9

                    mov r11, 1
                    and r11, rdx
                    add r10, r11
                    xor rdx, rdx

                    mov rax, r8
                    idiv r9

                    mov r11, 1
                    and r11, rdx
                    add r10, r11
                    xor rdx, rdx

                    mov rax, r10
                    ret

Switch:             ; k in rdi
                    cmp rdi, 0
                    je .first
                    cmp rdi, 1
                    je .first
                    cmp rdi, 2
                    je .first
                    cmp rdi, 5
                    je .first

                    cmp rdi, 4
                    je .second
                    cmp rdi, 42
                    je .second
                    cmp rdi, 43
                    je .second

                    cmp rdi, 13
                    je .third

                    cmp rdi, -99
                    je .fourth
                    cmp rdi, -100
                    je .fourth

                    cmp rdi, 10
                    je .fifth
                    cmp rdi, 100
                    je .fifth
                    cmp rdi, 1000
                    je .fifth

                    jmp .sixth

.first:
                    mov rax, switch_first_response
                    jmp .return
.second:
                    mov rax, switch_second_response
                    jmp .return
.third:
                    mov rax, switch_third_response
                    jmp .return
.fourth:
                    mov rax, switch_fourth_response
                    jmp .return
.fifth:
                    mov rax, switch_fifth_response
                    jmp .return
.sixth:
                    mov rax, switch_sixth_response
                    jmp .return
.return:
                    ret

MagicMetric:        ; x in rdi
                    xor rax, rax
                    cmp rdi, 10000000
                    jl .return
                    cmp rdi, 99999999
                    jg .return

                    xor r8, r8
                    ; ans in r8

                    mov rax, rdi
                    mov r10, 10

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, 2
                    je .first_ok

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, 2
                    je .first_ok

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, 2
                    je .first_ok

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, 2
                    je .first_ok

                    jmp .second
.first_ok:
                    inc r8
.second:
                    mov rax, rdi
                    mov r10, 100000
                    xor rdx, rdx
                    idiv r10

                    xor r9, r9
                    ; sum in r9

                    mov r10, 10

                    xor rdx, rdx
                    idiv r10
                    add r9, rdx

                    xor rdx, rdx
                    idiv r10

                    xor rdx, rdx
                    idiv r10
                    add r9, rdx

                    cmp r9, 5
                    jle .third

                    inc r8
.third:
                    mov rax, rdi
                    mov r10, 10
                    xor rdx, rdx
                    idiv r10
                    xor rdx, rdx
                    idiv r10
                    xor rdx, rdx
                    idiv r10

                    mov r9, rdx
                    ; second in r9

                    xor rdx, rdx

                    mov r10, 1000
                    idiv r10
                    xor rdx, rdx
                    mov r10, 10
                    idiv r10

                    ; sixth in rdx

                    cmp rdx, r9
                    jne .fourth

                    inc r8
.fourth:
                    mov rax, rdi
                    mov r10, 10

                    xor rdx, rdx
                    idiv r10
                    mov rcx, rdx

                    xor rdx, rdx
                    idiv r10
                    mov r9, rdx

                    xor rdx, rdx
                    idiv r10
                    mov r11, rdx

                    xor rdx, rdx
                    idiv r10
                    mov r12, rdx

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, r12
                    jne .after_fourth

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, r11
                    jne .after_fourth

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, r9
                    jne .after_fourth

                    xor rdx, rdx
                    idiv r10
                    cmp rdx, rcx
                    jne .after_fourth

                    inc r8
.after_fourth:
                    mov rax, r8
.return:
                    ret