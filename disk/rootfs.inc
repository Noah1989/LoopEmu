rcurdir	equ	 0;~ 1	current directory
rmmdesc	equ	 2;~21	dynamically allocated device descriptor
rmmdata	equ	22;~26	and data for memory-mapped directory

; template for device descriptor
rmmtplt	dw	mmread	; read
	dw	ioerr	; write
	dw	mmseek	; seek
	dw	mmgetc	; getc
	dw	ioerr	; putc
	dw	ioerr	; control
	dw	mminit	; init
	dw	mmopen	; open
	dw	mmclose	; close
	dw	0

rinit	call	dvdatiy
	ld	hl,rootdir
	ld	(iy+rcurdir),l
	ld	(iy+rcurdir+1),h
	db	0fdh ; use iy
	ld	e,l
	db	0fdh ; use iy
	ld	d,h
	ld	hl,rmmdesc
	add	hl,de
	ex	de,hl
	push	de
	ld	hl,rmmtplt
	ld	bc,ddsize
	ldir	; after this, de points to rmmdata
	ld	(iy+rmmdesc+dvdata),e
	ld	(iy+rmmdesc+dvdata+1),d
	ex	(sp),ix ; pushed de from earlier
	ld	de,rootdir
	call	dvinit
	pop	ix
	ret

rctrl	cp	a,fscinfo
	call	nz,error ; invalid control code
; get file info
rcinfo	inc	de ; return fstdir for . and /
	ld	a,(de)
	dec	de
	and	a,a
	jr	nz,rcinfo1
	ld	a,(de)
	cp	a,'.'
	jr	z,rcinfo0
	cp	a,'/'
	jr	nz,rcinfo1
rcinfo0	ld	a,fstdir
	ret
rcinfo1	call	dvdatiy
	call	rpath
	ld	a,(hl) ; entry type
	cp	a,fstmnt
	ret	nz
	; recurse into other filesystem ...
	ld	a,(de)
	and	a,a ; ... unless subbpath is empty
	jr	nz,rcinfo2
	ld	a,fstmnt
	ret
rcinfo2	push	ix
	inc	hl
	ld	a,(hl)
	db	0ddh ; use ix
	ld	l,a
	inc	hl
	ld	a,(hl)
	db	0ddh ; use ix
	ld	h,a
	ld	a,fscinfo
	rst	dvctrl
	pop	ix
	ret

ropen	call	dvdatiy
	inc	de	; check for special paths / and .
	ld	a,(de)
	dec	de
	and	a,a
	jr	nz,ropen03
	ld	a,(de)
	cp	a,'/'
	jr	nz,ropen02
	ld	de,rootdir
	jr	ropend
ropen02	cp	a,'.'
	jr	nz,ropen03
	ld	e,(iy+rcurdir)
	ld	d,(iy+rcurdir+1)
	jr	ropend
ropen03	call	rpath
	ld	a,(hl) 	; entry type
	inc	hl
	cp	a,fstdir
	jr	z,ropen1
; open device or mount point
	ld	a,(hl)
	db	0ddh ; use ix
	ld	l,a
	inc	hl
	ld	a,(hl)
	db	0ddh ; use ix
	ld	h,a
	call	dvopen ; de points to subpath, if any
	ret
; open directory
ropen1	ld	e,(hl)
	inc	hl
	ld	d,(hl) ; pointer to dir data in de
ropend	bit	mmsopen,(iy+rmmdata+mmstate)
	call	nz,error ; another dir still open
	db	0fdh
	ld	c,l;iy
	db	0fdh
	ld	b,h;iy
	ld	ix,rmmdesc
	add	ix,bc ; memory mapped dir
	call	dvinit ; set offset to de
	call	dvopen
	ret

; get entry by path (expects device data pointer in iy, path in de)
rpath	ld	a,(de)
	and	a,a
	call	z,error ; empty path
	cp	a,'/'
	jr	z,rpath1
	ld	l,(iy+rcurdir)
	ld	h,(iy+rcurdir+1)
	jr	rpath2
rpath1	ld	hl,rootdir
	inc	de
	
rpath2	push	de
	inc	hl
rpath3	inc	hl
rpath31	ld	a,(de)
	inc	de
	cp	a,(hl)
	jr	nz,rpath4
	and	a,a
	jr	nz,rpath3
; final match (both strings ending)
	inc	hl ; points to file type now
	pop	bc ; points to filename part now
	dec	de ; points to null terminator
	ret
; difference found
rpath4	cp	a,'/'
	jr	nz,rpath5
	ld	a,(hl)
	and	a,a
	jr	nz,rpath5
; subpath matched (difference was '/' vs. nul)
	pop	bc ; old saved subpath pointer no longer needed
	inc	hl
	ld	a,(hl) ; check type
	cp	a,fstmnt ; mount point (other file system)
	jr	nz,rpath41
	dec	de	; pointer to remaining path
	ret		; including leading '/'
rpath41	cp	a,fstdir
	call	nz,error ; not a directory
	push	de ; this is the new subpath
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl ; hl points to subdirectory now
	pop	de
	jr	rpath2 ; find next part
; (sub)path did not match, try next entry
rpath5	xor	a
	ld	b,a
	ld	c,a
	cpir	; find end of string
	inc	hl ; skip entry
	inc	hl
	inc	hl
	cp	a,(hl)
	call	z,error ; end of list - path not found
	pop	de
	push	de
	jr	rpath31

rootdir	dw	0
	db	'devices',0
	db	fstdir
	dw	devdir
	db	'files',0
	db	fstmnt
	dw	f32desc
	db	0,0

devdir	dw	rootdir
	db	'error',0,fstdev
	dw	dverror
	db	'memory',0,fstdev
	dw	mmdescr
	db	'nothing',0,fstdev
	dw	dvnull
	db	'sdcard',0,fstdev
	dw	sddescr
	db	'timekeeper',0,fstdev
	dw	tkdescr
	db	'video',0,fstdev
	dw	vdescr
	db	0,0
