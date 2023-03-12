; data block indices
cmodif	equ	0 ; console keyboard modifiers

; modifier bits
cmshift	equ	001b
cmctrl	equ	010b
cmalt	equ	100b

; data block indices
ckeybrd	equ	0 ; keyboard device attached to the console
cvideo	equ	2 ; video device attached to the console

cinit	call	dvdatiy
	ld	(iy+ckeybrd),e
	ld	(iy+ckeybrd+1),d
	ld	(iy+cvideo),c
	ld	(iy+cvideo+1),b
	ret

cgetc	call	dvdatiy
cgetc0	call	cgetkey
	cp	a,0e0h
	jr	z,cgetc
	cp	a,0f0h
	jr	z,ckeyup
cgetc1	ld	h,0
	ld	l,a
	ld	bc,ckeys
	add	hl,bc
	ld	a,(hl)
	ret
ckeyup	call	cgetkey ; discard byte
	jr	cgetc0

cgetkey	push	ix
	ld	a,(iy+ckeybrd)
	db	0ddh ; use ixl instead of l
	ld	l,a
	ld	a,(iy+ckeybrd+1)
	db	0ddh ; use ixh instead of h
	ld	h,a
	rst	dvgetc
	pop	ix
	ret

cputc	ld	c,a
	call	dvdatiy
	push	ix
	ld	a,(iy+cvideo)
	db	0ddh ; use ixl instead of l
	ld	l,a
	ld	a,(iy+cvideo+1)
	db	0ddh ; use ixh instead of h
	ld	h,a
	ld	a,c
	rst	dvputc
	pop	ix
	ret

ckeys	db	000,000,000,000, 000,000,000,000	;00h..07h
	db	000,000,000,000, 000,'`',000,000	;08h..0fh
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
	db	000,000,000,000, 000,'~',000,000	;08h..0fh
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

