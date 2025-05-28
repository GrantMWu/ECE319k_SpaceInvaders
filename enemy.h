// enemy.h

#ifndef ENEMY_H
#define ENEMY_H
#include <stdbool.h>
#include "sprite.h"
#include "images.h"
#include "player.h"

#define NUM_ENEMIES 15

extern sprite_t Enemies[NUM_ENEMIES];
extern sprite_t Enemy_Bomb[];

bool Enemies_AnyAlive(void);
void Switch_Dir(void);
void Update_Crit_Enemies(int *crit_R, int *crit_L);
void Enemies_Init(void);
void Enemies_Move(void);
int Enemies_Check_Collisions();

#endif