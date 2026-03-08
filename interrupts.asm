[bits 32]

global idt_load
global irq1_stub

extern keyboard_handler

idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret

irq1_stub:
    pusha
    call keyboard_handler
    popa
    iretd
