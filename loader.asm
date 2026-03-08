[org 0x1000]

KERNEL_OFFSET equ 0x2000

mov ah,0x0e
mov al,'L'
int 0x10
mov al,'D'
int 0x10

xor ax, ax
mov es, ax
mov bx, KERNEL_OFFSET

mov ah,0x02
mov al,12
mov ch,0
mov cl,3
mov dh,0
mov dl,0x80
int 0x13

cli
lgdt [gdt_descriptor]

mov eax, cr0
or eax,1
mov cr0,eax

jmp CODE_SEG:protected_mode

[bits 32]

protected_mode:
mov ax, DATA_SEG
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

mov esp, 0x90000

mov eax, KERNEL_OFFSET
call eax

jmp $

gdt_start:
gdt_null:
dq 0

gdt_code:
dq 0x00cf9a000000ffff

gdt_data:
dq 0x00cf92000000ffff

gdt_end:

gdt_descriptor:
dw gdt_end - gdt_start - 1
dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
