all: bios.hex shell.hex charmap.hex test.hex

version.def: *.z80 *.inc *.def
	echo "\tdb\t'$$(date +%Y-%m-%dT%H:%M)'\n" > version.def
	unix2dos version.def

bios.hex:  *.inc *.def
shell.hex: *.inc *.def version.def
test.hex:  *.inc *.def

%.hex: %.z80
	tnylpo zasmb $* hex
	tnylpo-convert -c $*.hex -u $*.hex
	objdump -s $*.hex || (rm $*.hex && exit 1)

load: bios.hex shell.hex test.hex
	python3 -u ../tools/loader.py $?
