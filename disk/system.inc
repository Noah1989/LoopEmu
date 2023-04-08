	include	a:system.def

; device descriptors
vdescr	dw	ioerr	; read
	dw	vwrite	; write
	dw	vseek	; seek
	dw	ioerr	; getc
	dw	vputc	; putc
	dw	vctrl	; control
	dw	vinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 00h

kdescr	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	kgetc	; getc
	dw	ioerr	; putc
	dw	kctrl	; control
	dw	kinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 10h

sdescr	dw	sread	; read
	dw	swrite	; write
	dw	ioerr	; seek
	dw	sgetc	; getc
	dw	sputc	; putc
	dw	sctrl	; control
	dw	sinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 20h

cdescr	dw	cread	; read
	dw	cwrite	; write
	dw	ioerr	; seek
	dw	cgetc	; getc
	dw	cputc	; putc
	dw	cctrl	; control
	dw	cinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 30h

mdescr	dw	mread	; read
	dw	mwrite	; write
	dw	ioerr	; seek
	dw	mgetc	; getc
	dw	mputc	; putc
	dw	ioerr	; control
	dw	minit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 40h

; initialize device list
sysinit ld	hl,mdescr
	ld	(dvlist+term),hl
; initialize video
	ld	ix,vdescr
	ld	b,0a0h
	call	dvinit
; initialize keyboard
	ld	ix,kdescr
	ld	b,0c0h
	call	dvinit
; initialize serial
	ld	ix,sdescr
	ld	b,0c2h
	call	dvinit
; initialize console
	ld	ix,cdescr
	ld	de,kdescr
	ld	bc,vdescr
	call	dvinit
; initialize multi
	ld	ix,mdescr
	ld	de,cdescr
	ld	bc,sdescr
	call	dvinit
	ret
