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
	push	ix
	ld	ix,(dvlist+rootfs)
	call	dvopen
	pop	ix
	ret

