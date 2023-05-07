rcurdir	equ	0

rinit	call	dvdatiy
	ld	hl,rootdir
	ld	(iy+rcurdir),hl
	ret

ropen	call	dvdatiy
	ld	a,(de)
	and	a,a
	call	z,error
	cp	a,'/'
	jr	z,ropen1
	ld	hl,(iy+curdir)
	jr	ropen2
ropen1	ld	hl,rootdir
	inc	de
ropen2	push	de
	inc	hl
ropen3	inc	hl
	ld	a,(de)
	inc	de
	cp	a,(hl)
	jr	nz,ropen4
	and	a,a
	jr	nz,ropen3 
; final match
	inc	hl
	ld	a,(hl) ; entry type
; difference found
ropen4	cp	a,'/'
	jr	nz,ropen5
	ld	a,(hl)
	and	a,a
	jr	nz,ropen5
; subpath matched (difference was '/' vs. nul)
	inc	hl
	ld	a,(hl)
	cp	a,fsdir
	call	nz,error
	
; (sub)path did not match, try next entry
ropen5

rootdir	dw	0
	db	'devices',0
	db	fsdir
	dw	devdir
	db	0

devdir	dw	rootdir
	db	'memory',0
	db	fsdev
	dw	mmdescr
	db	'video',0
	db	fsdir
	dw	vdevdir
	db	0

vdevdir	dw	devdir
	db	'attribute',0
	db	fsdevc
	dw	vattrdc
	db	'name',0
	db	fsdevc
	dw	vnamedc
	db	'palette',0
	db	fsdevc
	dw	vpaledc
	db	'pattern',0
	db	fsdevc
	dw	vpattdc
	db	0

vattrdc	dw	vdescr
	db	vattr
vnamedc	dw	vdescr
	db	vname
vpaledc	dw	vdescr
	db	vpale
vpattdc	dw	vdescr
	db	vpatt

