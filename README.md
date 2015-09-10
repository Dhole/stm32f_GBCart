stm32f_GBCart
=============

Game Boy Cartridge emulation from a stm32f4 Development Board

I wrote about this project in my blog:

- https://dhole.github.io/post/gameboy_cartridge_emu_1/
- https://dhole.github.io/post/gameboy_cartridge_emu_2/
- https://dhole.github.io/post/gameboy_custom_logo/

## Description

This project implements the emulation of a gameboy cartridge using a stm32f4
Development Board. A real gameboy can be connected to a stm32f4 running this
software and load real roms as well as homebrew roms.

- ROM Only and MBC1 Cartridges are implemented. See a full list at:
	- http://www.devrs.com/gb/files/gbmbcsiz.txt
- RAM emulation implemented. The contents will be erased upon powering off the stm32f4
- Custom boot logo implemented. It will be shown only during the first boot.

## ROMs

In order to use a rom in the code, the file must be converted into a C array:
```
cp Tetris.gb rom.gb
xxd -i rom.gb | sed 's/unsigned/unsigned const/g' > tetris_rom.h
rm rom.gb
```

## Files

main.c: Main Program body.
> Initialization of the GPIOs.

stm32f4xx_it.c: Interrupt handlers.
> The interrupt handler for the rising flag trigger is defined here. It handles 
> the read and write operations of the gameboy to the cartridge.

### Custom logo

draw_logo.py: Draws a logo on a window
> Requieres pygame

make_logo.py: Converts a png logo image into a binary file to be used as a boot
logo
> Requieres pygame
```
./make_logo.py dhole_logo2.png dhole_logo2.bin
cp dhole_logo2.bin logo.bin
xxd -i logo.bin > dhole2_logo.h
rm logo.bin
```

## Issues

If some games don't run, or you get crashes at some points, it may be a timming
issue (the timmings are very tight, and I believe they may need to be different
on different hardware revisions). You can try [goebish's](https://github.com/goebish/stm32f_GBCart/commit/f5200854f8541e80b23790774c8a3aefe13a9adb) branch which modifies
the number of NOPs in the code.
