# include "player.h"

sprite_t Player;
sprite_t Beam;
int Player_old_pos;

/**************** Convert ***********************
 Convert an ADC input into a x position on lcd screen
 Input: ADC reading
 Output: x position on ST7735 LCD Screen
 ************************************************/
uint32_t Convert(uint32_t x){
  return 93*x/4096+16;
}


/**************** Player_Init ***********************
 Initialize the player sprite
 Input: None
 Output: Player sprite is initialized
 ************************************************/
void Player_Init(void){
	Sprite_Init(&Player, 55, 159, 1, 0, PlayerShip0, BlackEnemy, 18, 8);
	Sprite_Init(&Beam, 0, 0, 0, -2, PlayerBeam, BlackBeam, 2, 5);
	Beam.life = dead;
}

/**************** Player_Move ***********************
 Move the player
 Input: adc value to convert to position
 Output: Player position is updated
 ************************************************/
void Player_Move(uint32_t adc_val){
	uint32_t pos = Convert(adc_val);
	if (Player.x != pos){
		Player_old_pos = Player.x;
		Player.x = pos;
		Player.needDraw = 1;
	}
}

/**************** Player_Shoot ***********************
 Initiate Beam
 Input: None
 Output: Beam is initialized
 ************************************************/
void Player_Shoot(void){
	Beam.x = (Player.x + Player.w / 2) - 1;
	Beam.y = Player.y - Player.h;
	Beam.needDraw = 1;
	Beam.life = alive;
}

/**************** Beam_Move ***********************
 Move the Beam
 Input: none
 Output: Beam position is updated
 ************************************************/
void Beam_Move(void){
	// check if beam reached end
	if (Beam.y < 0){
		Beam.life = dead;
		Beam.needDraw = 0;
	}
	
	// Move Beam if still alive
	if(Beam.life == alive){
		Beam.y += Beam.vy;
	}
}

