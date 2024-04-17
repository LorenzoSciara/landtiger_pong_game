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

#define lWall 5
#define rWall 235
#define tWall 32
#define botton 286

volatile Ball b;
volatile int paddleLP1;
volatile int paddleLP2;
volatile int paddle2Dir;
volatile int changeP=0;
volatile int gameStart=0;
volatile int gamePause=0;
volatile int scoreP1=0;
volatile int scoreP2=0;

void game_init(int score_p1, int score_p2){
	int i, j;
	char scores[5];
	LCD_Clear(Black);
	//Wall 
	for(i=0; i<5; i++){
		LCD_DrawLine(i, 0, i, (yDim-1), Red);
		LCD_DrawLine((xDim-1)-i, 0, (xDim-1)-i, (yDim-1), Red);
	}
	//PaddleP1
	for(i=0; i<10; i++){
		LCD_DrawLine(5, (yDim-1)-(32-i), 48+5, (yDim-1)-(32-i), Green);
	}
	paddleLP1=5;
	//ScoreP1
	scoreP1=score_p1;
	sprintf(scores, "%d", scoreP1);
	GUI_Text(5, 160,(uint8_t *) scores, Black, White);
	
	//PaddleP2
	for(i=0; i<10; i++){
		LCD_DrawLine((((xDim-1)-48)-5), (32-i), ((xDim-1)-5), (32-i), Green);
	}
	paddleLP2=(((xDim-1)-48)-5);
	paddle2Dir=-1;
	//ScoreP2
	scoreP2=score_p2;
	sprintf(scores, "%d", scoreP2);
	GUI_Text(xDim-14, 160,(uint8_t *) scores, Black, White);
	
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
void changePadlle1Pos(int currPos){
	int i;
	static int lastPos;
	if(currPos>=5 && currPos+48<=(xDim-1)-5){
		if(paddleLP1!=currPos){
			if(lastPos==currPos/*lastPos+5>=currPos && currPos>=lastPos-5*/){
				changeP++;
			}
			lastPos=currPos;
		}
		if(changeP>0){
			if(currPos>paddleLP1){
				for(i=paddleLP1; i<currPos; i++){
					LCD_DrawLine(i, (yDim-1)-32, i, (yDim-1)-(32-9), Black);
					LCD_DrawLine(48+i, (yDim-1)-32, 48+i, (yDim-1)-(32-9), Green);
				}
			}
			else if(currPos<paddleLP1){
				for(i=paddleLP1; i>currPos; i--){
					LCD_DrawLine(i, (yDim-1)-32, i, (yDim-1)-(32-9), Green);
					LCD_DrawLine(48+i, (yDim-1)-32, 48+i, (yDim-1)-(32-9), Black);
				}
			}
			paddleLP1=currPos;
			changeP=0;
		}
	}
	return;
}
void game_start(void){
	int i, winner=0;
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
	else if((b.topy-1)==tWall){	//zona player 2
		if(b.rightx>=paddleLP2 && b.leftx<=(paddleLP2+48)){
			//sound
			disable_timer(0);
			reset_timer(0);
			init_timer(0,freqs[4]);
			enable_timer(0);
			//direction
			if(b.leftx>=paddleLP2+20 && b.rightx<=paddleLP2+26){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=-4;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=-4;
				}
			}
			else if((b.leftx>=paddleLP2+16 && b.rightx<=paddleLP2+22) || (b.leftx>=paddleLP2+24 && b.rightx<=paddleLP2+30)){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=-3;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=-3;
				}
			}
			else if((b.leftx>=paddleLP2+12 && b.rightx<paddleLP2+20) || (b.leftx>paddleLP2+26 && b.rightx<paddleLP2+34)){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=-2;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=-2;
				}
			}
			else if((b.leftx>=paddleLP2+6 && b.rightx<paddleLP2+16) || (b.leftx>paddleLP2+30 && b.rightx<=paddleLP2+40)){
				if(b.dirx>0){
				b.dirx=2;
				b.diry=-1;
				}
				else if(b.dirx<0){
				b.dirx=-2;
				b.diry=-1;
				}
			}
			else if((b.rightx>=paddleLP2 && b.rightx<paddleLP2+12) || (b.leftx>paddleLP2+34 && b.leftx<paddleLP2+48)){
				if(b.dirx>0){
				b.dirx=3;
				b.diry=-1;
				}
				else if(b.dirx<0){
				b.dirx=-3;
				b.diry=-1;
				}
			}
			else{
				if(b.dirx>0){
				b.dirx=2;
				b.diry=-2;
				}
				else if(b.dirx<0){
				b.dirx=-2;
				b.diry=-2;
				}
			}
		}
		else{
			if(scoreP1<4){
				pointP1();
			}
			else{
				scoreP1++;
				sprintf(scores, "%d", scoreP1);
				GUI_Text(xDim-14, 160,(uint8_t *) scores, Black, White);
				winner=1;
				game_end(winner);
			}
			return;
		}
	}
	else if(b.bottony==botton){//zona player 1
		if(b.rightx>=paddleLP1 && b.leftx<=(paddleLP1+48)){
			//sound
			disable_timer(0);
			reset_timer(0);
			init_timer(0,freqs[4]);
			enable_timer(0);
			//direction
			if(b.leftx>=paddleLP1+20 && b.rightx<=paddleLP1+26){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=4;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=4;
				}
			}
			else if((b.leftx>=paddleLP1+16 && b.rightx<=paddleLP1+22) || (b.leftx>=paddleLP1+24 && b.rightx<=paddleLP1+30)){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=3;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=3;
				}
			}
			else if((b.leftx>=paddleLP1+12 && b.rightx<paddleLP1+20) || (b.leftx>paddleLP1+26 && b.rightx<paddleLP1+34)){
				if(b.dirx>0){
				b.dirx=1;
				b.diry=2;
				}
				else if(b.dirx<0){
				b.dirx=-1;
				b.diry=2;
				}
			}
			else if((b.leftx>=paddleLP1+6 && b.rightx<paddleLP1+16) || (b.leftx>paddleLP1+30 && b.rightx<=paddleLP1+40)){
				if(b.dirx>0){
				b.dirx=2;
				b.diry=1;
				}
				else if(b.dirx<0){
				b.dirx=-2;
				b.diry=1;
				}
			}
			else if((b.rightx>=paddleLP1 && b.rightx<paddleLP1+12) || (b.leftx>paddleLP1+34 && b.leftx<paddleLP1+48)){
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
		}
		else{
			if(scoreP2<4){
				pointP2();
			}
			else{
				scoreP2++;
				sprintf(scores, "%d", scoreP2);
				GUI_Text(xDim-14, 160,(uint8_t *) scores, Black, White);
				winner=2;
				game_end(winner);
			}
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
	//Paddle2
	changePadlle2Pos();
	
	return;
}	

