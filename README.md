stm32f_GBCart
=============

Game Boy Cartridge emulation from a stm32f4 Development Board

main.c: Main Program body.
> Initialization of the GPIOs.

stm32f4xx_it.c: Interrupt handlers.
> The interrupt handler for the rising flag trigger is defined here. It handles the read and write operations of the gameboy to the cartridge. 