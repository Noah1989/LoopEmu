dvread	equ	virt0	; DE: target, BC: length
dvwrite	equ	virt1	; DE: source, BC: length
dvseek	equ	virt2	; DE: address
dvgetc	equ	virt3	; result in A
dvputc	equ	virt4	; char from A
dvcntl	equ	virt5
dvinit	equ	virt6
dvopen	equ	virt7
dvclose	equ	virt8
dvdata	equ	2 * 9

ionull	xor	a,a
	ret
ioerr	rst	error

