/**
  ******************************************************************************
  * @author  Eduard S.
  * @version V1.0.0
  * @date    23-December-2014
  * @brief   Interrupt handlers.
  *          The interrupt handler for the rising flag trigger is defined here.
  *          It handles the read and write operations of the gameboy to the
  *          cartridge.  
  ******************************************************************************
  */ 

#include "stm32f4xx_it.h"
#include "stm32f4_discovery.h"
#include "repeat.h"

#include "roms/tetris_rom.h"
//#include "roms/drmario_rom.h"
//#include "roms/jml_rom.h"
//#include "roms/zelda_rom.h"
//#include "roms/fubu_rom.h"
//#include "roms/dmgp_rom.h"
//#include "roms/zelda_f_rom.h"
//#include "roms/20y_rom.h"
//#include "roms/gejmboj_rom.h"
//#include "roms/oh_rom.h"
//#include "roms/mcmrder_rom.h"
//#include "roms/cpu_test_rom.h"
//#include "roms/gemgem_rom.h"
//#include "roms/organizer_rom.h"
//#include "roms/organizer_sav.h"
#include "dhole2_logo.h"


/* 
 * Macros to relate the GPIO and the functionality
 */
#define BUS_RD (GPIOC->IDR & 0x0002)
#define BUS_WR (GPIOC->IDR & 0x0004)
#define ADDR_IN GPIOD->IDR
#define DATA_IN GPIOE->IDR
#define DATA_OUT GPIOE->ODR
#define SET_DATA_MODE_IN GPIOE->MODER = 0x00000000;
#define SET_DATA_MODE_OUT GPIOE->MODER = 0x55550000;


/* Defines wether to show the Nintendo logo or the custom logo */
uint8_t no_show_logo;

uint8_t rom_bank; 
uint8_t ram_bank;
uint8_t ram_enable;
uint8_t rom_ram_mode;

uint8_t ram[0x8000]; // 32K


/* Write cartridge operation for MBC1 */
inline void mbc1_write(uint16_t addr, uint8_t data) {
	if (addr >= 0xA000 && addr < 0xC000) {
 		/* 8KB RAM Bank 00-03, if any */
		//ram[addr - 0xA000 + 0x2000 * ram_bank] = data;
	}
	/*if (addr < 0x2000) {
		if (data) {
			ram_enable = 1;
		} else {
			ram_enable = 0;
		}
	}*/ else if (addr >= 0x2000 && addr < 0x4000) {
		/* ROM Bank Number */
		data &= 0x1F;
		rom_bank = (rom_bank & 0xE0) | data;
		if (data == 0x00) {
			rom_bank |= 0x01;
		}
	} else if (addr < 0x6000) {
		/*RAM Bank Number - or - Upper Bits of ROM Bank Number */
		if (rom_ram_mode) {
			/* ROM mode */
			data &= 0x07;
			rom_bank = (rom_bank & 0x1F) | (data << 5);
		} else {
			/* RAM mode */
			ram_bank = data & 0x03;
		}
	} else if (addr < 0x8000) {
		/* ROM/RAM Mode Select */
		if (data) { 
			/* Emable RAM Banking mode */
			rom_ram_mode = 0;
		} else { 
			/* Emable ROM Banking mode */
			rom_ram_mode = 1;
		}
	}
}

/* Read cartridge operation for MBC1 */
inline uint8_t mbc1_read(uint16_t addr) {
	if (addr < 0x4000) {
		/* 16KB ROM bank 00 */
		if (no_show_logo) {
			/* Custom logo disabled */
			return rom_gb[addr];
		} else {
			/* Custom logo enabled, only during first read at boot */
			if (addr == 0x133) {
				no_show_logo = 1;
			}
			return logo_bin[addr - 0x104];
		}
	} else if (addr < 0x8000) {
		/* 16KB ROM Bank 01-7F */
		return rom_gb[addr + 0x4000 * (rom_bank - 1)];
	} else if (addr >= 0xA000 && addr < 0xC000) {
		/* 8KB RAM Bank 00-03, if any */
		return ram[addr - 0xA000 + 0x2000 * ram_bank];
	}
	return 0x00;
}

/* Handle PC0 interrupt (rising edge of the gameboy CLK) */
void EXTI0_IRQHandler(void) {
	uint16_t addr;
	uint8_t data;
	
	uint32_t enablestatus;
	enablestatus =  EXTI->IMR & EXTI_Line0;
	
	if (((EXTI->PR & EXTI_Line0) != (uint32_t)RESET) &&
	    (enablestatus != (uint32_t)RESET)) {
		/* Do stuff on trigger */

		/* Wait 10 NOPs, until the ADDR is ready in the bus */
		REP(1,0,asm("NOP"););
		
		/* Read ADDR from the bus */
		addr = ADDR_IN;
		
		if (BUS_RD || !BUS_WR) {
			/* Write operation */
			
			/* Wait 30 NOPs, until the DATA is ready in the bus */
			REP(3,0,asm("NOP"););
			
			/* Read DATA from the bus */
			data = DATA_IN >> 8;
			
			/* Write data to cartridge at addr */
			mbc1_write(addr, data);
		} else {
			/* Read operation */
			
			/* Set the GPIOE in output mode */
			SET_DATA_MODE_OUT;
			/* Output the data read at addr through GPIOE */
			DATA_OUT = ((uint16_t)mbc1_read(addr)) << 8;
			/* Wait 14 NOPs, until the gameboy has read the DATA
			 * in the bus */
			REP(1,4,asm("NOP"););
			/* Set the GPIOE back to input mode */
			SET_DATA_MODE_IN;
		}
	}
	/* Clear interrupt flag */
	EXTI->PR = EXTI_Line0;
	//EXTI_ClearITPendingBit(EXTI_Line0);
}