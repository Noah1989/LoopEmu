; dump command (argc b, *argv[] de)
; (ix points to standard i/o device)

dump	jr	dump0
	db	'dump',0
dump0	djnz	dump1
; no args
	ld	de,dumphlp
	ld	bc,dumphll
	rst	dvwrite
	ret
dumphlp	db	27,'[93m','usage: dump filename [address]'
	db	27,'[0m',13,10
dumphll	equ	$-dumphlp
dump1	inc	de
	inc	de
	ex	de,hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	dec	b
	ld	bc,0
	jr	z,dump10
; address given
	push	de ; filename
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	call	fromhex
	ld	b,a
	call	fromhex
	ld	c,a
	pop	de ; filename
; reserve buffer space on stack
dump10	ld	hl,-16
	add	hl,sp
	ld	sp,hl
; open device
	push	ix
	ld	ix,(dvlist+rootfs)
	push	bc ; address
	call	dvopen
	pop	de ; address (filename no longer needed)
	ex	(sp),ix
	ld	b,16
; print address
dump11	push	bc ; counter in c
	push	de
	ld	a,d
	call	dumphex
	pop	de
	push	de
	ld	a,e
	call	dumphex
	ld	a,':'
	rst	dvputc
	ld	a,' '
	rst	dvputc
	ld	a,' '
	rst	dvputc
	pop	de
	pop	bc
; seek to address and read 16 bytes
	ex	(sp),ix
	push	bc
	push	de
	rst	dvseek
	ld	bc,16
	ld	hl,6
	add	hl,sp
	push	hl
	ex	de,hl
	rst	dvread
	pop	hl
	pop	de
	pop	bc
	ex	(sp),ix
; dump 4x4 bytes
	push	bc
	push	de
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
	
	ld	a,'|'
	rst	dvputc
	ld	a,13
	rst	dvputc
	ld	a,10
	rst	dvputc
	pop	hl ; was de
	ld	bc,16
	add	hl,bc
	ex	de,hl
	pop	bc
	djnz	dump11

	ex	(sp),ix
	call	dvclose
	pop	ix
	
	ld	hl,16
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
