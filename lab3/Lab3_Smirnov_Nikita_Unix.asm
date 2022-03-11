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

AsmIsSquare:
                    ; YOUR_CODE_HERE
                    ret

AsmRemoveDigits:
                    ; YOUR_CODE_HERE
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