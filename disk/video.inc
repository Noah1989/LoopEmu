defattr	equ	07h

	include	a:video.def

; timing values (see silkscreen on card)
vtiming	db	0d9h,039h
	db	0d4h,074h
	db	0f6h,07eh
	db	0e4h,0e2h
	db	0e1h,0f4h
	db	090h

; data block indices
vbase	equ	0 ; base I/O address
vtable	equ	1 ; currently selected table (I/O address)

vctrl	cp	a,vname
	call	c,ioerr
	cp	a,vpale+vincr+1
	call	nc,ioerr
	ld	b,a
	call	dvdatiy
	ld	a,b
	add	a,(iy+vbase)
	ld	(iy+vtable),a
	ret

vwrite	xor	a,a
	cp	a,b
	call	nz,error ; TODO: support larger writes
	ld	b,c
	call	dvdatiy
	ld	c,(iy+vtable)
	ex	de,hl
	otir
	ret

vputc	ld	b,a
	call	dvdatiy
	ld	c,(iy+vtable)
	out	(c),b
	ret

vseek	call	dvdatiy
	ld	a,(iy+vbase)
	add	a,vadrl
	ld	c,a
	out	(c),e
	inc	c;vadrh
	out	(c),d
	ret

; initialize data block
vinit	call	dvdatiy
	ld	(iy+vbase),b
	ld	a,b
	add	a,vname+vincr
	ld	(iy+vtable),a
; set timings
	ld	c,(iy+vbase)
	ld	hl,vtiming
	ld	b,11
vinit00	ld	a,(hl)
	inc	hl
	out	(c),a
	inc	c
	djnz	vinit00
; set video parameters
	ld	a,(iy+vbase)
	add	a,vscrx
	ld	c,a
	ld	a,-3
	out	(c),a
	ld	a,-4
	inc	c;vscry
	out	(c),a
	ld	a,10110011b
	inc	c;vscrh
	out	(c),a
; load character map
	ld	a,b
	add	a,vadrl
	ld	c,a
	xor	a,a
	out	(c),a
	inc	c ; vadrh
	out	(c),a
	exx	; alternate
	ld	a,(iy+vbase)
	add	a,vpatt+vincr
	ld	c,a
	exx	; normal
	push	iy
	ld	iy,charmap
	ld	c, 0
vinit0	exx	; alternate
	ld	b,4
vinit1	ld	d,(iy+2)
	ld	e,(iy+3)
	ld	h,(iy+18)
	ld	l,(iy+19)
	exx	; normal
	ld	d,(iy+0)
	ld	e,(iy+1)
	ld	h,(iy+16)
	ld	l,(iy+17)
	ld	b,8
vinit2	ld	a,80h ; falls into carry after 8 rotations
vinit21	rl	d
	rra
	rl	e
	rra
	rl	h
	rra
	rl	l
	rra
	exx	; always executed twice
	jr	nc,vinit21
	exx	; alternate
	out	(c),a
	exx	; normal
	djnz	vinit2
	ld	de,4
	add	iy,de
	exx	; alternate
	djnz	vinit1
	exx	; normal
	ld	de,16
	add	iy,de
	inc	c
	jr	nz,vinit0
	pop	iy
; load palettes
	ld	hl,defpal
	call	loadpal
; clear name and attribute tables
	ld	a,(iy+vbase)
	add	a,vname+vincr
	ld	c,a
	ld	b,' '
	call	vinit3
	ld	a,(iy+vbase)
	add	a,vattr+vincr
	ld	c,a
	ld	b,defattr
vinit3	ld	de,2000h
vinit4	out	(c),b
	dec	de
	ld	a,e
	or	a,d
	jr	nz,vinit4
	ret

; load palettes from 24 colors pointed to by hl:
; 8 background colors
; 8 regular forground colors
; 8 high contrast foreground colors
; intended for use with a 2-font 8x16 character map
loadpal	ld	d,h
	ld	e,l
	ld	hl,8
	add	hl,de
	ld	a,(iy+vbase)
	add	a,vadrl
	ld	c,a
	xor	a,a
	out	(c),a
	inc	c;vadrh
	out	(c),a
	exx
	ld	a,(iy+vbase)
	add	a,vpale+vincr
	ld	c,a
	exx
	push	iy
	ld	iy,temp
	ld	(iy+0),8 ; pattern bit count
	ld	(iy+1),1 ; pattern bit width
	ld	(iy+2),4 ; palette sets
loadp0	ld	(iy+3),8 ; background colors
loadp1	ld	(iy+4),16; foreground colors
; output a single palette
loadp2	ld	c,(iy+0)
loadp3	ld	a,(de)
	ld	b,(iy+1)
loadp4	exx
	out	(c),a
	exx
	djnz	loadp4
	ld	a,(hl)
	ld	b,(iy+1)
loadp5	exx
	out	(c),a
	exx
	djnz	loadp5
	dec	c
	jr	nz,loadp3
; next foreground color
	inc	hl
	dec	(iy+4)
	jr	nz,loadp2
; next background color
	ld	bc,-16
	add	hl,bc
	inc	de
	dec	(iy+3)
	jr	nz,loadp1
; next palette set
	ex	de,hl
	ld	bc,-8
	add	hl,bc
	ex	de,hl
	srl	(iy+0)
	sla	(iy+1)
	dec	(iy+2)
	jr	nz,loadp0
	pop	iy
	ret

; default palette
defpal	db	000h,008h,020h,028h,080h,088h,0a0h,0a8h
	db	054h,00ah,022h,02ah,082h,08ah,0a2h,0aah
	db	057h,05fh,077h,07fh,0d7h,0dfh,0f7h,0ffh
