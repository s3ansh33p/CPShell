# convert_fonts.py by InterChan

# this script converts pngs in the "fonts" folder and saves them in the "res/folder_name/fnt" folder which you then copy onto your Classpad
# converted fonts are in 1-bit opacity and the image resolution (stored in 4 bytes) is added before the actual image data
# there are 95 characters in total, in ASCII order (from 32 to 126, including space as the first character)
# when making your font, arrange the characters in 19x5 configuration and ensure each character is the same size

# set a custom folder name to export textures to, or leave blank to automatically use this file's directory folder name
folder_name = ""
# only pixels of this color will be read as 1, any other color is transparent in the converted font
pixel_color = (255, 255, 255)


import glob
import os
from PIL import Image

font_cols = 19
font_rows = 5
font_gap_x = 1
font_gap_y = 1

def uint16to8(input16):
	return (input16 >> 8) & 0xFF, input16 & 0xFF

if folder_name == "":
	folder_name = os.path.basename(os.getcwd())
fonts = []
for imgpath in glob.iglob("fonts/**/*.png", recursive = True):
	fonts.append(imgpath)
for font in fonts:
	imgobject = Image.open(font)
	char_size_x = int(imgobject.size[0] / font_cols - font_gap_x)
	char_size_y = int(imgobject.size[1] / font_rows - font_gap_y)
	contents = [0] * (int(char_size_x * char_size_y * 95 / 8) + 5)
	contents[0], contents[1] = uint16to8(char_size_x)
	contents[2], contents[3] = uint16to8(char_size_y)
	current_byte = 4
	current_bit = 128
	temp_byte = 0
	imgpixels = imgobject.load()
	for b in range(font_rows):
		for a in range(font_cols):
			for y in range(char_size_y):
				for x in range(char_size_x):
					pxl = imgpixels[a * (char_size_x + font_gap_x) + x, b * (char_size_y + font_gap_y) + y]
					pxl = (pxl[0], pxl[1], pxl[2])
					if pxl == (255, 255, 255):
						temp_byte |= current_bit
					current_bit >>= 1
					if current_bit < 1:
						contents[current_byte] = temp_byte
						current_bit = 128
						current_byte += 1
						temp_byte = 0
	if current_bit > 0:
		contents[current_byte] = temp_byte
	filepath = "res/" + folder_name + "/fnt/" + font[6:-4]
	if not os.path.exists(os.path.dirname(filepath)):
		os.makedirs(os.path.dirname(filepath))
	file = open(filepath, "wb")
	file.write(bytearray(contents))
	file.close()
