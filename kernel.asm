[bits 32]

global _start
extern kernel_main

_start:
    mov eax, KERNEL_OFFSET
    call eax

hang:
    jmp hang
