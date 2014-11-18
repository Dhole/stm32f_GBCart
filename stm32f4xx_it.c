/**
  ******************************************************************************
  * @file    TIM_TimeBase/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4_discovery.h"

//#include "tetris_rom.h"
//#include "drmario_rom.h"
//#include "jml_rom.h"
//#include "zelda_rom.h"
//#include "fubu_rom.h"
#include "dmgp_rom.h"
//#include "zelda_f_rom.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t rom_bank;
uint8_t ram_bank;
uint8_t ram_enable;
uint8_t rom_ram_mode;

uint8_t ram[0x8000]; // 32K
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{}

/******************************************************************************/
/*            STM32F4xx Peripherals Interrupt Handlers                        */
/******************************************************************************/

/* Set interrupt handlers */
/* Handle PD0 interrupt */
void EXTI0_IRQHandler(void) {
	volatile uint16_t addr;
	//uint16_t addr;
	volatile uint8_t data;
	//if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
	uint32_t enablestatus;
	enablestatus =  EXTI->IMR & EXTI_Line0;
	if (((EXTI->PR & EXTI_Line0) != (uint32_t)RESET) &&
	    (enablestatus != (uint32_t)RESET)) {
		/* Do stuff on trigger */

		asm("NOP");asm("NOP");asm("NOP");asm("NOP");
		asm("NOP");asm("NOP");asm("NOP");asm("NOP");

		addr = GPIOD->IDR;
		if ((GPIOC->IDR & 0x0002) || !(GPIOC->IDR & 0x0004)) {
			//GPIOE->MODER = 0x55550000;
			//GPIOE->ODR = 0xff00;
			//goto clear;
			data = GPIOE->IDR;
			if (addr < 0x2000) {
				ram_enable = 1;
			} else if (addr < 0x4000) {
				if (data & 0x1F != 0){
					asm("NOP");
				}
				data |= 0xE0;
				rom_bank &= data;
				if (data == 0xE0) {
					rom_bank |= 0x01;
				}
				//rom_bank = data;
			} else if (addr < 0x6000) {
				if (rom_ram_mode) {
					// ROM mode
					rom_bank |= (data << 5) & 0x60;
				} else {
					// RAM mode
					ram_bank = data & 0x03;
				}
			}
		} else {
			//addr = (addr & 0x00ff) << 8 | (addr & 0xff00) >> 8;
			GPIOE->MODER = 0x55550000;
			if (addr < 0x4000) {
				GPIOE->ODR = ((uint16_t)rom_gb[addr]) << 8;
				asm("NOP");asm("NOP");asm("NOP");asm("NOP");
				asm("NOP");asm("NOP");asm("NOP");asm("NOP");
			} else if (addr < 0x8000) {
				
				GPIOE->ODR = ((uint16_t)rom_gb[addr + 0x4000 * (rom_bank - 1)]) << 8;
			} else if (addr >= 0xA000 && addr < 0xC000) {
				GPIOE->ODR = ((uint16_t)ram[addr - 0xA000 + 0x2000 * ram_bank]) << 8;
			}
			
			
			asm("NOP");asm("NOP");asm("NOP");asm("NOP");
			asm("NOP");asm("NOP");asm("NOP");asm("NOP");
			// until here for ROM ONLY
//asm("NOP");asm("NOP");asm("NOP");asm("NOP");
			//asm("NOP");asm("NOP");asm("NOP");asm("NOP");
			//asm("NOP");asm("NOP");asm("NOP");asm("NOP");
			//asm("NOP");asm("NOP");asm("NOP");asm("NOP");
			GPIOE->MODER = 0x00000000;
		}
		//GPIOA->ODR = 0X0001;
		//asm("NOP");asm("NOP");
		//GPIOA->ODR = 0X0000;
	}
clear:  
	/* Clear interrupt flag */
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
