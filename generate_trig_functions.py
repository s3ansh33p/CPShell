# generate_trig_functions.py by InterChan

# this script generates trig_functions.hpp
# the generated arrays contain 256 elements representing sine and cosine values, and it uses integer overflow in place of modulo to loop around
# sin127[] and cos127[] don't correspond to one degree every element but instead (1*256/360 = 1.40625), and the values are multiplied by 127
# use "DEGREE_TO_256" to convert degrees to the equivalent uint8_t measurement which goes to 256 instead of 360
# the functions "SIN" and "COS" take the angle in degrees and the hypotenuse, and output the value multiplied with the provided hypotenuse
# alternatively you can directly input a uint8_t angle with the functions "SIN256" and "COS256" which directly use the 256 measurement

import math

contents = ""

lines = open("generate_trig_functions.py", "r").readlines()
for i in range(6):
	if i == 0:
		contents += "// trig_functions.hpp by InterChan\n\n// this header file is generated from generate_trig_functions.py\n"
	else:
		contents += "//" + lines[i+2][1:]

contents += "\n#pragma once\n\nconst int8_t sin127[256] = {"
for i in range(256):
	contents += str(int(round(math.sin(math.radians(i * 45 / 32)) * 127)))
	if i < 255:
		contents += ", "
contents += "};\nconst int8_t cos127[256] = {"
for i in range(256):
	contents += str(int(round(math.cos(math.radians(i * 45 / 32)) * 127)))
	if i < 255:
		contents += ", "
contents += "};"

contents += """

#define DEGREE_TO_256(angle) ((uint8_t)(angle * 32 / 45))
#define SIN(angle, hyp) (((int)sin127[DEGREE_TO_256(angle)] * hyp) / 127)
#define COS(angle, hyp) (((int)cos127[DEGREE_TO_256(angle)] * hyp) / 127)
#define SIN256(angle256, hyp) (((int)sin127[angle256] * hyp) / 127)
#define COS256(angle256, hyp) (((int)cos127[angle256] * hyp) / 127)
"""
file = open("trig_functions.hpp", "w")
file.write(contents)
file.close()
