ftblkdv	equ	0

ftinit	call	dvdatiy
	ld	(iy+ftblkdv),e
	ld	(iy+ftblkdv+1),d
	ret

ftopen	rst	error	; TODO


