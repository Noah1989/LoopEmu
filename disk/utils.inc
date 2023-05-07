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

