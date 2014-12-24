#! /usr/bin/python2

from __future__ import print_function
import sys, pygame, binascii

pygame.init()

size_x = 96/2
size_y = 16/2

def usage():
	print("Usage:", sys.argv[0], "boot_logo.png boot_logo.bin")

def binarify(b):
	if b == 0:
		return '1'
	else:
		return '0'
###

if len(sys.argv) < 3:
	usage()
	sys.exit(1)

logo_img = pygame.image.load(sys.argv[1])
logo_img = logo_img.convert(8)
logo = pygame.surfarray.pixels2d(logo_img)

for y in range(0, size_y):
	for x in range(0, size_x):
		if logo[x,y] == 0:
			print("*", sep="", end="")
		else:
			print(" ", sep="", end="")
	print()

logo_s = ['0'] * size_x * size_y

x = 0
y = 0
for i in range(0, 24):
	x = (i * 4) % size_x
	y = ((i * 4) / size_x) * 4
	for j in range(0, 16):
		logo_s[i*16+j] = binarify(logo[x + (j%4), y + (j/4)])

#print(logo_s)

with open(sys.argv[2], 'wb') as file_logo_bin:
	for i in range(0, len(logo_s) / 8):
		file_logo_bin.write(chr(int(''.join(logo_s[i*8:i*8+8]), 2)))
	
