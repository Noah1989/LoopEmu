rcurdir	equ	 0;~ 1	current directory
rmmdesc	equ	 2;~22	dynamically allocated device descriptor
rmmdata	equ	22;~27	and data for memory-mapped directory

rinit	call	dvdatiy
	ld	hl,rootdir
	ld	(iy+rcurdir),l
	ld	(iy+rcurdir+1),h
	ret

ropen	call	dvdatiy
	call	rpath
	ld	a,(hl) 	; entry type
	inc	hl
	cp	a,fsdev
	jr	nz,ropen1
; open device
ropen0	ld	a,(hl)
	db	0ddh ; use ixl
	ld	l,a
	inc	hl
	ld	a,(hl)
	db	0ddh ; use ixh
	ld	h,a
	call	dvopen
	ret
ropen1	cp	a,fsdir
	jr	nz,ropen2
; open directory
ropen2	rst	error ; cannot open this type

; get entry by path (expects device data pointer in iy, path in de)

rpath	call	dvdatiy
	ld	a,(de)
	and	a,a
	call	z,error
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
	inc	hl
	pop	de ; conveniently points to filename part now
	ret
; difference found
rpath4	cp	a,'/'
	jr	nz,rpath5
	ld	a,(hl)
	and	a,a
	jr	nz,rpath5
; subpath matched (difference was '/' vs. nul)
	inc	hl
	ld	a,(hl)
	cp	a,fsdir
	call	nz,error ; not a directory
	pop	bc ; old saved subpath pointer no longer needed
	push	de ; this is the new one, pointing to remaining part
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
	db	fsdir
	dw	devdir
	db	'files',0
	db	fsmount
	dw	ftdescr
	db	0

devdir	dw	rootdir
	db	'error',0,fsdev
	dw	dverror
	db	'memory',0,fsdev
	dw	mmdescr
	db	'nothing',0,fsdev
	dw	dvnull
	db	'sdcard',0,fsdev
	dw	sddescr
	db	'timekeeper',0,fsdev
	dw	tkdescr
	db	'video',0,fsdev
	dw	vdescr
	db	0
