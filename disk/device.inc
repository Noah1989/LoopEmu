	include	a:device.def

dvdatiy	ld	a,(ix+dvdata)
	db	0fdh ; use iyl instead of l
	ld	l,a
	ld	a,(ix+dvdata+1)
	db	0fdh ; use iyh instead of h
	ld	h,a
	ret

ionull	xor	a,a
	ret
ioerr	rst	error

dvputcr	ld	a,(de)
	inc	de
	push	bc
	push	de
	rst	dvputc
	pop	de
	pop	bc
	dec	bc
	ld	a,b
	or	a,c
	jr	nz,dvputcr
	ret

dvgetcr	push	bc
	push	de
	rst	dvgetc
	pop	de
	pop	bc
	ld	(de),a
	inc	de
	dec	bc
	ld	a,b
	or	a,c
	jr	nz,dvgetcr
	ret

; dummy device descriptors
dverror	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	ioerr	; getc
	dw	ioerr	; putc
	dw	ioerr	; control
	dw	ioerr	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	0

dvnull	dw	dvgetcr	; read
	dw	ionull	; write
	dw	ionull	; seek
	dw	ionull	; getc
	dw	ionull	; putc
	dw	ionull	; control
	dw	ionull	; init
	dw	ionull	; open
	dw	ionull	; close
	dw	0
