; list command (argc b, *argv[] de)
; (ix points to standard i/o device)

; buffer size
lstbufs	equ	80

; local variables (offsets)
lstpath	equ	0		; pointer to path
lstbuf	equ	lstpath+2	; buffer
lsttyp	equ	lstbuf+lstbufs	; file type
lstfnl	equ	lsttyp+1	; file name length
lstvars	equ	lstfnl+1	; stack frame size

list	jr	list0
	db	'list',0
list0	; allocate variables on stack
	ld	iy,-lstvars
	add	iy,sp
	ld	sp,iy
	djnz	list1
; no args - list current dir
	ld	de,lstcdir
	jp	list2
lstcdir	db	'.',0
list1	inc	de ; skip arg 0 (program name)
	inc	de
	ex	de,hl ; get path
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
list2	ld	(iy+lstpath),e
	ld	(iy+lstpath+1),d
	push	ix
	ld	ix,(dvlist+rootfs)
	ld	a,fscinfo
	push	iy
	rst	dvctrl
	pop	iy
	call	switch
	db	fstdir ; directory
	dw	lstdir
	db	0 ; default
	dw	error
listend	pop	ix ; stdout
; clean up stack
	ld	hl,lstvars
	add	hl,sp
	ld	sp,hl
	ret

; list directory
lstdir	ld	e,(iy+lstpath)
	ld	d,(iy+lstpath+1)
	push	iy
	call	dvopen
	pop	iy
lstdir0	push	iy
	rst	dvgetc ; skip implementation-specific data
	rst	dvgetc
	pop	iy
	db	0fdh
	ld	e,l;iy
	db	0fdh
	ld	d,h;iy
	ld	hl,lstbuf
	add	hl,de
	ld	b,lstbufs
	push	iy
lstdir1	push	bc
	push	hl
	rst	dvgetc
	pop	hl
	ld	(hl),a
	inc	hl
	and	a,a
	jr	z,lstdir2
	pop	bc
	djnz	lstdir1
	rst	error ; file name too long
lstdir2	rst	dvgetc ; type
	pop	bc
	pop	iy
	ld	(iy+lsttyp),a
	and	a,a
	jr	z,lstdir3 ; end
	ld	a,lstbufs
	sub	a,b ; file name length
	call	z,error ; empty file name
	ld	(iy+lstfnl),a
	ex	(sp),ix ; stdout
	ld	de,lstcol
	ld	bc,lstcoll
	push	iy
	rst	dvwrite
	pop	iy
	ld	a,(iy+lsttyp)
	and	a,7
	add	a,'0'
	push	iy
	rst	dvputc
	ld	a,'m'
	rst	dvputc
	pop	iy
	db	0fdh
	ld	e,l;iy
	db	0fdh
	ld	d,h;iy
	ld	hl,lstbuf
	add	hl,de
	ex	de,hl
	ld	b,0
	ld	c,(iy+lstfnl)
	push	iy
	rst	dvwrite
	ld	de,lstnxt
	ld	bc,lstnxtl
	rst	dvwrite
	pop	iy
	ex	(sp),ix ; directory
	jr	lstdir0
lstdir3	call	dvclose
	jp	listend

lstcol	db	27,'[3'
lstcoll	equ	$-lstcol
lstnxt	db	27,'[0m',13,10
lstnxtl	equ	$-lstnxt

