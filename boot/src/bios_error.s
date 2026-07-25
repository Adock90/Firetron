;This is used to tell BIOS users that BIOS is incompatible
[bits 16]
[org 0x7c00]

jmp boot_msg_main

boot_msg_main:
	mov bx, bios_error_msg
	call fireboot_print_str
	jmp $	

fireboot_print_str:
	pusha
	
	mov ah, 0x0e
	mov si, bx
	

.fireboot_print_char:
	lodsb
	
	cmp AL, 0x00
	je .fireboot_print_end_string
	
	int 0x10
	jmp .fireboot_print_char

.fireboot_print_end_string:
	popa
	ret

bios_error_msg db "[FATAL ERR] Your Device has booted Firetron in BIOS mode which is unsupported."
times 510-($-$$) db 0x00
dw 0xaa55
