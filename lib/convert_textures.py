# convert_textures.py by InterChan

# this script converts pngs in the "textures" folder and saves them in the "res" folder which you then copy onto your Classpad
# converted images are in rgb565 (taking up 2 bytes each pixel) and the image resolution (stored in 4 bytes) is added before the actual image data

# set a custom folder name to export textures to, or leave blank to automatically use this file's directory folder name
folder_name = ""
# use this to ensure colors close to your transparency color don't become transparent when converted (because rgb565 has lower precision)
transparency_color = (255, 0, 255)


import glob
import os
from PIL import Image

def rgb888to565(rgbtuple):
	return ((rgbtuple[0] & 0b11111000) << 8) + ((rgbtuple[1] & 0b11111100) << 3) + (rgbtuple[2] >> 3)

def uint16to8(input16):
	return (input16 >> 8) & 0xFF, input16 & 0xFF

if folder_name == "":
	folder_name = os.path.basename(os.getcwd())
textures = []
for imgpath in glob.iglob("textures/**/*.png", recursive = True):
	textures.append(imgpath)
for texture in textures:
	imgobject = Image.open(texture)
	contents = [0] * (imgobject.size[0] * imgobject.size[1] * 2 + 4)
	contents[0], contents[1] = uint16to8(imgobject.size[0])
	contents[2], contents[3] = uint16to8(imgobject.size[1])
	i = 4
	imgpixels = imgobject.load()
	for y in range(imgobject.size[1]):
		for x in range(imgobject.size[0]):
			pxl = imgpixels[x, y]
			pxl = (pxl[0], pxl[1], pxl[2])
			pxl565 = rgb888to565(pxl)
			if pxl565 == rgb888to565(transparency_color) and pxl != transparency_color:
				pxl565 ^= 1
			contents[i], contents[i+1] = uint16to8(pxl565)
			i += 2
	filepath = "res/" + folder_name + "/" + texture[9:-4]
	if not os.path.exists(os.path.dirname(filepath)):
		os.makedirs(os.path.dirname(filepath))
	file = open(filepath, "wb")
	file.write(bytearray(contents))
	file.close()
