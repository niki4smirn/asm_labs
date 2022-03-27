                    global AsmProduct

                    section .text

AsmProduct:         ; &array in rdi
                    movsx rsi, esi
                    ; size in rsi
                    movsx rcx, edx
                    ; module in ecx
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