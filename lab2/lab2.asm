%include "../lab1/lib.inc"

%define link 0
; amount of bytes to link code
%define word_size 8

%define pc r15
%define w  r14
%define rstack r13
%define here rbx

; macro for native command building
%macro native 3
section .data
w_ %+ %2:
    %%link dq link
%define link %%link
    db %1, 0
    db %3
xt_ %+ %2:
    dq %2 %+ _impl
section .text
%2 %+ _impl:
%endmacro
%macro native 2
    native %1, %2, 0
%endmacro
; macro for colon- words
%macro colon 3
section .data
w_ %+ %2:
    %%link dq link
%define link %%link
    db %1, 0
    db %3
    dq xt_docol
%endmacro
%macro colon 2
    colon %1, %2, 0
%endmacro

section .text
; arythm block
native '+', plus
    pop     rax
    add     [rsp], rax
    jmp     next

native '-', minus
    pop     rax
    pop     rcx
    sub     rcx, rax
    push    rcx
    jmp     next

native '*', multiply
    pop     rax
    pop     rcx
    imul    rcx
    push    rax
    jmp     next

native '/', division
    pop     rcx ; !!! DONE
    pop     rax
    cqo
    idiv    rcx
    push    rax
    jmp     next


native '=', equals
    pop     rax  ; sete + movzx
    pop     rcx  ; DONE
    cmp     rax, rcx
    sete    al
    movzx   rax, al
    push    rax
    jmp     next

native  '<', less
    pop     rcx
    pop     rax
    cmp     rax, rcx
    setl    al
    movzx   rax, al
    push    rax 
    jmp     next
     
; END arythm block
   


; logic block
native 'and', conjunction
    pop     rax
    pop     rcx
    and     rcx, rax
    cmp     rcx, 0
    je      .null
    mov     rcx, 1
.null:
    push    rcx
    jmp     next

native 'not', negation
    pop     rax
    cmp     rax, 0
    setne    al
    movzx   rax, al
    push    rax
    jmp     next
; END logic block



; print all stack
native '.S', dotS
    mov     r9, rsp
.loop:
    mov     rdi, [r9] ;!!! done
    push    r9
    call    print_int
    pop     r9
    call    print_newline
    add     r9, word_size
    mov     r10, [stackHead]
    cmp     r9, r10
    jge      .f
    jmp     .loop
.f:
    jmp     next

; print head of stack
native '.', dot
    cmp     rsp, [stackHead]
    je      .error
    pop     rdi
    call    print_int
    call    print_newline
    jmp     .f
.error:
    mov     rdi, stackUnderflow
    call    print_string
    call    print_newline
.f:
    jmp     next

; rotate three top elements
native 'rot', rot
    pop     rax
    pop     rcx
    pop     rdx
    push    rcx
    push    rax
    push    rdx
    jmp     next 

native 'swap', swap
    pop     rax
    pop     rcx
    push    rax
    push    rcx
    jmp     next

native 'dup', dup
    pop     rax
    push    rax
    push    rax
    jmp     next

native 'drop', drop
    pop     rax
    jmp     next

; IO
native 'key', key
    ;!!!
    call    read_char
    push    qword[INPUT]
    jmp     next

native 'emit', emit
    pop     rdi
    call    print_char
    jmp     next

native 'number', number
    call    read_word
    mov     rdi, rax
    call    parse_int
    push    rax
    jmp     next
; user memory
native 'mem', mem
    push    ustackPtr
    jmp     next

native '!', mmrwrite
    pop     rdx
    pop     rax
    mov     [rax], rdx
    jmp     next

native '@', mmrread
    pop     rax
    mov     rdx, [rax]
    push    rdx
    jmp     next

colon '>', greatest
    dq      xt_swap
    dq      xt_less
    dq      xt_exit

colon 'or', disjunction
    dq      xt_negation
    dq      xt_swap
    dq      xt_negation
    dq      xt_conjunction
    dq      xt_negation
    dq      xt_exit
