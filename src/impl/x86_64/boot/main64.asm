global long_mode_start
extern kernel_main
global getch
extern keeb_inter
	
section .text
bits 64
long_mode_start:
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call kernel_main
	
	hlt
	
