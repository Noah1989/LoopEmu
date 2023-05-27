; dump command (argc b, *argv[] de)
; (ix points to standard i/o device)

; local variables (offsets)
dumbbuf	equ	0	;buffer (to read bytes from device)
dumplin	equ	16	;lines to print
dumpfil	equ	17	;pointer to filename
dumpadr	equ	19	;address (16 bit)
dumpblk	equ	21	;block	 (32 bit)
dumpvln	equ	25	;total length in bytes

dump	jr	dump0
	db	'dump',0
dump0	djnz	dump1
; no args
	ld	de,dumphlp
	ld	bc,dumphll
	rst	dvwrite
	ret
dumphlp	db	27,'[93m','usage: dump filename [address [block]]'
	db	27,'[0m',13,10
dumphll	equ	$-dumphlp
; allocate local variables on stack
dump1	ld	hl,0 ; default value
	push	hl ; block
	push	hl ; block
	push	hl ; address
; filename
	inc	de ; skip argv[0]
	inc	de
	ex	de,hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	push	de
	ld	iy,-dumpfil
	add	iy,sp ; space for remaining vars
	ld	sp,iy
; address
	dec	b
	jr	z,dump10
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	push	hl
	push	bc
	ex	de,hl
	call	fromhex
	ld	(iy+dumpadr+1),a
	call	fromhex
	ld	(iy+dumpadr),a
	pop	bc
	pop	hl
; block
	dec	b
	jr	z,dump10
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	call	fromhex
	ld	(iy+dumpblk+3),a
	call	fromhex
	ld	(iy+dumpblk+2),a
	call	fromhex
	ld	(iy+dumpblk+1),a
	call	fromhex
	ld	(iy+dumpblk),a
; open device
dump10	push	ix
	ld	ix,(dvlist+rootfs)
	ld	e,(iy+dumpfil)
	ld	d,(iy+dumpfil+1)
	push	iy
	call	dvopen
	pop	iy
	ex	(sp),ix ; back to stdio device
	ld	(iy+dumplin),16 ; number of lines
; print address
dump11	ld	a,(iy+dumpadr+1)
	push	iy
	call	dumphex
	pop	iy
	ld	a,(iy+dumpadr)
	push	iy
	call	dumphex
	ld	a,':'
	rst	dvputc
	ld	a,' '
	rst	dvputc
	ld	a,' '
	rst	dvputc
	pop	iy
; seek to address and read 16 bytes
	ex	(sp),ix ; device to dump from
	exx
	ld	b,(iy+dumpblk+3)
	ld	c,(iy+dumpblk+2)
	exx
	ld	b,(iy+dumpblk+1)
	ld	c,(iy+dumpblk)
	ld	d,(iy+dumpadr+1)
	ld	e,(iy+dumpadr)
	push	de
	push	iy
	rst	dvseek
	pop	iy
	pop	hl
; move address pointer forward
	ld	bc,16
	add	hl,bc
	ld	(iy+dumpadr+1),h
	ld	(iy+dumpadr),l
	db	0fdh	;use iyh
	ld	d,h
	db	0fdh	;use iyl
	ld	e,l
	push	de
	push	iy
	rst	dvread
	pop	iy
	pop	hl ; buffer
	ex	(sp),ix ; stdio
; dump 4x4 bytes
	push	iy
	ld	c,4
dump2	ld	b,4
dump3	ld	a,(hl)
	push	bc
	push	hl
	call	dumphex
	ld	a,' '
	rst	dvputc
	pop	hl
	pop	bc
	inc	hl
	djnz	dump3
	ld	a,' '
	push	bc
	push	hl
	rst	dvputc
	pop	hl
	pop	bc
	dec	c
	jr	nz,dump2
	ld	bc,-16
	add	hl,bc
	push	hl
	ld	a,'|'
	rst	dvputc
	pop	hl
	ld	b,16
dump4	ld	a,(hl)
	cp	a,20h
	jr	c,dump41
	cp	a,7fh
	jr	c,dump42
dump41	ld	a,'.'
dump42	push	bc
	push	hl
	rst	dvputc
	pop	hl
	pop	bc
	inc	hl
	djnz	dump4
; ascii dump
	ld	a,'|'
	rst	dvputc
	ld	a,13
	rst	dvputc
	ld	a,10
	rst	dvputc
	pop	iy
	dec	(iy+dumplin)
	jp	nz, dump11
; close device
	ex	(sp),ix
	call	dvclose
	pop	ix
; deallocate local vars
	ld	hl,dumpvln
	add	hl,sp
	ld	sp,hl
	ret

dumphex	push	af
	rrca
	rrca
	rrca
	rrca
	call	dumpnib
	pop	af
dumpnib	and	a,0fh
	add	a,90h
	daa
	adc	a,40h
	daa
	rst	dvputc
	ret
