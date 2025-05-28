// SpaceInvaders.c
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the ECE319K Lab 10

// Last Modified: 1/2/2023 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0


#include <stdint.h>
#include <stdbool.h>
#include <locale.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "../inc/ADC.h"
#include "../inc/wave.h"
#include "Timer0.h"
#include "Timer1.h"
#include "sprite.h"
#include "delay.h"
#include "enemy.h"
#include "player.h"
#include "images.h"

// Languages and Phrases
typedef enum {English, Spanish} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE, INTRO} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Intro_English[]="The year is 3070.\nAliens have waged\nwar against earth\nand you are\nthe last hope.\n\nKill all the\nenemies and protect\nthe world from\ndestruction!";
const char Intro_Spanish[]="A\xA4o 3070.\nLos extraterrestres\nhan declarado la\nguerra a la Tierra\ny t\x96 eres la \x96ltima\n esperanza.\n\n\xAD Acaba con todos\nlos enemigos\ny protege al mundo\nde la destrucci\x95n!";
const char Game_Lost_English[] = "You Lost!";
const char Game_Lost_Spanish[] = "\xADPerdiste!";
const char Game_Won_English[] = "You Won!";
const char Game_Won_Spanish[] = "\xADGanaste!";
const char Points_English[] = "Points:";
const char Points_Spanish[] = "Puntos:";
const char *Phrases[4][2]={
  {Hello_English,Hello_Spanish},
  {Goodbye_English,Goodbye_Spanish},
  {Language_English,Language_Spanish},
	{Intro_English, Intro_Spanish}
};

/*PROTOTYPES*/
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void GameInit(void);
void GameMove(void);
void GameDraw(void);
void PortE_Init(void);
void PortF_Init(void);
void display_title(void);
void display_intro(void);
void switch_lang(void);
void buttons_game_init(uint32_t);

/*Global*/
int GameFlag;
bool GameLost = false, GameWon = false;
uint32_t Player_Pts = 0, update_pts;

/*main*/
int maingame(void){
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
	SysTick_Init();
  Random_Init(1);
	PortF_Init();
	PortE_Init();
	Wave_Init();
	buttons_game_init(1);
	ADC_Init();
  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
	GameInit();
	
	Timer1_Init(80000000/30, 3);
	Timer0_Init(40000000, 2);
  EnableInterrupts();
	
	while(1){
		while(!GameLost && !GameWon){
			while (GameFlag == 0){};
			GameFlag = 0;
			GameDraw();
		}
		
		if(GameLost){
			ST7735_SetCursor(7, 7);
			ST7735_OutString(Game_Lost_English);
		}else if(GameWon){
			ST7735_SetCursor(7, 7);
			ST7735_OutString(Game_Won_English);
		}
	};
}

int main(void){ 
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
	SysTick_Init();
  Output_Init();
	PortE_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
	int i = 0;
  while(1){
		Delay100ms(1);
		display_title();
		
		// wait for user selection
		while((GPIO_PORTE_DATA_R & 0x10) == 0){		// wait for PE4 signal to select language
			if((GPIO_PORTE_DATA_R & 0x20) == 0x20){		// PE5 to switch languages
				switch_lang();
				SysTick_Wait(30);
				while((GPIO_PORTE_DATA_R & 0x20) == 0x20);
			}
		}
		
		// Display Intro in appropriate language and wait for continue
		ST7735_FillScreen(0x0000);
		display_intro();
		SysTick_Wait(50000);
		ST7735_FillScreen(0x0000);
		
		
  }  
}

/************************* Helper Functions *************************/

// ******* GAME FUNCTIONS ********
void GameInit(void){
	// Draw score
	ST7735_SetCursor(0, 0);
	ST7735_OutString(Points_English);
	ST7735_OutUDec(Player_Pts);
	
	ST7735_SetCursor(9, 0);
	ST7735_DrawBitmap(90, 8, Heart, 10, 8);
	
	
	Player_Init();
	Enemies_Init();
}

void GameMove(void){
	// Move player
	uint32_t adc_val = ADC_In();
	Player_Move(adc_val);
	
	// Move beam
	Beam_Move();
	
	// Move Enemies
	update_pts = Enemies_Check_Collisions();
	if(update_pts){
		Player_Pts += update_pts;
	}
	
	if(!Enemies_AnyAlive()){
		GameWon = true;
	}else if(Player.life == dead){
		GameLost = true;
	}else{
		Enemies_Move();
	}
}

