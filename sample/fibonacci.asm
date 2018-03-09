; fibonacci.asm
;
; $ nasm -v
; NASM version 2.11.08
; $ nasm fibonacci.asm -o fibonacci.bin
; $ hexdump -C fibonacci.bin
;
;

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
