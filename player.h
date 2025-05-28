// player.h

#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "sprite.h"
#include "images.h"

extern sprite_t Player;
extern int Player_old_pos;
extern sprite_t Beam;

void Player_Init(void);
void Player_Move(uint32_t pos);
void Player_Shoot(void);
void Beam_Move(void);

#endif