; data block indices
ckeybrd	equ	0;..1	keyboard device attached to the console
cvideo	equ	2;..3	video device attached to the console
cmodif	equ	4;	console keyboard modifiers
ctxta	equ	5;	text attribute
ccura	equ	6;	cursor attribute
cposx	equ	7;	cursor column
cposy	equ	8;	cursor row
cscroll	equ	9;	scroll lines
cescch	equ	10;	escape sequenc (current char)
cescc1	equ	11;	C1 control code of escape sequence (e.g. '[' for CSI)
cescarg	equ	12;	argument for CSI sequence (currently only one)

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
	ld	(iy+ccura),[defattr*16&0f0h]|[defattr/16&0fh]
	ld	(iy+cposx),0
	ld	(iy+cposy),0
	ld	(iy+cscroll),0
	ld	(iy+cescch),0
	ld	(iy+cescc1),0
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
	ld	a,(iy+cescch)
	and	a,a
	jr	z,cputc00
	ld	(iy+cescch),b
	ld	b,27
	jr	cputc0
cputc00	ld	a,b
	cp	a,7fh ; erase
	jr	nz,cputc0
	ld	a,(iy+cposx)
	and	a,a
	ret	z
	push	iy
	ld	a,8
	rst	dvputc
	ld	a,' '
	rst	dvputc
	pop	iy
	ld	b,8
cputc0	push	ix
	push	bc
	ld	a,(iy+cvideo)
	db	0ddh ; use ixl instead of l
	ld	l,a
	ld	a,(iy+cvideo+1)
	db	0ddh ; use ixh instead of h
	ld	h,a
;ix points to video device now
	call	cposde
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
	jp	nc,cputc1
	cp	a,10 ; line feed
	jr	nz,cputc0a
	inc	(iy+cposy)
	jp	cputc2
cputc0a	cp	a,13 ; carriage return
	jr	nz,cputc0b
	ld	(iy+cposx),0
	jp	cputc2
cputc0b	cp	a,8 ; backspace
	jr	nz,cputc0c
	dec	(iy+cposx)
	jp	p,cputc2
	ld	(iy+cposx),79
	dec	(iy+cposy)
	jp	p,cputc2
	ld	(iy+cposx),0
	ld	(iy+cposy),0
	jp	cputc2
cputc0c cp	a,12 ; form feed (clear screen)
	jr	nz,cputc0d
	ld	e,(iy+ctxta)
	ld	a,vclear
	push	iy
	call	dvctrl
	pop	iy
	ld	(iy+cposx),0
	ld	(iy+cposy),0
	jp	cputc2
cputc0d	cp	a,9 ; tab
	jr	nz,cputc0e
	ld	a,(iy+cposx)
	add	a,8
	and	a,0f8h
	cp	a,80
	adc	a,-1
	ld	(iy+cposx),a
	jp	cputc2
cputc0e cp	a,27 ; escape
	jp	nz,cputc0f
	ld	a,(iy+cescch)
	and	a,a
	jr	nz,cputces
	ld	(iy+cescch),27
	pop	ix
	ret
; next byte in escape sequence
cputces ld	a,(iy+cescc1)
	and	a,a
	jr	z,cputcc1
; we already have c1 here (must be CSI, since nothing else is supported yet)
	ld	a,(iy+cescch)
	cp	40h
	jr	nc,cputcet
; another part of the CSI sequence (argument)
	sub	a,30h
	ld	b,a
	ld	a,(iy+cescarg)
	ld	c,a
	rlca	;*2
	rlca	;*4
	rlca	;*8
	add	a,c;*9
	add	a,c;*10
	add	a,b
	ld	(iy+cescarg),a
	pop	ix
	ret
; read c1 control character
cputcc1	ld	a,(iy+cescch)
	cp	a,'['
	jp	nz,cputcex
	ld	(iy+cescc1),a
	ld	(iy+cescarg),0
	pop	ix
	ret
; terminator of CSI sequence found
cputcet	cp	a,'m'
	jp	nz,cputcex
; select graphic rendition
	ld	a,(iy+cescarg)
	and	a,a	; 0 - default style
	jr	nz,cputcg1
	ld	a,defattr
	ld	(iy+ctxta),a
	jp	cputcex
cputcg1 cp	a,1	; 1 - bold
	jr	nz,cputcg2
	ld	a,(iy+ctxta)
	or	a,80h
	ld	(iy+ctxta),a
	jr	cputcex
cputcg2 cp	a,22	; 22 - regular
	jr	nz,cputcg3
	ld	a,(iy+ctxta)
	and	a,7fh
	ld	(iy+ctxta),a
	jr	cputcex
cputcg3 cp	a,30	; 30-37 - foreground color
	jr	c,cputcex
	cp	a,38
	jr	nc,cputcg4
	sub	a,30
	ld	b,0
	rra
	rl	b
	rra
	rl	b
	rra
	rl	b
	ld	a,(iy+ctxta)
	and	a,0f0h
	add	a,b
	ld	(iy+ctxta),a
	jr	cputcex
cputcg4	cp	a,40	; 40-47 - background color
	jr	c,cputcex
	cp	a,48
	jr	nc,cputcg5
	sub	a,40
	ld	b,0
	rra
	rl	b
	rra
	rl	b
	rra
	rl	b
	ld	a,(iy+ctxta)
	and	a,8fh
	ld	c,a
	ld	a,b
	rlca
	rlca
	rlca
	rlca
	add	a,c
	ld	(iy+ctxta),a
	jr	cputcex
cputcg5	cp	a,90	; 90-97 - bright foreground color
	jr	c,cputcex
	cp	a,98
	jr	nc,cputcg6
	sub	a,90
	ld	b,0
	rra
	rl	b
	rra
	rl	b
	rra
	rl	b
	ld	a,(iy+ctxta)
	and	a,0f0h
	or	a,008h
	add	a,b
	ld	(iy+ctxta),a
	jr	cputcex
cputcg6
; end of escape sequence
cputcex	ld	(iy+cescch),0
	ld	(iy+cescc1),0
	jr	cputc2
cputc0f
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
; scroll adjust
cputc2	ld	a,(iy+cposy)
	cp	a,30
	jr	nz,cputc3
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
; draw cursor
cputc3	call	cposde
	push	iy
	rst	dvseek
	ld	a,vattr
	rst	dvctrl
	pop	iy
	ld	a,(iy+ccura)
	rst	dvputc
	pop	ix
	ret


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
