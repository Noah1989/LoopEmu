; data block indices

ckeybrd	equ	0;..1	keyboard device attached to the console
cvideo	equ	2;..3	video device attached to the console
cmodif	equ	4;	console keyboard modifiers
ctxta	equ	5;	text attribute
ccura	equ	6;	saved attribute originally below cursor
cposx	equ	7;	cursor column
cposy	equ	8;	cursor row
cscroll	equ	9;	scroll lines
cstate	equ	10;	state bits
cansid	equ	16;..31	ansi escape sequence data (16 bytes)

	include	a:ansi.inc

; modifier bitmasks
cmshftl	equ	001b
cmshftr	equ	010b
cmshift	equ	011b

; state bit numbers
csansi	equ	0

; cursor xor mask
ccurmsk	equ	077h

cinit	call	dvdatiy
	ld	(iy+ckeybrd),e
	ld	(iy+ckeybrd+1),d
	ld	(iy+cvideo),c
	ld	(iy+cvideo+1),b
	ld	(iy+cmodif),0
	ld	(iy+ctxta),defattr
	ld	(iy+ccura),defattr
	ld	(iy+cposx),0
	ld	(iy+cposy),0
	ld	(iy+cscroll),0
	ld	(iy+cstate),0
	call	ansiclr
	ret

; INPUT ROUTINES

cread	equ	dvgetcr

cgetc	call	cgetkey
; special key prefix (ignored)
	cp	a,0e0h
	jr	z,cgetc
; key release prefix
	cp	a,0f0h
	jr	z,ckeyup
; modifier keys
	ld	c,0
	call	cmod
	jr	z,cgetc
; normal keys
	ld	h,0
	ld	l,a
	ld	bc,ckeys
	add	hl,bc
	call	dvdatiy
	ld	a,(iy+cmodif)
	and	a,cmshift
	jr	z,cgetc1
	ld	bc,128
	add	hl,bc
cgetc1	ld	a,(hl)
	ret
ckeyup	call	cgetkey
; modifier release
	ld	c,0ffh
	call	cmod
	jr	cgetc

cmod	ld	b,cmshftl
	cp	a,12h
	jr	z,cmod1
	ld	b,cmshftr
	cp	a,59h
	jr	z,cmod1
	ret	; z cleared
cmod1	call	dvdatiy
	ld	a,c
	and	a,b
	ld	c,a
	ld	a,(iy+cmodif)
	or	a,b
	xor	a,c
	ld	(iy+cmodif),a
	xor	a,a
	ret	; z set

cgetkey	call	dvdatiy
	push	ix
	ld	a,(iy+ckeybrd)
	db	0ddh ; use ixl instead of l
	ld	l,a
	ld	a,(iy+ckeybrd+1)
	db	0ddh ; use ixh instead of h
	ld	h,a
	rst	dvgetc
	pop	ix
	ret

ckeys	db	000,000,000,000, 000,000,000,000	;00h..07h
	db	000,000,000,000, 000,000,'`',000	;08h..0fh
	db	000,000,000,000, 000,'q','1',000	;10h..17h
	db	000,000,'z','s', 'a','w','2',000	;18h..1fh
	db	000,'c','x','d', 'e','4','3',000	;20h..27h
	db	000,' ','v','f', 't','r','5',000	;28h..2fh
	db	000,'n','b','h', 'g','y','6',000	;30h..37h
	db	000,000,'m','j', 'u','7','8',000	;38h..3fh
	db	000,',','k','i', 'o','0','9',000	;40h..47h
	db	000,'.','/','l', ';','p','-',000	;48h..4fh
	db	000,000,27h,000, '[','=',000,000	;50h..57h
	db	000,000,0dh,']', 000,5ch,000,000	;58h..5fh
	db	000,000,000,000, 000,000,7fh,000	;60h..67h
	db	000,000,000,000, 000,000,000,000	;68h..6fh
	db	000,000,000,000, 000,000,1bh,000	;70h..77h
	db	000,000,000,000, 000,000,000,000	;78h..7fh
;shifted
	db	000,000,000,000, 000,000,000,000	;00h..07h
	db	000,000,000,000, 000,000,'~',000	;08h..0fh
	db	000,000,000,000, 000,'Q','!',000	;10h..17h
	db	000,000,'Z','S', 'A','W','@',000	;18h..1fh
	db	000,'C','X','D', 'E','$','#',000	;20h..27h
	db	000,' ','V','F', 'T','R','%',000	;28h..2fh
	db	000,'N','B','H', 'G','Y','^',000	;30h..37h
	db	000,000,'M','J', 'U','&','*',000	;38h..3fh
	db	000,'<','K','I', 'O',')','(',000	;40h..47h
	db	000,'>','?','L', ':','P','_',000	;48h..4fh
	db	000,000,'"',000, '{','+',000,000	;50h..57h
	db	000,000,0ah,'}', 000,'|',000,000	;58h..5fh
	db	000,000,000,000, 000,000,08h,000	;60h..67h
	db	000,000,000,000, 000,000,000,000	;68h..6fh
	db	000,000,000,000, 000,000,1bh,000	;70h..77h
	db	000,000,000,000, 000,000,000,000	;78h..7fh

cctrl	ld	b,a
	call	dvdatiy
	push	ix
	ld	a,(iy+ckeybrd)
	db	0ddh ; use ixl instead of l
	ld	l,a
	ld	a,(iy+ckeybrd+1)
	db	0ddh ; use ixh instead of h
	ld	h,a
	ld	a,b
	rst	dvctrl
	pop	ix
	ret

