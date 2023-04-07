; device descriptors
video	dw	ioerr	; read
	dw	vwrite	; write
	dw	vseek	; seek
	dw	ioerr	; getc
	dw	vputc	; putc
	dw	vctrl	; control
	dw	vinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 00h

keybrd	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	kgetc	; getc
	dw	ioerr	; putc
	dw	kctrl	; control
	dw	kinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 10h

serial	dw	sread	; read
	dw	swrite	; write
	dw	ioerr	; seek
	dw	sgetc	; getc
	dw	sputc	; putc
	dw	sctrl	; control
	dw	sinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 20h

console	dw	cread	; read
	dw	cwrite	; write
	dw	ioerr	; seek
	dw	cgetc	; getc
	dw	cputc	; putc
	dw	cctrl	; control
	dw	cinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 30h

multi	dw	mread	; read
	dw	mwrite	; write
	dw	ioerr	; seek
	dw	mgetc	; getc
	dw	mputc	; putc
	dw	ioerr	; control
	dw	minit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 40h