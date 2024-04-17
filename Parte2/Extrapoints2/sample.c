/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Lorenzo Sciara
** Modified date:           07/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "Led/led.h"
#include "button_EXINT/button.h"
#include "RIT/RIT.h"
#include "ADC/adc.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int main(void)
{
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	LCD_Initialization();
	//LCD_Clear(White);
	GUI_Text(0, 160, (uint8_t *) "Press the KEY1 button to start the game.", Black, White);
	BUTTON_init();
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       */
	init_timer(1,0x1312D0); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	init_timer(2,0xBEBC20); 						/*500ms 0xBEBC20*/
	ADC_init();
	NVIC_DisableIRQ(EINT2_IRQn);
	NVIC_DisableIRQ(EINT0_IRQn);
	//enable_RIT();
	 
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);
	LPC_PINCON->PINSEL1 |= (1<<21);				//Sound
	LPC_PINCON->PINSEL1 &= ~(1<<20);			//Sound
	LPC_GPIO0->FIODIR |= (1<<26);
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }
	
	
	//TP_Init();
	//TouchPanel_Calibrate();
	//GUI_Text(0, 280, (uint8_t *) " touch here : 1 sec to clear  ", Blue, White);
	
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	//init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	//enable_timer(0);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
