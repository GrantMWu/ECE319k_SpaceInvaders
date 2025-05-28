#include "enemy.h"
#include <stdbool.h>

sprite_t Enemies[NUM_ENEMIES];
sprite_t Enemy_Bomb[];

const unsigned short* SmallEnemyBitmaps[3] = {
	SmallEnemy10pointA,
	SmallEnemy20pointA,
	SmallEnemy30pointA
};

static int crit_R = 14;
static int crit_L = 10;
bool switch_dir = false;

/**************** Enemies_Init ***********************
 	Initialize enemies
 ******************************************************/
void Enemies_Init(void){
	int cols = 5;
	int rows = 3;
	
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			Sprite_Init(&Enemies[cols*i+j], 20 * j, 12 * (i + 3), -1, 0, SmallEnemyBitmaps[rows - i - 1], BlackEnemy, 16, 10);
		}
	}
}

// --------------------Switch_Dir----------------------------
// Check critical enemies and see if direction needs to be switched
// ----------------------------------------------------------
void Switch_Dir(void){
	// check for enemy reaching the edge 
	if ((Enemies[crit_R].x >= 112) || Enemies[crit_L].x <= 0){
		for(int i = 0; i < 15; i++){
			Enemies[i].y += 1;
			Enemies[i].vx *= -1;
		}
	}
}

// ---------------------Update_Crit_Enemies-------------------
// checks the critical enemies and updates the location of the critical enemy
// the critical enemy is the leftmost or rightmost enemy
// this critical enemy is used to determine if the Enemies have reached the screen edge
// -----------------------------------------------------------
void Update_Crit_Enemies(int *crit_R, int *crit_L){
	// find rightmost critical enemy
	while(Enemies[*crit_R].life == dead){
		if((*crit_R - 5) >= 0){
			*crit_R -= 5;
		}else{
			*crit_R += 9;
		}
	}
	
	// find leftmost critical enemy
	while(Enemies[*crit_L].life == dead){
		if((*crit_L - 5) >= 0){
			*crit_L -= 5;
		}else{
			*crit_L += 11;
		}
	}
}

//-----------------Enemies_AnyAlive--------------------
// check if any Enemies are alive 
// ------------------------------------------------
bool Enemies_AnyAlive(void){
	for(int i = 0; i < NUM_ENEMIES; i++){
		if(Enemies[i].life == alive){
			return 1;
		}
	}
	return 0;
	
}

//-----------------Enemies_Move--------------------
// Function to move any alive enemies 
// ------------------------------------------------
void Enemies_Move(void){
	
	// update ciritcal enemies and switch directions if needed
	Update_Crit_Enemies(&crit_R, &crit_L);
	Switch_Dir();
		
	// move each Enemy if alive
	for(int i = 0; i < NUM_ENEMIES; i++){
		if(Enemies[i].life == alive){
			Sprite_Move(&Enemies[i]);
		}
	}
}


// --------------------Calculate_Enemy_Points-------------------------
// Calculates the amount of points an Enemy is worth
// -----------------------------------------------------------------------
int Calculate_Enemy_Points(sprite_t* Enemy) {
	
	if(Enemy->image == SmallEnemy10pointA){
		return 10;
	}else if(Enemy->image == SmallEnemy20pointA){
		return 20;
	}else if(Enemy->image == SmallEnemy30pointA){
		return 30;
	}else{
		return 0;
	}
}

// --------------------Enemies_Check_Collisions-------------------------
// accepts a beam as an argument and checks each Enemy to see if beam has collided with enemy
// if a collision is found then the Enemy and beam life is dead and should break (only 1 enemy can die from a beam)
// -----------------------------------------------------------------------
int Enemies_Check_Collisions(void) {
	int dead_enemy_pts = 0;
	for (int i = 0; i < NUM_ENEMIES; i++) {
		if(Enemies[i].life == alive && Sprite_Overlap(&Enemies[i], &Beam) && Beam.life == alive) {
			Enemies[i].life = dead;
			dead_enemy_pts = Calculate_Enemy_Points(&Enemies[i]);
			Beam.life = dead;		
		}
		if(Enemies[i].life == alive && Sprite_Overlap(&Enemies[i], &Player)){
			Player.life = dead;
			break;
		}
	}
	
	return dead_enemy_pts;
}

