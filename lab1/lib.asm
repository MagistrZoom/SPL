; ќжидаемый вывод:
; +
; the length of this string is 29
; 18446744073709551615 = 18446744073709551615
; -1
; 1
; 0
; -999
; 888

section .data
integer: db "12345678hh91987654321hehehe", 0
;integer: db "0", 0
intbuf:	times 22 db 0;
section .text
;%include "lib.inc"

global _start 

print_string:
	call    string_length
	mov     rsi, rdi
        push    rdi
        mov     rdi, rsi
        mov     rdi, 1 
        mov     rdx, rax
        mov     rax, 1
        syscall
        pop     rdi
        ret


string_copy:
	xchg 	rdi, rsi
.loop:
	cmp	byte[rsi], 0
	je	.finish
	movsb	
	jmp	.loop
.finish:
	movsb
	ret


print_uint:
        push    rcx
        mov     rbx, 10
        mov     rcx, 21
.tg:    
	mov     rax, rdi
        mov     rdi, intbuf + 20
.div:
        xor     rdx, rdx
        idiv    rbx
        or      dl , 30h
        mov     [rdi], dl
        dec     rdi
        dec     rcx
        cmp     rax, 0
        je      .ll
        jmp     .div
.ll:
	inc 	rdi
	mov	rsi, intbuf
	call 	string_copy
.finish:
        mov     rdi, intbuf
        call    print_string
        pop     rcx
        ret


string_length:
        xor rax, rax
.loop:
        cmp byte[rdi + rax], 0
        je .finish
        inc rax
        jmp .loop
.finish:
        ret


parse_uint:
        push    rbx
        mov     rbx, 1
        mov     rcx, 10
        call    string_length
	mov	r10, rax
        add     rdi, rax
	dec	rdi
        xor     r9, r9
.loop:
	cmp	r10, 0
	je	.finish
	xor	rax, rax
        mov     al, [rdi]
        dec     rdi
	dec	r10
        sub     al, 30h
	cmp	al, 9
	jg	.loop
        mul     rbx
        add     r9, rax
        mov     rax, rbx
        mul     rcx
        mov     rbx, rax
        jmp     .loop
.finish:
        mov     rax, r9
        pop     rbx
        ret


_start:
    ;mov rdi, '+'
    ;call print_char
    ;call print_newline
    ;mov rdi, str
    ;call print_string
    ;mov rdi, str
    ;call string_length
    ;mov rdi, rax
    ;call print_uint
    ;call print_newline
 
    ;mov rdi, -1
    ;call print_uint
    ;mov rdi, pow
    ;call print_string
    ;call print_newline

    ;mov rdi, -1
    ;call print_int
    ;call print_newline

    ;mov rdi, str1
    ;mov rsi, str2
    ;call string_equals
    ;mov rdi, rax
    ;call print_int
    ;call print_newline

    ;mov rdi, str2
    ;mov rsi, str3
    ;call string_equals
    ;mov rdi, rax
    ;call print_int
    ;call print_newline
    ;call read_word
    mov rdi, integer
    call parse_uint
    mov rdi, rax
    call print_uint
    ;call print_newline
   
    ;call read_word
    ;mov rdi, rax
    ;call parse_uint
    ;mov rdi, rax
    ;call print_uint
    ;call print_newline
   
    ;mov rdi, str1
    ;mov rsi, copybuf 

    ;call string_copy
    ;mov rdi, str1 
    ;call print_string
    ;call print_newline
    ;mov rdi, copybuf
    ;call print_string
    ;call print_newline
    
.end:
    mov rax, 60
    xor rdi, rdi
    syscall
