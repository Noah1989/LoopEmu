; data block indices
kbase	equ	0 ; base I/O address

; initialize data block
kinit	call	dvdatiy
	ld	(iy+kbase),b
	ld	c,b
	inc	c
	ld	hl,kinit1
	ld	b,6
	otir
	ret
kinit1	db	4,007h
	db	3,0c1h
	db	5,0e0h

kgetc	call	dvdatiy
	ld	c,(iy+kbase)
	inc	c
kgetc0	in	a,(c)
	and	a,1
	jr	z,kgetc0
	dec	c
	in	a,(c)
	ret

kctrl	and	a,a
	call	nz,ioerr
	call	dvdatiy
	ld	c,(iy+kbase)
	inc	c
	in	a,(c)
	ret
