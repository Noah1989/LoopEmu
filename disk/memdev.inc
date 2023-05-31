mmptr	equ	0 ; current read/write pointer
mmorg	equ	2 ; origin offset
mmstate	equ	4 ; state bits

mmsopen	equ	0 ; set if device is in use (open)

mminit	call	dvdatiy
	ld	(iy+mmptr),e
	ld	(iy+mmptr+1),d
	ld	(iy+mmorg),e
	ld	(iy+mmorg+1),d
	ld	(iy+mmstate),0
	ret

mmopen	call	dvdatiy
	bit	mmsopen,(iy+mmstate)
	call	nz,error ; already open
	ld	l,(iy+mmorg)
	ld	h,(iy+mmorg+1)
	ld	(iy+mmptr),l
	ld	(iy+mmptr+1),h
	set	mmsopen,(iy+mmstate)
	ret

mmclose	call	dvdatiy
	bit	mmsopen,(iy+mmstate)
	call	z,error ; already closed
	res	mmsopen,(iy+mmstate)
	ret

mmread	call	dvdatiy
	bit	mmsopen,(iy+mmstate)
	call	z,error ; not open
	ld	l,(iy+mmptr)
	ld	h,(iy+mmptr+1)
	ldir
	ld	(iy+mmptr),l
	ld	(iy+mmptr+1),h
	ret

mmwrite	rst	error ; TODO

mmseek	call	dvdatiy
	bit	mmsopen,(iy+mmstate)
	call	z,error ; not open
	ld	l,(iy+mmorg)
	ld	h,(iy+mmorg+1)
	add	hl,de
	ld	(iy+mmptr),l
	ld	(iy+mmptr+1),h
	ret

mmgetc	call	dvdatiy
	bit	mmsopen,(iy+mmstate)
	call	z,error ; not open
	ld	l,(iy+mmptr)
	ld	h,(iy+mmptr+1)
	ld	a,(hl)
	inc	hl
	ld	(iy+mmptr),l
	ld	(iy+mmptr+1),h
	ret

mmputc	rst	error ; TODO

mmctrl	rst	error