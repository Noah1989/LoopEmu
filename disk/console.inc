; data block indices
ckeybrd	equ	0;..1	keyboard device attached to the console
cvideo	equ	2;..3	video device attached to the console
cmodif	equ	4;	console keyboard modifiers
ctxta	equ	5;	text attribute
ccura	equ	6;	cursor attribute
cposx	equ	7;	cursor column
cposy	equ	8;	cursor row


; modifier bitmasks
cmshftl	equ	001b
cmshftr	equ	010b
cmshift	equ	011b

cinit	call	dvdatiy
	ld	(iy+ckeybrd),e
	ld	(iy+ckeybrd+1),d
	ld	(iy+cvideo),c
	ld	(iy+cvideo+1),b
	ld	(iy+cmodif),0
	ld	(iy+ctxta),defattr
	ld	(iy+ccura),defattr~70h
	ld	(iy+cposx),0
	ld	(iy+cposy),0
	ret

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

cwrite	equ	dvputcr

cputc	ld	b,a
	call	dvdatiy
	push	ix
	push	bc
	ld	a,(iy+cvideo)
	db	0ddh ; use ixl instead of l
	ld	l,a
	ld	a,(iy+cvideo+1)
	db	0ddh ; use ixh instead of h
	ld	h,a
;ix points to video device now
	ld	e,(iy+cposx)
	ld	d,(iy+cposy)
	push	iy
	rst	dvseek
; remove cursor
	ld	a,vattr
	rst	dvctrl
	pop	iy
	ld	a,(iy+ctxta)
	push	iy
	rst	dvputc
	pop	iy
; control characters
	pop	af ; previously bc
	cp	a,20h
	jr	nc,cputc1
	cp	a,10 ; line feed
	jr	nz,cputc0a
	inc	(iy+cposy)
	jr	cputc2
cputc0a	cp	a,13 ; carriage return
	jr	nz,cputc2
	ld	(iy+cposx),0
	jr	cputc2
; put character
cputc1	push	af
	ld	a,vname
	push	iy
	rst	dvctrl
	pop	iy
	pop	af
	push	iy
	rst	dvputc
; new cursor position
	pop	iy
	ld	a,(iy+cposx)
	inc	a
	cp	a,80
	jr	c,cputc11
	xor	a,a
	inc	(iy+cposy)
cputc11	ld	(iy+cposx),a
; draw cursor
cputc2	ld	e,(iy+cposx)
	ld	d,(iy+cposy)
	push	iy
	rst	dvseek
	ld	a,vattr
	rst	dvctrl
	pop	iy
	ld	a,(iy+ccura)
	rst	dvputc
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
	db	000,000,000,']', 000,5ch,000,000	;58h..5fh
	db	000,000,000,000, 000,000,000,000	;60h..67h
	db	000,000,000,000, 000,000,000,000	;68h..6fh
	db	000,000,000,000, 000,000,000,000	;70h..77h
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
	db	000,000,000,'}', 000,'|',000,000	;58h..5fh
	db	000,000,000,000, 000,000,000,000	;60h..67h
	db	000,000,000,000, 000,000,000,000	;68h..6fh
	db	000,000,000,000, 000,000,000,000	;70h..77h
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
