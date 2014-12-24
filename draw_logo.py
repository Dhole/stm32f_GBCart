#! /usr/bin/python2

import sys, pygame, binascii

pygame.init()

size_x = 96/2
size_y = 16/2

scale = 8

WHITE = (255,255,255)
GREY = (80,80,80)

# byte packing function
def pack_rgb(c):
	result = c[0] << 16 | c[1] << 8 | c[2]
	return result

def colorize(b):
	if b == '0':
		return WHITE
	if b == '1':
		return GREY
	else:
		print "Error at colorize"

def usage():
	print "Usage:", sys.argv[0], "boot_logo.bin"

###

if len(sys.argv) < 2:
	usage()
	sys.exit(1)
	
with open(sys.argv[1], 'rb') as file_logo_bin:
	logo_bin = file_logo_bin.read()

if len(logo_bin) != 0x30:
	print "Wrong logo size:", len(logo_bin)
	sys.exit(2)

logo_surf = pygame.Surface((size_x, size_y))
logo_surf.fill(WHITE)
logo = pygame.surfarray.pixels2d(logo_surf)

logo_s = bin(int(binascii.hexlify(logo_bin), base=16))[2:]

x = 0
y = 0
for i in range(0, 24):
	x = (i * 4) % size_x
	y = ((i * 4) / size_x) * 4
	for j in range(0, 16):
		logo[x + (j%4), y + (j/4)] = pack_rgb(colorize(logo_s[i*16+j]))

window = pygame.display.set_mode((size_x*scale, size_y*scale))
pygame.display.set_caption("Boot logo")

window.blit(pygame.transform.scale(logo_surf, (size_x * scale, size_y * scale)), (0,0))

pygame.display.flip()

#input handling (somewhat boilerplate code):
while True: 
	for event in pygame.event.get(): 
		if event.type == pygame.QUIT: 
			sys.exit(0) 