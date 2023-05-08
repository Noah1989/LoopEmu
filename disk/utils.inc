	include	a:utils.def

	org	switch
	pop	hl
	ld	bc,3
switch1	cp	a,(hl)
	jr	nz,switch3
switch2	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	jp	(hl)
switch3	add	hl,bc
	ld	e,a
	xor	a,a
	cp	a,(hl)
	jr	z,switch2
	ld	a,e
	jr	switch1

	org	fromhex
	call	hexnib
	rlca
	rlca
	rlca
	rlca
	ld	c,a
	call	hexnib
	or	a,c
	ret
hexnib	ld	a,(hl)
	inc	hl
	cp	a,'0'
	call	c,error
	cp	a,'9'+1
	jr	nc,hexnib1
	sub	a,'0'
	ret
hexnib1	or	a,20h ; to lowercase
	cp	a,'a'
	call	c,error
	cp	a,'f'+1
	call	nc,error
	add	a,0ah-'a'
	ret

