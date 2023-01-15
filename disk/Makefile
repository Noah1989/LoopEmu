all: bios.hex charmap.hex

bios.hex: *.inc

%.hex: %.z80
	tnylpo zasmb $* hex
	tnylpo-convert -c $*.hex -u $*.hex
	objdump -s $*.hex
