; data block indices
kbase	equ	0 ; base I/O address

; initialize data block
kinit	call	dvdatiy
	ld	(iy+kbase),b
	ret

kgetc	call	dvdatiy
	ld	c,(iy+kbase)
	inc	c
	inc	c
kgetc1	in	a,(c)
	and	a,00000001b
	jr	z,kgetc1
	dec	c
	dec	c
	in	a,(c)
	ret