; compiler
native ':', colon
    call    read_word
    mov     rdi, rax
    mov     rdx, [last_word]
    mov     [here], rdx
    mov     qword[last_word], here 
    add     here, word_size
    mov     rsi, here  
    push    rsi
    call    string_copy
    pop     rsi
    mov     rdi, rax
    call    string_length
    add     here, rax
    mov     word[here], 0 ;!!! flag? :( DONE
    add     here, 2
    mov     qword[here], xt_docol
    add     here, word_size
    mov     byte[state], 1
    jmp     next

native ';', semicolon, 1
    mov     qword[here], xt_exit
    add     here, word_size
    mov     byte[state], 0
    jmp     next

native 'lit', lit
    push    qword[pc]
    add     pc, word_size
    jmp     next

native 'branch', branch
    mov     rax, qword[pc]
    inc     rax
    mov     rdx, word_size
    mul     rdx
    add     pc, rax
    jmp     next

native 'branch0', branch0
    pop     rax
    cmp     rax, 0
    jne     .f
    mov     rax, qword[pc]
    inc     rax
    mov     rdx, word_size
    mul     rdx
 
    add     pc, rax
.f:
    jmp     next

section .text
global _start
_start:
    mov     [stackHead], rsp
    mov     pc, xt_interpreter
    mov     here, dictionary
    mov     rstack, retstack + 65536*word_size
    mov     qword[ustackPtr], userstack + 65536*word_size
    jmp     next

section .data    
    program_stub:       dq 0
    xt_interpreter:     dq .interpreter
    .interpreter:       dq interpreter_loop
section .text
interpreter_loop:
    call    read_word
    cmp     rdx, 0
    je      .stop
    mov     rdi, rax
    push    rdi
    call    find_word
    pop     rdi
    cmp     rax, 0
    je      .look_for_number
    mov     rdi, rax
    push    rdi
    call    cfa
    pop     rdi
    cmp     rax, xt_branch
    sete    [last_branch]
    cmp     rax, xt_branch0
    sete    [last_branch]
    cmp     byte[state], 0
    je      .interpretate
    mov     rdx, rax
    dec     rdx
    cmp     byte[rdx], 0
    jne      .interpretate
    mov     [here], rax
    add     here, word_size
    jmp     interpreter_loop
.interpretate:
    mov     w, rax
    mov     [program_stub], rax
    mov     pc, program_stub
    jmp     next
.look_for_number:
    push    rdi
    call    look_for_integer
    pop     rdi
    cmp     rax, 0
    je      .undefined
    call    parse_int 
    cmp     byte[state], 1
    je      .clook
    push    rax
    jmp     interpreter_loop
.clook:
    cmp     byte[last_branch], 1
    je      .branch
    mov     qword[here], xt_lit
    add     here, word_size
    mov     qword[here], rax
    add     here, word_size
    jmp     .jump
.branch:
    mov     qword[here], rax
    add     here, word_size
    mov     byte[last_branch], 0
.jump:
    jmp     interpreter_loop
.undefined:
    mov     rdi, undefined
    call    print_string
    call    print_newline
    jmp     interpreter_loop
.stop:
    mov     rax, 60
    mov     rdi, 0
    syscall

next:
    mov     w, pc
    add     pc, word_size 
    mov     w, [w]
    jmp     [w]
    
; EXEC TOKENS ;   

; It helps to run extended words
xt_docol:
    sub     rstack, word_size
    mov     [rstack], pc
    add     w, word_size 
    mov     pc, w
    jmp     next

xt_exit: dq exit ; second indirection step
exit:
    mov     pc, [rstack]
    add     rstack, word_size
    jmp     next


; @return exec token by word name
cfa:
    add     rdi, word_size
.loop:
    mov     al, [rdi]
    test    al, 0xFF
    jz      .bt
    inc     rdi
    jmp     .loop
.bt:
    add     rdi, 2
    mov     rax, rdi
ret
; @return address of word which name has pointed by rdi
find_word:
    ; rdi - addr of word
    mov     rax, 0
    mov     rsi, [last_word]
.cycle:
    add     rsi, word_size
    push    rdi
    push    rsi
    call    string_equals
    pop     rsi
    pop     rdi
    cmp     rax, 1
    je      .f
    sub     rsi, word_size
    mov     rsi, [rsi]
    cmp     rsi, 0
    je      .ff
    jmp     .cycle
.f:
    sub     rsi, word_size
.ff:
    mov     rax, rsi
ret

section .data
    undefined:          db 'Word is undefined', 0
    stackUnderflow:     db 'Stack underflow exception', 0
    last_word:          dq link
section .bss
    retstack: resq 65536 
    userstack: resq 65536
    dictionary: resq 65536
    stackPtr:   resq 1
    stackHead:  resq 1
    ustackPtr:  resq 1
    dictionaryPtr: resq 1
    state:      resq 1
    last_branch:resq 1