void GameDraw(void){
	int i;
	
	// Draw score
	if(update_pts){	
		ST7735_SetCursor(0, 0);
		ST7735_OutString(Points_English);
		ST7735_OutUDec(Player_Pts);
	}
	
	// Draw Player
	if(Player.needDraw){
		ST7735_DrawBitmap(Player_old_pos, Player.y, Player.black, Player.w, Player.h);
		ST7735_DrawBitmap(Player.x, Player.y, Player.image, Player.w, Player.h);
		Player.needDraw = 0;
	}
	
	// Draw Beam
	if(Beam.needDraw){
		ST7735_DrawBitmap(Beam.x, Beam.y - Beam.vy, Beam.black, Beam.w, Beam.h);
		if(Beam.life == alive){
			ST7735_DrawBitmap(Beam.x, Beam.y, Beam.image, Beam.w, Beam.h);
		} else{
			ST7735_DrawBitmap(Beam.x, Beam.y, Beam.black, Beam.w, Beam.h);
			Beam.needDraw = 0;
		}
		
	}
	
	// Draw Enemies
	for(i=0; i < 15; i++){
		if(Enemies[i].needDraw){
			if(Enemies[i].life == alive){
				ST7735_DrawBitmap(Enemies[i].x, Enemies[i].y, Enemies[i].image, Enemies[i].w, Enemies[i].h);
			}else{
				ST7735_DrawBitmap(Enemies[i].x, Enemies[i].y, Enemies[i].black, Enemies[i].w, Enemies[i].h);
				Enemies[i].needDraw = 0;
			}
		} 
	}
}

// ***************** Timer1A_Handler ****************
// Timer used to move game one step and draw
// Inputs:  none
// Outputs: none
void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
  GameMove();
	GPIO_PORTF_DATA_R ^= 0x02;
	GameFlag = 1;
}

// ***************** Timer0A_Handler ****************
// Timer used to drop a random enemy bomb
// Inputs:  none
// Outputs: none
void Timer0A_Handler(void){ // can be used to perform tasks in background
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
  if (Enemy_Bomb.life == alive){
		
	}
}


void PortE_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_PRGPIO_R & 0x10) == 0) {} // Wait until Port E is ready
	GPIO_PORTE_DEN_R |= 0x30;				// enable PE4 and PE5
	GPIO_PORTE_DIR_R |= 0x00;			// as inputs
}

void buttons_game_init(uint32_t priority){
	GPIO_PORTE_IS_R &= ~0x10;			// PE4 set on edge
	GPIO_PORTE_IBE_R &= ~0x10;
	GPIO_PORTE_IEV_R &= ~0x10;
	GPIO_PORTE_ICR_R = 0x10;
	GPIO_PORTE_IM_R |= 0x10;			// arm interrupt on PE4
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFF00FF)|(priority<<5); // bits 5:7 set priority of PORTE. I want this higher priority than the timer
	NVIC_EN0_R = 0x10;					// enable interrupt 4 for PORTE
}

void GPIOPortE_Handler(void){
	GPIO_PORTE_ICR_R = 0x10;		// acknowledge flag
	
	// check to see if there is a beam and initiate one if not
	if(Beam.life == dead){
		Player_Shoot();
		Wave_Shoot();
	}
}

void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	while((SYSCTL_PRGPIO_R & 0x20) != 0x20) {} // Wait until Port F is ready
	GPIO_PORTF_DEN_R |= 0x02;				// enable PF1
	GPIO_PORTF_DIR_R |= 0x02;			// as output
}




// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}

void display_title(void){
	myLanguage = English;
	// initialize start screen, default to english
	ST7735_SetCursor(5, 7);
	ST7735_OutChar(175);
	ST7735_OutChar(' ');
	ST7735_OutString((char *)Phrases[LANGUAGE][English]);
	ST7735_OutChar(10);
	ST7735_OutChar(13);
	ST7735_SetCursor(7, 9);
	ST7735_OutString((char *)Phrases[LANGUAGE][Spanish]);
}

void display_intro(void){
	ST7735_SetCursor(0,0);
	ST7735_OutString((char *)Phrases[INTRO][myLanguage]);
}

void switch_lang(void){
	switch(myLanguage){
		case English:
			ST7735_SetCursor(5,7);
			ST7735_OutChar(' ');
			ST7735_SetCursor(5,9);
			ST7735_OutChar(175);
			myLanguage = Spanish;
			break;
		case Spanish:
			ST7735_SetCursor(5,7);
			ST7735_OutChar(175);
			ST7735_SetCursor(5,9);
			ST7735_OutChar(' ');
			myLanguage = English;
			break;
	}
}