; OUTPUT ROUTINES

cwrite	equ	dvputcr

cputc	ld	b,a
	call	dvdatiy
	bit	csansi,(iy+cstate)
	jp	nz,ansiesc
	ld	a,b
	cp	a,7fh	; delete
	jp	z,cputdel
	cp	a,20h
	jr	nc,cputc3
	ld	hl,ccchrtb
	call	switch
	ret
cputc3	push	ix
	push	af
	call	cvdevix
	call	cposde
	call	ccurhid
	pop	af
	call	cchrout
	call	ccuradv
	call	cposde
	call	ccurshw
	pop	ix
	ret

; ordered table of control character handlers
ccchrtb	db	8	; backspace
	dw	cputbs
	db	9	; tab
	dw	cputtab
	db	10	; line feed
	dw	cputlf
	db	12	; form feed (clears screen)
	dw	cputff
	db	13	; carriage return
	dw	cputcr
	db	27	; escape
	dw	cputesc
	db	0	; default
	dw	ionull

cputdel	push	ix
	call	cvdevix
	call	cposde
	call	ccurhid
	dec	(iy+cposx)
	jp	p,cputdl1
	ld	(iy+cposx),0
cputdl1	call	cposde
	ld	a,' '
	call	cchrout
	call	ccurshw
	pop	ix
	ret

cputbs	push	ix
	call	cvdevix
	call	cposde
	call	ccurhid
	dec	(iy+cposx)
	jp	p,cputbs1
	ld	(iy+cposx),79
	dec	(iy+cposy)
	jp	p,cputbs1
	ld	(iy+cposx),0
	ld	(iy+cposy),0
cputbs1	call	cposde
	call	ccurshw
	pop	ix
	ret

cputtab	push	ix
	call	cvdevix
	call	cposde
	call	ccurhid
	ld	a,(iy+cposx)
	add	a,8
	and	a,-7
	cp	a,80
	jr	c,cputtb1
	ld	a,79
cputtb1	ld	(iy+cposx),a
	call	cposde
	call	ccurshw
	pop	ix
	ret

cputlf	push	ix
	call	cvdevix
	call	cposde
	call	ccurhid
	inc	(iy+cposy)
	call	cscradj
	call	cposde
	call	ccurshw
	pop	ix
	ret

cputff	push	ix
	call	cvdevix
	ld	e,(iy+ctxta)
	ld	a,vclear
	push	iy
	call	dvctrl
	pop	iy
	ld	(iy+cposx),0
	ld	(iy+cposy),0
	call	cposde
	call	ccurshw
	pop	ix
	ret

cputcr	push	ix
	call	cvdevix
	call	cposde
	call	ccurhid
	ld	(iy+cposx),0
	call	cposde
	call	ccurshw
	pop	ix
	ret

cputesc	set	csansi,(iy+cstate)
	ret

; get video device
cvdevix	ld	a,(iy+cvideo)
	db	0ddh ; use ixl instead of l
	ld	l,a
	ld	a,(iy+cvideo+1)
	db	0ddh ; use ixh instead of h
	ld	h,a
	ret

; remove the cursor from postion de (requires video device in ix)
ccurhid	push	iy
	rst	dvseek
	ld	a,vattr
	rst	dvctrl
	pop	iy
	ld	a,(iy+ccura)
	push	iy
	rst	dvputc
	pop	iy
	ret

; output printable character from a to position de on video device in ix
cchrout	push	af
	push	iy
	rst	dvseek
	ld	a,vattr
	rst	dvctrl
	pop	iy
	ld	a,(iy+ctxta)
	push	iy
	rst	dvputc
	ld	a,vname
	rst	dvctrl
	pop	iy
	pop	af
	push	iy
	rst	dvputc
	pop	iy
	ret

; advance to next cursor position (requires video device in ix)
ccuradv	ld	a,(iy+cposx)
	inc	a
	cp	a,80
	jr	nc,ccurad1
	ld	(iy+cposx),a
	ret
ccurad1	ld	(iy+cposx),0
	inc	(iy+cposy)
	call	cscradj
	ret

; scroll adjust (requires video device in ix)
cscradj	ld	a,(iy+cposy)
	cp	a,30
	ret	nz
	dec	(iy+cposy)
	ld	a,(iy+cscroll)
	inc	a
	and	a,3fh
	ld	(iy+cscroll),a
	ld	l,a
	ld	h,0
	ld	l,a
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	ex	de,hl
	ld	a,vscry
	push	iy
	rst	dvctrl
	pop	iy
	ld	a,(iy+cscroll)
	add	a,(iy+cposy)
	and	a,1fh
	ld	d,a
	ld	e,(iy+ctxta)
	push	iy
	ld	a,vnewln
	rst	dvctrl
	pop	iy
	ret

; draw the cursor at position de (requires video device in ix)
ccurshw	push	iy
	rst	dvseek
	ld	a,vattr
	rst	dvctrl
	rst	dvgetc
	pop	iy
	ld	(iy+ccura),a
	xor	a,ccurmsk
	rst	dvputc
	ret

; get the video address of the cursor position into de
cposde	ld	a,(iy+cscroll)
	add	a,(iy+cposy)
	ld	b,a
	and	a,1fh
	ld	d,a
	ld	a,b
	and	a,20h
	rlca
	rlca
	add	a,(iy+cposx)
	ld	e,a
	ret
