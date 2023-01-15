dvread	equ	virt0
dvwrite	equ	virt1
dvseek	equ	virt2
dvgetc	equ	virt3
dvputc	equ	virt4
dvcntl	equ	virt5
dvinit	equ	virt6
dvopen	equ	virt7
dvclose	equ	virt8
dvdata	equ	2 * 9

ionull	xor	a,a
	ret
ioerr	rst	error

