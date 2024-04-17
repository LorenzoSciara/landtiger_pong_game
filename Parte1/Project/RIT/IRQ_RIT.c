/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../Game/game.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int downk2=0;
volatile int downk1=0;
volatile int downk0=0;
extern int gameStart;
extern int gamePause;

void RIT_IRQHandler (void)
{
	if(downk0!=0){
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	
			downk0++;		
			switch(downk0){
				case 2:						
					game_init();
					break;
				default:
					break;
			}
		}
		else{	
			downk0=0;
			disable_RIT();
			reset_RIT();
			NVIC_EnableIRQ(EINT0_IRQn);							 
			LPC_PINCON->PINSEL4    |= (1 << 20);     
		}
	}
	
	/* button management */
	if(downk1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			downk1++;				
			switch(downk1){
				case 2:						/* pay attention here: please see slides to understand value 2 */
					if(gameStart==0 && gamePause==0){
					game_init();
					enable_timer(1);
					NVIC_EnableIRQ(EINT2_IRQn);
					}
					else if(gameStart!=0 && gamePause==0){
						enable_timer(1);
					}
					else if(gameStart!=0 && gamePause!=0){
						game_resume();
					}
					break;
				default:
					break;
			}
		}
		else{	/* button released */
			downk1=0;
			disable_RIT();
			reset_RIT();
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(downk2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			downk2++;				
			switch(downk2){
				case 2:						/* pay attention here: please see slides to understand value 2 */
					if(gamePause!=1){
						game_pause();
					}
					break;
				default:
					break;
			}
		}
		else{	/* button released */
			downk2=0;
			disable_RIT();
			reset_RIT();
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	/* ADC management */
	//if(start!=0)
	//ADC_start_conversion();		
			
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
