; fibonacci.asm
;
; $ nasm -v
; NASM version 2.11.08
; $ nasm fibonacci.asm -o fibonacci.bin
; $ hexdump -C fibonacci.bin
; 00000000  66 31 db 66 b9 09 00 00  00 66 31 d2 66 b8 01 00  |f1.f.....f1.f...|
; 00000010  00 00 eb 00 66 89 da 66  89 c3 66 01 d0 66 49 66  |....f..f..f..fIf|
; 00000020  83 f9 00 75 ef f4                                 |...u..|
; 00000026
; $

section .text
global _start

_start:
    xor ebx, ebx
    mov ecx, 0x09
    xor edx, edx
    mov eax, 0x01
    jmp loop1

loop1:
    mov edx, edx
    mov ebx, eax
    add eax, edx
    dec ecx
    cmp ecx, 0x00
    jne loop1
    hlt
