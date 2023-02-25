; device descriptor
kbd	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	kgetc	; getc
	dw	ioerr	; putc
	dw	ioerr	; control
	dw	kinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	kdata

kinit	ret

kgetc	in	a,(0c2h)
	and	a,00000001b
	jr	z,kgetc
	in	a,(0c0h)
	ret

