f32blkd	equ	0

f32init	call	dvdatiy
	ld	(iy+f32blkd),e
	ld	(iy+f32blkd+1),d
	ret

f32open	call	dvdatiy
	
	rst	error	; TODO


