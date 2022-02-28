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
                    cmp rdx, 0x40000000
                    jge .inside

                    imul rdi, rdi
                    imul rsi, rsi
                    add rdi, rsi

                    imul rdx, rdx

                    cmp rdi, rdx
                    jle .inside

                    jmp .return
.inside:
                    mov rax, 1
.return:
                    ret

TwoIntegers:        ; YOUR_CODE_HERE
                    ret

CountNumbers:       ; YOUR_CODE_HERE
                    ret

Switch:             ; YOUR_CODE_HERE
                    ret

MagicMetric:        ; YOUR_CODE_HERE
                    ret