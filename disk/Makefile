all: bios.hex charmap.hex test.hex

bios.hex: *.inc *.def
test.hex: *.def

%.hex: %.z80
	tnylpo zasmb $* hex
	tnylpo-convert -c $*.hex -u $*.hex
	objdump -s $*.hex || (rm $*.hex && exit 1)
