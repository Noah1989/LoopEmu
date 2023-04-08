; data block indices
sbase	equ	0 ; base I/O address

; initialize data block
sinit	call	dvdatiy
	ld	(iy+sbase),b
	ld	c,b
	inc	c
	ld	hl,sinit1
	ld	b,6
	otir
	ret
sinit1	db	4,00ch
	db	3,0c1h
	db	5,0e8h

sread	equ	dvgetcr
swrite	equ	dvputcr

sgetc	call	dvdatiy
	ld	c,(iy+sbase)
	inc	c
sgetc0	in	a,(c)
	and	a,1
	jr	z,sgetc0
	dec	c
	in	a,(c)
	ret

sputc	ld	b,a
	call	dvdatiy
	ld	c,(iy+sbase)
	inc	c
sputc1	in	a,(c)
	and	a,04h
	jr	z,sputc1
	dec	c
	out	(c),b
	ret

sctrl	and	a,a
	call	nz,ioerr
	call	dvdatiy
	ld	c,(iy+sbase)
	inc	c
	in	a,(c)
	ret
