f32blkd	equ	0	; pointer to block device
f32omsk	equ	2	; bitmask of open files
f32part	equ	3	; partition table (66 bytes)
f32size	equ	69

f32init	call	dvdatiy
	ld	(iy+f32blkd),e
	ld	(iy+f32blkd+1),d
	xor	a,a
	ld	(iy+f32omsk),0
	ret

f32open	call	dvdatiy
	push	ix	; self pointer
	push	de	; points to path (may be empty)
	ld	e,(iy+f32blkd)
	ld	d,(iy+f32blkd+1)
	db	0ddh ; ix
	ld	l,e
	db	0ddh ; ix
	ld	h,d
	; from now on ix is the block device
	ld	a,(iy+f32omsk)
	and	a,a ; don't open block device again for 2nd file
	jr	nz,f32opn1
	push	iy
	call	dvopen
	pop	iy
f32opn1	

	pop	de ; saved path parameter

	rst	error	; TODO
	pop	ix
	ret

