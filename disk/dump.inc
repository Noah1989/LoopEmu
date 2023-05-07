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
dumphlp	db	27,'[93m','usage: dump filename [address [count]]'
	db	27,'[0m',13,10
dumphll	equ	$-dumphlp
dump1	inc	de
	inc	de
	ex	de,hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
; TODO parse remaining args
	ld	hl,-16 ; buffer space
	add	hl,sp
	ld	sp,hl
	push	ix
	ld	ix,(dvlist+rootfs)
	push	hl
	call	dvopen
	pop	de
	ld	bc,16
	rst	dvread
	pop	ix
	ld	hl,0
	add	hl,sp
	ld	b,16
dump2	ld	a,(hl)
	push	bc
	push	hl
	rst	dvputc
	pop	hl
	pop	bc
	inc	hl
	djnz	dump2
	ld	sp,hl
	ret
