; data offsets (relative to iy+cansid)

ansic1	equ	0;	C1 character
ansibln	equ	1;	sequence buffer length (without null terminator)
ansibuf	equ	2;..14	sequence buffer (max. 13 bytes)
ansibnt	equ	15;	reserved space for null termination

ansimax	equ	ansibnt-ansibuf

; clear state
ansiclr	ld	(iy+cansid+ansic1),0
	ld	(iy+cansid+ansibln),0
	ld	(iy+cansid+ansibuf),0
	ret

; handle ansi escape sequence character in b
ansiesc	ld	a,(iy+cansid+ansic1)
	and	a,a
	jr	nz,ansies1
	ld	a,b
	cp	a,'['	; control sequence introducer
	jr	nz, ansiend
	ld	(iy+cansid+ansic1),a
	ret
; no need to check the C1 chracter here, since only CSI is supported for now
ansies1	ld	a,b
	cp	a,40h ; final byte >= 40h
	jr	c,ansies2
	ld	hl,ansiftb
	call	switch
	jr	ansiend
; store into buffer
ansies2	ld	e,a
	ld	a,(iy+cansid+ansibln)
	cp	a,ansimax
	jr	nc,ansiend ; buffer full
	add	a,cansid+ansibuf
	ld	l,a
	ld	h,0
	db	0fdh ; use iy instead of hl
	ld	c,l
	db	0fdh ; use iy instead of hl
	ld	b,h
	add	hl,bc
	ld	(hl),e
	inc	hl
	ld	(hl),0
	inc	(iy+cansid+ansibln)
	ret
; end of escape sequence
ansiend	call	ansiclr
	res	csansi,(iy+cstate)
	ret

; CSI final byte switch table
ansiftb	db	'm'	; select graphic rendition
	dw	ansisgr
	db	0	; default
	dw	ionull

ansisgr	db	0fdh ; use iy instead of hl
	ld	e,l
	db	0fdh ; use iy instead of hl
	ld	d,h
	ex	de,hl
	ld	bc,cansid+ansibuf
	add	hl,bc
ansisg0	xor	a,a
	call	ansiarg
	and	a,a	; 0 - default style
	jr	nz,ansisg1
	ld	a,defattr
	ld	(iy+ctxta),a
	jp	ansisg9
ansisg1	cp	a,1	; 1 - bold
	jr	nz,ansisg2
	ld	a,(iy+ctxta)
	or	a,80h
	ld	(iy+ctxta),a
	jr	ansisg9
ansisg2	cp	a,22	; 22 - regular
	jr	nz,ansisg3
	ld	a,(iy+ctxta)
	and	a,7fh
	ld	(iy+ctxta),a
	jr	ansisg9
ansisg3	cp	a,30	; 30-37 - foreground color
	jr	c,ansisg9
	cp	a,38
	jr	nc,ansisg4
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
	jr	ansisg9
ansisg4	cp	a,40	; 40-47 - background color
	jr	c,ansisg9
	cp	a,48
	jr	nc,ansisg5
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
	jr	ansisg9
ansisg5	cp	a,90	; 90-97 - bright foreground color
	jr	c,ansisg9
	cp	a,98
	jr	nc,ansisg6
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
	jr	ansisg9
ansisg6
ansisg9	ld	a,(hl)
	and	a,a
	ret	z
	inc	hl ; skip separator
	jp	ansisg0


; get argument from string at hl, default value from a unchanged if arg is empty
; upon return, hl points to the first non-numeric character encountered
ansiarg	ld	c,-1 ; gets incremented to 0 on first argument digit
	ld	b,a ; stores partial result
ansiar0	ld	a,(hl)
	cp	a,'0'
	jr	c,ansiar2
	cp	a,'9'+1
	jr	nc,ansiar2
	inc	hl
	inc	c
	jr	nz, ansiar1
	ld	b,0 ; discard default on first digit found
ansiar1	sub	a,'0'
	ld	e,a ; new digit
	ld	a,b ; result so far
	add	a,a ; *2
	add	a,a ; *4
	add	a,a ; *8
	add	a,b ; *9
	add	a,b ; *10
	add	a,e
	ld	b,a
	jr	ansiar0
ansiar2	ld	a,b
	ret

