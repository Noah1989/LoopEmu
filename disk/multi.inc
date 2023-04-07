; data block indices
mconsle	equ	0;..1	console device of the multiterminal
mserial	equ	2;..3	serial device of the multiterminal


minit	call	dvdatiy
	ld	(iy+mconsle),e
	ld	(iy+mconsle+1),d
	ld	(iy+mserial),c
	ld	(iy+mserial+1),b
	ret

mread	equ	dvgetcr
mwrite	equ	dvputcr

mgetc	call	dvdatiy
	push	ix
mgetc0	ld	a,(iy+mserial)
	db	0ddh ; use ixl
	ld	l,a
	ld	a,(iy+mserial+1)
	db	0ddh ; use ixh
	ld	h,a
	xor	a,a
	push	iy
	rst	dvctrl
	pop	iy
	and	a,1
	jr	nz,mgetc1
	ld	a,(iy+mconsle)
	db	0ddh ; use ixl
	ld	l,a
	ld	a,(iy+mconsle+1)
	db	0ddh ; use ixh
	ld	h,a
	xor	a,a
	push	iy
	rst	dvctrl
	pop	iy
	and	a,1
	jr	z,mgetc0
mgetc1	rst	dvgetc
	pop	ix
	ret

mputc	push	ix
	push	af
	ld	b,a
	call	dvdatiy
	push	iy
	push	bc
	ld	a,(iy+mconsle)
	db	0ddh ; use ixl
	ld	l,a
	ld	a,(iy+mconsle+1)
	db	0ddh ; use ixh
	ld	h,a
	pop	af ; previously bc
	rst	dvputc
	pop	iy
	ld	a,(iy+mserial)
	db	0ddh ; use ixl
	ld	l,a
	ld	a,(iy+mserial+1)
	db	0ddh ; use ixh
	ld	h,a
	pop	af
	rst	dvputc
	pop	ix
	ret

