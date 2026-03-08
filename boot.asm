[org 0x7c00]

LOADER_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl

mov ah,0x0e
mov al,'B'
int 0x10
mov al,'L'
int 0x10
mov al,'!'
int 0x10

mov bx, LOADER_OFFSET

mov ah,0x02
mov al,1
mov ch,0
mov cl,2
mov dh,0
mov dl,[BOOT_DRIVE]
int 0x13

jc disk_error

jmp LOADER_OFFSET

disk_error:
mov ah,0x0e
mov al,'E'
int 0x10
jmp $

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xaa55