void game_end(int winner){
	disable_timer(1);
	disable_timer(2);
	if(winner==1){
		GUI_Text(80, 160+40,(uint8_t *) "YOU WIN :)", Black, White);
		GUI_Text(80, 160-40,(uint8_t *) "YOU LOSE :(", Black, White);
	}
	else{
		GUI_Text(80, 160-40,(uint8_t *) "YOU WIN :)", Black, White);
		GUI_Text(80, 160+40,(uint8_t *) "YOU LOSE :(", Black, White);
	}
		
	NVIC_EnableIRQ(EINT0_IRQn);
	return;
}


void game_pause(void){
	disable_timer(1);
	disable_timer(2);
	GUI_Text(100, 160,(uint8_t *) "||", Black, White);
	gamePause=1;
	return;
}

void game_resume(void){
	int i, j;
	for(i=0; i<20; i++){
		for(j=0; j<20; j++){
			LCD_SetPoint(100+i, 160+j, Black);
		}
	}
	enable_timer(1);
	enable_timer(2);
	gamePause=0;
}

void changePadlle2Pos(void){
	int i;
	for(i=0; i<20; i++){
		if(paddleLP2+49==rWall){
			paddle2Dir=-1;
		}
		else if(paddleLP2-1==lWall){
			paddle2Dir=1;
		}
		if(paddle2Dir>0){
			LCD_DrawLine(paddleLP2, 32, paddleLP2, (32-9), Black);
			LCD_DrawLine(48+paddleLP2+1, 32, 48+paddleLP2+1, (32-9), Green);
			paddleLP2++;
		}
		else if(paddle2Dir<0){
			LCD_DrawLine(paddleLP2, 32, paddleLP2, (32-9), Green);
			LCD_DrawLine(48+paddleLP2, 32, 48+paddleLP2, (32-9), Black);
			paddleLP2--;
		}
	}
	return;
}

void game_scoreUpdate(void){
	char scores[5];
	sprintf(scores, "%d", scoreP1);
	GUI_Text(5, 160,(uint8_t *) scores, Black, White);
	sprintf(scores, "%d", scoreP2);
	GUI_Text(xDim-14, 160,(uint8_t *) scores, Black, White);
	return;
}

void pointP1(void){
	int i, j;
	char scores[5];
	disable_timer(1);
	disable_timer(2);
	scoreP1++;
	sprintf(scores, "%d", scoreP1);
	GUI_Text(5, 160,(uint8_t *) scores, Black, White);
	for(i=b.leftx; i<=b.rightx; i++){
		for(j=b.topy; j<=b.bottony; j++){
			LCD_SetPoint(i, j, Black);
		}
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
	
	enable_timer(1);
	enable_timer(2);
	return;
}
void pointP2(void){
	int i, j;
	char scores[5];
	disable_timer(1);
	disable_timer(2);
	scoreP2++;
	sprintf(scores, "%d", scoreP2);
	GUI_Text(xDim-14, 160,(uint8_t *) scores, Black, White);
	for(i=b.leftx; i<=b.rightx; i++){
		for(j=b.topy; j<=b.bottony; j++){
			LCD_SetPoint(i, j, Black);
		}
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
	
	enable_timer(1);
	enable_timer(2);
	return;
}
