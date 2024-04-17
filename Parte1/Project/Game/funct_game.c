#include "../GLCD/GLCD.h" 
#include "game.h"
#include "../timer/timer.h"
#include "../button_EXINT/button.h"
#include <stdlib.h>
#include <stdio.h>

const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5
*/

#define lWall 4
#define rWall 235
#define tWall 4
#define botton 286

volatile Ball b;
volatile int paddleLP;
volatile int changeP=0;
volatile int gameStart=0;
volatile int gamePause=0;
volatile int score=0;
volatile int record=100;

void game_init(void){
	int i, j;
	char records[5];
	LCD_Clear(Black);
	//Wall 
	for(i=0; i<5; i++){
		LCD_DrawLine(i, 0, i, (yDim-1)-(32+1), Red);
		LCD_DrawLine((xDim-1)-i, 0, (xDim-1)-i, (yDim-1)-(32+1), Red);
		LCD_DrawLine(0, i, (xDim-1), i, Red);
	}
	//Paddle
	for(i=0; i<10; i++){
		LCD_DrawLine(0, (yDim-1)-(32-i), 48, (yDim-1)-(32-i), Green);
	}
	paddleLP=0;
	//Score
	GUI_Text(5, 160,(uint8_t *) "0", Black, White);
	score=0;
	if(gameStart==0){
		GUI_Text((xDim-1)-30, 5, (uint8_t *) "100", Black, White);
		record=100;
	}
	else{
		sprintf(records, "%d", record);
		GUI_Text((xDim-1)-30, 5, (uint8_t *) records, Black, White);
	}
	//Ball
	for(i=0; i<5; i++){
		for(j=0; j<5; j++){
			LCD_SetPoint(((xDim-1)-5)-i, 160-j, Green);
		}
	}
	b.rightx=(xDim-1)-5;
	b.leftx=((xDim-1)-5)-4;
	b.topy=160-4;
	b.bottony=160;
	b.dirx=-1;
	b.diry=-1;
	gameStart=1;
}
void changePadllePos(int currPos){
	int i;
	if(paddleLP!=currPos){
		changeP++;
	}
	if(changeP>20){
		if(currPos>paddleLP){
			for(i=paddleLP; i<currPos; i++){
				LCD_DrawLine(i, (yDim-1)-32, i, (yDim-1)-(32-9), Black);
				LCD_DrawLine(48+i, (yDim-1)-32, 48+i, (yDim-1)-(32-9), Green);
			}
		}
		else if(currPos<paddleLP){
			for(i=paddleLP; i>currPos; i--){
				LCD_DrawLine(i, (yDim-1)-32, i, (yDim-1)-(32-9), Green);
				LCD_DrawLine(48+i, (yDim-1)-32, 48+i, (yDim-1)-(32-9), Black);
			}
		}
		paddleLP=currPos;
		changeP=0;
	}
}
void game_start(void){
	int i;
	char scores[5];
	if((b.leftx-1)==lWall){	//muro di sinistra->rimbalza con angolo opposto rispetto alle y
		//sound
		disable_timer(0);
		reset_timer(0);
		init_timer(0,freqs[3]);
		enable_timer(0);
		//direction
		b.dirx=abs(b.dirx);
		b.diry=b.diry;	
	}
	else if((b.rightx+1)==rWall){	////muro di destra->rimbalza con angolo opposto rispetto alle y
		//sound
		disable_timer(0);
		reset_timer(0);
		init_timer(0,freqs[3]);
		enable_timer(0);
		//direction
		b.dirx=-abs(b.dirx);
		b.diry=b.diry;
	}
	else if((b.topy-1)==tWall){	//muro alto->rimbalza con angolo opposto rispetto alle x
		//sound
		disable_timer(0);
		reset_timer(0);
		init_timer(0,freqs[3]);
		enable_timer(0);
		//direction
		b.diry=-abs(b.diry);
		b.dirx=b.dirx;
	}
	else if(b.bottony==botton){
		if(b.leftx>=paddleLP && b.rightx<=(paddleLP+48)){
			//sound
			disable_timer(0);
			reset_timer(0);
			init_timer(0,freqs[4]);
			enable_timer(0);
			//direction
			if(b.leftx>=paddleLP+20 && b.rightx<=paddleLP+26){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=4;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=4;
				}
			}
			else if((b.leftx>=paddleLP+16 && b.rightx<=paddleLP+22) || (b.leftx>=paddleLP+24 && b.rightx<=paddleLP+30)){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=3;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=3;
				}
			}
			else if((b.leftx>=paddleLP+12 && b.rightx<paddleLP+20) || (b.leftx>paddleLP+26 && b.rightx<paddleLP+34)){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=2;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=2;
				}
			}
			else if((b.leftx>=paddleLP+6 && b.rightx<paddleLP+16) || (b.leftx>paddleLP+30 && b.rightx<=paddleLP+40)){
				if(b.dirx>0){
				b.dirx=2;
				b.diry=1;
				}
				else if(b.dirx<0){
				b.dirx=-2;
				b.diry=1;
				}
			}
			else if((b.leftx>=paddleLP && b.rightx<paddleLP+12) || (b.leftx>paddleLP+34 && b.rightx<paddleLP+48)){
				if(b.dirx>0){
				b.dirx=3;
				b.diry=1;
				}
				else if(b.dirx<0){
				b.dirx=-3;
				b.diry=1;
				}
			}
			else{
				if(b.dirx>0){
				b.dirx=2;
				b.diry=2;
				}
				else if(b.dirx<0){
				b.dirx=-2;
				b.diry=2;
				}
			}
			if(score<100){
				score=score+5;
			}
			else{
				score=score+10;
			}
			sprintf(scores, "%d", score);
			GUI_Text(5, 160,(uint8_t *) scores, Black, White);
			if(score>record){
				record=score;
				sprintf(scores, "%d", record);
				GUI_Text((xDim-1)-30, 5, (uint8_t *) scores, Black, White);
			}
		}
		else{
			game_end();
			return;
		}
	}
	//se la pallina si trova in situazione normale
	if(b.dirx>0){	//se la direzione delle x è positiva
		for(i=0; i<b.dirx; i++){
			if((b.rightx+1)!=rWall){
				LCD_DrawLine(b.leftx, b.topy, b.leftx, b.bottony, Black);
				b.leftx++;
				b.rightx++;
				LCD_DrawLine(b.rightx, b.topy, b.rightx, b.bottony, Green);
			}
		}
	}
	else if(b.dirx<0){	//se la direzione delle x è negativa
		for(i=0; i>b.dirx; i--){
			if((b.leftx-1)!=lWall){
				b.leftx--;
				LCD_DrawLine(b.leftx, b.topy, b.leftx, b.bottony, Green);
				LCD_DrawLine(b.rightx, b.topy, b.rightx, b.bottony, Black);
				b.rightx--;
			}
		}
	}
	if(b.diry>0){	//se la direzione delle y è positiva
		for(i=0; i<b.diry; i++){
			if((b.topy-1)!=tWall){
				b.topy--;
				LCD_DrawLine(b.leftx, b.topy, b.rightx, b.topy, Green);
				LCD_DrawLine(b.leftx, b.bottony, b.rightx, b.bottony, Black);
				b.bottony--;
			}
		}
	}
	if(b.diry<0){	//se la direzione delle y è negativa
		for(i=0; i>b.diry; i--){	//NB per y negative ho direzione negativa->i è negativa (ma i pixel crescono)
			if((b.bottony)!=botton){
				LCD_DrawLine(b.leftx, b.topy, b.rightx, b.topy, Black);
				b.topy++;
				b.bottony++;
				LCD_DrawLine(b.leftx, b.bottony, b.rightx, b.bottony, Green);
			}
		}
	}
	/*sprintf(scores, "%d", score);
	GUI_Text(5, 160,(uint8_t *) scores, Black, White);
	sprintf(scores, "%d", record);
	GUI_Text((xDim-1)-30, 5, (uint8_t *) scores, Black, White);*/
	return;
}	

void game_end(void){
	disable_timer(1);
	GUI_Text(80, 160,(uint8_t *) "GAME OVER", Black, White);
	NVIC_EnableIRQ(EINT0_IRQn);
	return;
}


void game_pause(void){
	disable_timer(1);
	GUI_Text(80, 160,(uint8_t *) "||", Black, White);
	gamePause=1;
	return;
}

void game_resume(void){
	int i, j;
	for(i=0; i<20; i++){
		for(j=0; j<20; j++){
			LCD_SetPoint(80+i, 160+j, Black);
		}
	}
	enable_timer(1);
	gamePause=0;
}