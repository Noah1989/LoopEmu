; device descriptors
video	dw	ioerr	; read
	dw	vwrite	; write
	dw	vseek	; seek
	dw	ioerr	; getc
	dw	vputc	; putc
	dw	ioerr	; control
	dw	vinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 00h

keybrd	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	kgetc	; getc
	dw	ioerr	; putc
	dw	ioerr	; control
	dw	kinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 10h

console	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	cgetc	; getc
	dw	cputc	; putc
	dw	ioerr	; control
	dw	cinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 20h

