	include	a:system.def

; DEVICE DESCRIPTORS

; video
vdescr	dw	vread	; read
	dw	vwrite	; write
	dw	vseek	; seek
	dw	vgetc	; getc
	dw	vputc	; putc
	dw	vctrl	; control
	dw	vinit	; init
	dw	vopen	; open
	dw	ionull	; close
	dw	dvspace + 00h

; keyboard
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

; serial
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

; console
cdescr	dw	cread	; read
	dw	cwrite	; write
	dw	ioerr	; seek
	dw	cgetc	; getc
	dw	cputc	; putc
	dw	cctrl	; control
	dw	cinit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 30h ; needs 32 bytes!

; multi (console+serial)
mdescr	dw	mread	; read
	dw	mwrite	; write
	dw	ioerr	; seek
	dw	mgetc	; getc
	dw	mputc	; putc
	dw	ioerr	; control
	dw	minit	; init
	dw	ioerr	; open
	dw	ioerr	; close
	dw	dvspace + 50h

; rootfs
rdescr	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	ioerr	; getc
	dw	ioerr	; putc
	dw	ioerr	; control
	dw	rinit	; init
	dw	ropen	; open
	dw	ioerr	; close
	dw	dvspace + 60h ; needs at least 27 bytes

; memory
mmdescr	dw	mmread	; read
	dw	mmwrite	; write
	dw	mmseek	; seek
	dw	mmgetc	; getc
	dw	mmputc	; putc
	dw	mmctrl	; control
	dw	mminit	; init
	dw	mmopen	; open
	dw	mmclose	; close
	dw	dvspace + 80h

; sd card
sddescr	dw	sdread	; read
	dw	sdwrite	; write
	dw	sdseek	; seek
	dw	sdgetc	; getc
	dw	sdputc	; putc
	dw	sdctrl	; control
	dw	sdinit	; init
	dw	sdopen	; open
	dw	sdclose	; close
	dw	dvspace + 90h

; fat32
ftdescr	dw	ioerr	; read
	dw	ioerr	; write
	dw	ioerr	; seek
	dw	ioerr	; getc
	dw	ioerr	; putc
	dw	ioerr	; control
	dw	ftinit	; init
	dw	ftopen	; open
	dw	ioerr	; close
	dw	dvspace + 0A0h

; initialize device list
sysinit ld	hl,mdescr
	ld	(dvlist+term),hl
	ld	hl,rdescr
	ld	(dvlist+rootfs),hl
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
; initialize rootfs
	ld	ix,rdescr
	call	dvinit
; initialize memory
	ld	ix,mmdescr
	ld	de,0
	call	dvinit
; initialize sdcard
	ld	ix,sddescr
	ld	b,0d0h
	ld	de,sdbufs
	call	dvinit
; initialize fat32
	ld	ix,ftdescr
	ld	de,sddescr
	call	dvinit
; done
	ret
