all: charmap.hex bios.hex

bios.hex: *.inc *.def
test.hex: *.def

%.hex: %.z80
	tnylpo zasmb $* hex
	tnylpo-convert -c $*.hex -u $*.hex
	objdump -s $*.hex || (rm $*.hex && exit 1)

load: bios.hex
	for file in $? ; do \
	python.exe -u ../tools/loader.py $$file || exit 1 ; \
	done
