from PIL import ImageFont, Image, ImageDraw
import sys, unicodedata

def get_char(byte):
    return byte.decode('ibm437').translate({
        0x01: "\u263A", 0x02: "\u263B", 0x03: "\u2665", 0x04: "\u2666",
        0x05: "\u2663", 0x06: "\u2660", 0x07: "\u2022", 0x08: "\u25D8",
        0x09: "\u25CB", 0x0a: "\u25D9", 0x0b: "\u2642", 0x0c: "\u2640",
        0x0d: "\u266A", 0x0e: "\u266B", 0x0f: "\u263C", 0x10: "\u25BA",
        0x11: "\u25C4", 0x12: "\u2195", 0x13: "\u203C", 0x14: "\u00B6",
        0x15: "\u00A7", 0x16: "\u25AC", 0x17: "\u21A8", 0x18: "\u2191",
        0x19: "\u2193", 0x1a: "\u2192", 0x1b: "\u2190", 0x1c: "\u221F",
        0x1d: "\u2194", 0x1e: "\u25B2", 0x1f: "\u25BC", 0x7f: "\u2302",
    })

font = ImageFont.truetype("terminus-normal.otb", 16)
bold = ImageFont.truetype("terminus-bold.otb", 16)

print ("; Terminus Font")
print ("\torg\t5000h\t; 5000h-6fffh")

for i in range(256):
    byte = i.to_bytes(1, 'little')
    char = get_char(byte)
    if i == 0:
        name = "NULL"
    else:
        name = unicodedata.name(char)
    print(";", i, name)
    canvas = Image.new('RGB', (16, 16), 'black')
    ImageDraw.Draw(canvas).text((0, 0), char, 'white', font)
    ImageDraw.Draw(canvas).text((8, 0), char, 'white', bold)
    for y in range(16):
        print("\tdb\t", end='')
        for x in range(16):
            if (canvas.getpixel((x,y))[0]):
                print('1', end='')
            else:
                print('0', end='')
            if x == 7:
                print('b, ', end='')
        print('b')
