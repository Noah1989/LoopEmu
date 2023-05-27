; device data block offsetrs

sdbufp	equ	0	; pointer to 512-byte buffer
sdstate	equ	2	; current state
sdport	equ	3	; i/o port base
sdblk	equ	4	; current block

; state bits
sdsopen	equ	0	; open
sdsdrty	equ	1	; dirty

; control bytes
sdcdes	equ	04h	; 312.5 kHz, deselected
sdcsel	equ	14h	; 312.5 kHz, selected

sdinit	call	dvdatiy
	ld	(iy+sdport),b
	ld	(iy+sdbufp+1),d
	ld	(iy+sdbufp),e
	ld	(iy+sdstate),0
	ret

sdopen	call	dvdatiy
	bit	sdsopen,(iy+sdstate)
	call	nz,error ; already open
; 80 clock cycles while deselected to allow for startup
	ld	c,(iy+sdport)
	inc	c ; control port
	ld	a,sdcdes
	out	(c),a
	dec	c ; data port
	ld	a, 0ffh
	ld	b,10
sdopen0	out	(c),a
	djnz	sdopen0
; select card
	inc	c ; control port
	ld	a, sdcsel
	out	(c),a
; enter idle mode (CMD0)
	ld	bc,-1
sdopen1	push	bc
	xor	a,a
	ld	d,a
	ld	e,a
	ld	h,a
	ld	l,a
	ld	b,95h
	call	sdcmd
	call	sdrsp1
	pop	bc
	cp	a,01h
	jr	z,sdopen2
	dec	bc
	ld	a,b
	or	a,c
	jr	nz,sdopen1
	rst	error ; timeout
; send interface condition (CMD8)
sdopen2	ld	a,8
	ld	de,0
	ld	hl,01aah
	ld	b,87h
	call	sdcmd
	call	sdrsp37
	ld	a,d
	or	a,e
	call	nz,error
	ld	a,01h
	cp	a,h
	call	nz,error
	ld	a,0aah
	cp	a,l
	call	nz,error
; read operation conditions (check voltage)
	ld	a,58
	call	sdcmd
	call	sdrsp37
	ld	a,e
	and	a,01111000 ; 3.1~3.5V
	cp	a,01111000
	call	nz,error
; send operation conditions (ACMD41)
	ld	bc,-1
sdopen3	push	bc
	ld	a,55
	call	sdcmd
	call	sdrsp1
	ld	a,41
	ld	de,4000h ; host supports high capicity
	ld	hl,0
	call	sdcmd
	call	sdrsp1
	pop	bc
	and	a,a
	jr	z,sdopen4
	dec	bc
	ld	a,b
	or	a,c
	jr	nz,sdopen3
	rst	error ; timeout
; read operation conditions again (check power and capacity)
sdopen4	ld	a,58
	call	sdcmd
	call	sdrsp37
	bit	7,d ; power up
	call	z,error
	bit	6,d ; this is 1 if the card is SDHC/SDXC
	call	z,error ; SDSC not supported (yet)
; done
	set	sdsopen,(iy+sdstate)
	ret

; send command a with argument dehl, checksum b
sdcmd	set	0,b ; end bit
	ld	c,(iy+sdport)
	or	a,40h
	out	(c),a
	out	(c),d
	out	(c),e
	out	(c),h
	out	(c),l
	out	(c),b
	ret

; get R1 respone (single byte) from card into a
; z flag indicates failure
sdrsp1	ld	c,(iy+sdport)
	ld	b,8
sdrsp11	in	a,(c)
	cp	0ffh
	ret	nz
	djnz	sdrsp11
	ret	; note z is set here

; get R3 or R7 response (5 bytes) from card into a, de and hl
sdrsp37	call	sdrsp1
	cp	a,2
	call	nc,error
	in	d,(c)
	in	e,(c)
	in	h,(c)
	in	l,(c)
	ret

; not implemented yet
sdread
sdwrite
sdseek
sdgetc
sdputc
sdctrl
sdclose
	rst	error

