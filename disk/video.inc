theme	equ	1
defattr	equ	67h

	include	a:video.def

; device descriptor
video	dw	ioerr ; read
	dw	vwrite
	dw	vseek
	dw	ioerr ; getc
	dw	ioerr ; putc
	dw	ioerr ; control
	dw	vinit ; init
	dw	ioerr ; open
	dw	ioerr ; close
	dw	vdata

vtable	equ	vdata+0 ; currently selected table (I/O address)

vwrite	xor	a,a
	cp	a,b
	call	nz,error ; TODO: support larger writes
	ld	b,c
	ld	a,(vtable)
	ld	c,a
	ex	de,hl
	otir
	ret

vseek	ld	a,e
	out	(vadrl),a
	ld	a,d
	out	(vadrh),a
	ret

; set video parameters
vinit	ld	a,-3
	out	(vscrx),a
	ld	a,-4
	out	(vscry),a
	ld	a,10110011b
	out	(vscrh),a
; load character map
	xor	a,a
	out	(vadrl),a
	out	(vadrh),a
	ld	ix,charmap
	ld	c, 0
vinit0	exx	; alternate
	ld	b,4
vinit1	ld	d,(ix+2)
	ld	e,(ix+3)
	ld	h,(ix+18)
	ld	l,(ix+19)
	exx	; normal
	ld	d,(ix+0)
	ld	e,(ix+1)
	ld	h,(ix+16)
	ld	l,(ix+17)
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
	out	(vpatt+vincr),a
	djnz	vinit2
	ld	de,4
	add	ix,de
	exx	; alternate
	djnz	vinit1
	exx	; normal
	ld	de,16
	add	ix,de
	inc	c
	jr	nz,vinit0
; load palettes
	ld	hl,pdark+theme*24
	call	loadpal
; clear name and attribute tables
	ld	bc,2000h
vinit3	ld	a,defattr
	out	(vattr),a
	ld	a,' '
	out	(vname+vincr),a
	dec	bc
	ld	a,b
	or	a,c
	jr	nz,vinit3
; initialize data block
	ld	hl,vname+vincr
	ld	(vtable),hl
	ret

; load palettes from 24 colors pointed to by hl:
; 8 background colors
; 8 regular forground colors
; 8 high contrast foreground colors
; intended for use with a 2-font 8x16 character map
loadpal	xor	a,a
	ld	d, h
	ld	e, l
	ld	hl,8
	add	hl,de
	out	(vadrl),a
	out	(vadrh),a
	ld	ix,temp
	ld	(ix+0),8 ; pattern bit count
	ld	(ix+1),1 ; pattern bit width
	ld	(ix+2),4 ; palette sets
loadp0	ld	(ix+3),8 ; background colors
loadp1	ld	(ix+4),16; foreground colors
; output a single palette
loadp2	ld	c,(ix+0)
loadp3	ld	a,(de)
	ld	b,(ix+1)
loadp4	out	(vpale+vincr),a
	djnz	loadp4
	ld	a,(hl)
	ld	b,(ix+1)
loadp5	out	(vpale+vincr),a
	djnz	loadp5
	dec	c
	jr	nz,loadp3
; next foreground color
	inc	hl
	dec	(ix+4)
	jr	nz,loadp2
; next background color
	ld	bc,-16
	add	hl,bc
	inc	de
	dec	(ix+3)
	jr	nz,loadp1
; next palette set
	ex	de,hl
	ld	bc,-8
	add	hl,bc
	ex	de,hl
	srl	(ix+0)
	sla	(ix+1)
	dec	(ix+2)
	jr	nz,loadp0
	ret

; default palettes
pdark	db	000h,005h,011h,015h,041h,045h,051h,054h
	db	002h,019h,020h,029h,081h,098h,0e0h,0fdh
	db	0a8h,01ah,022h,02ah,0c2h,0ddh,0e2h,0feh
plight	db	0ffh,0afh,0bbh,0bfh,0ebh,0efh,0fbh,0abh
	db	0fch,00eh,023h,02bh,0c3h,0deh,0e3h,003h
	db	057h,00dh,021h,028h,0c1h,0dch,0e1h,001h

