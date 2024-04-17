/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT and joystick buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "adc/adc.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	ADC_init();
	
	init_timer(1,0x002625A0);							/* allegro 10Hz - 4beats in 0.4 sec */
	enable_timer(1);											/* T*F=k -> F/F'=k -> 25MHz/10Hz=0x2625A0 */
	
	init_timer(3,0xB2D05E00);							/* watchdog set to 2 minute 				*/
	enable_timer(3);											/* T*F=k -> 25MHz*30=0xB2D05E00				*/
	
	LCD_Initialization();
	LCD_Clear(Blue);
	
	TP_Init();
	
	GUI_Text(45, 20, "CAs PoliTO 2018-19", Yellow, Blue);
	GUI_Text(60, 40, "LANDTIGER board ", White, Blue);
	GUI_Text(48, 60, "2 mins minimal check", White, Blue);
	
	GUI_Text(10, 100, "1) Speaker emits 2 tones", White, Blue);
	GUI_Text(10, 120, "   ? have you heard ?", White, Blue);
	GUI_Text(10, 140, "2) Potentiometer", White, Blue);
	GUI_Text(10, 160, "   -> operate min <-> max", White, Blue);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
	
	
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
