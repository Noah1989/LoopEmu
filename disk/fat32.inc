f32blkd	equ	0;~1	pointer to block device
f32omsk	equ	2;	bitmask of open files
; read from disk
f32bgin	equ	3;~6	LBA of FAT32 volume id
f32clsz	equ	7;	cluster size in blocks (always a power of 2)
f32rsrv	equ	8;~9	number of reserved blocks (usually 32)
f32spft	equ	10;~13	sectors per FAT
f32rdfc	equ	14;~17	root dir first cluster
; calculated values
f32fat	equ	18;~21	LBA of FAT
f32cl0	equ	22;~25	LBA of (imaginary) cluster 0 (clusters start at 2!)
f32shft	equ	26;	log2 of blocks per cluster (for bit shift multiply)

f32init	call	dvdatiy
	ld	(iy+f32blkd),e
	ld	(iy+f32blkd+1),d
	xor	a,a
	ld	(iy+f32omsk),0
	ret

f32open	call	dvdatiy
	push	ix	; self pointer
	push	de	; points to path (may be empty)
	ld	e,(iy+f32blkd)
	ld	d,(iy+f32blkd+1)
	db	0ddh ; ix
	ld	l,e
	db	0ddh ; ix
	ld	h,d
	; from now on ix is the block device
	ld	a,(iy+f32omsk)
	and	a,a
	jp	nz,f32opn1

	; open block device and read file system structure
	; we do this again when no files are open to allow changing media
	; block device will be closed when closing the last file
	push	iy
	call	dvopen
	; check MBR signature
	exx
	ld	bc,0
	exx
	ld	bc,0
	ld	de,1feh ; location of MBR signature
	rst	dvseek
	rst	dvgetc
	cp	a,055h
	call	nz,error
	rst	dvgetc
	cp	a,0aah
	call	nz,error ; invalid signature
	; check file system type
	exx
	ld	bc,0
	exx
	ld	bc,0
	ld	de,446+4 ; type code of first partition
	rst	dvseek
	rst	dvgetc
	cp	a,0bh
	jr	z,f32op01
	cp	a,0ch
	call	nz,error ; wrong file system type
f32op01	; locate volume id
	exx
	ld	bc,0
	exx
	ld	bc,0
	ld	de,446+8 ; LBA begin of first partition
	rst	dvseek
	pop	hl ; was iy
	push	hl
	ld	bc,f32bgin
	add	hl,bc
	ex	de,hl
	ld	bc,4
	rst	dvread
	; check volume id signature
	pop	iy
	ld	c,(iy+f32bgin)
	ld	b,(iy+f32bgin+1)
	exx
	ld	c,(iy+f32bgin+2)
	ld	b,(iy+f32bgin+3)
	exx
	ld	de,1feh
	push	iy
	rst	dvseek
	rst	dvgetc
	cp	a,055h
	call	nz,error
	rst	dvgetc
	cp	a,0aah
	call	nz,error ; invalid signature
	pop	iy
	; check bytes per sector
	ld	c,(iy+f32bgin)
	ld	b,(iy+f32bgin+1)
	exx
	ld	c,(iy+f32bgin+2)
	ld	b,(iy+f32bgin+3)
	exx
	ld	de,0bh ; bytes per sector
	push	iy
	rst	dvseek
	rst	dvgetc ; 512 (0200h) expected
	and	a,a
	call	nz,error
	rst	dvgetc
	cp	a,2
	call	nz,error
	; get sectors per cluster (follows immediately)
	rst	dvgetc
	pop	iy
	ld	(iy+f32clsz),a
	; get number of reserved sectors (follows immediately)
	push	iy
	rst	dvgetc
	pop	iy
	ld	(iy+f32rsrv),a
	push	iy
	rst	dvgetc
	pop	iy
	ld	(iy+f32rsrv+1),a
	; verify number of FATs (also follows immediately)
	push	iy
	rst	dvgetc ; should return 2
	cp	a,2
	call	nz,error
	pop	iy
	; get sectors per FAT
	ld	c,(iy+f32bgin)
	ld	b,(iy+f32bgin+1)
	exx
	ld	c,(iy+f32bgin+2)
	ld	b,(iy+f32bgin+3)
	exx
	ld	de,24h ; sectors per FAT
	push	iy
	rst	dvseek
	pop	hl ; was iy
	push	hl
	ld	bc,f32spft
	add	hl,bc
	ex	de,hl
	ld	bc,4
	rst	dvread
	pop	iy
	; get root dir first cluster
	ld	c,(iy+f32bgin)
	ld	b,(iy+f32bgin+1)
	exx
	ld	c,(iy+f32bgin+2)
	ld	b,(iy+f32bgin+3)
	exx
	ld	de,2ch ; root dir first cluster
	push	iy
	rst	dvseek
	pop	hl ; was iy
	push	hl
	ld	bc,f32rdfc
	add	hl,bc
	ex	de,hl
	ld	bc,4
	rst	dvread
	; calculate f32fat = f32bgin + f32rsrv
	pop	iy
	ld	l,(iy+f32bgin)
	ld	h,(iy+f32bgin+1)
	ld	e,(iy+f32bgin+2)
	ld	d,(iy+f32bgin+3)
	ld	c,(iy+f32rsrv)
	ld	b,(iy+f32rsrv+1)
	add	hl,bc
	ex	de,hl
	ld	bc,0
	adc	hl,bc
	ex	de,hl
	ld	(iy+f32fat),l
	ld	(iy+f32fat+1),h
	ld	(iy+f32fat+2),e
	ld	(iy+f32fat+3),d
	; calculate f32cl0 = f32fat + 2*f32spft - 2*f32clsz
	push	de	; save f32fat
	push	hl
	ld	l,(iy+f32spft) ; subtract first (factor out the 2*)
	ld	h,(iy+f32spft+1)
	ld	e,(iy+f32spft+2)
	ld	d,(iy+f32spft+3)
	ld	c,(iy+f32clsz)
	ld	b,0
	or	a,a	; clear carry
	sbc	hl,bc
	ex	de,hl
	ld	c,0
	sbc	hl,bc
	ex	de,hl ; dehl contains f32spft-f32clsz, now multipy by 2
	add	hl,hl
	ex	de,hl
	adc	hl,hl
	ex	de,hl ; now we just need to add f32fat
	pop	bc ; was hl
	add	hl,bc
	ex	de,hl
	pop	bc ; was de
	add	hl,bc
	ex	de,hl
	ld	(iy+f32cl0),l
	ld	(iy+f32cl0+1),h
	ld	(iy+f32cl0+2),e
	ld	(iy+f32cl0+3),d
	; calculate f32shft = log2(f32clsz)
	ld	a,(iy+f32clsz) ; posible values: 1,2,4,8,16,32,64,128
	ld	b,8
f32op02	rlca
	jr	c,f32op03
	djnz	f32op02
	rst	error ; 8 shifts, no carry (a was 0)
f32op03	dec	b
	ld	(iy+f32shft),b

f32opn1	

	pop	de ; saved path parameter

	rst	error	; TODO
	pop	ix
	ret

