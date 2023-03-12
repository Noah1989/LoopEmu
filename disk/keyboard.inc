kinit	ret

kgetc	in	a,(0c2h)
	and	a,00000001b
	jr	z,kgetc
	in	a,(0c0h)
	ret

