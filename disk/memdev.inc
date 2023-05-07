mmptr	equ	0

mminit
mmopen	call	dvdatiy
	ld	(iy+mmptr),0
	ld	(iy+mmptr+1),0
	ret

mmread	call	dvdatiy
	ld	l,(iy+mmptr)
	ld	h,(iy+mmptr+1)
	ldir
	ld	(iy+mmptr),l
	ld	(iy+mmptr+1),h
	ret

mmwrite	equ	ioerr

mmseek	equ	ioerr
mmgetc	equ	ioerr
mmputc	equ	ioerr

