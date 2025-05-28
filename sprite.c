// sprite.c
#include "sprite.h"

void Sprite_Init(sprite_t* s, int32_t x, int32_t y, int32_t vx, int32_t vy, 
                 const unsigned short* image, const unsigned short* black, uint32_t w, uint32_t h) {
    s->x = x;
    s->y = y;
    s->vx = vx;
    s->vy = vy;
    s->image = image;
    s->black = black;
    s->life = alive;
    s->w = w;
    s->h = h;
    s->needDraw = 1;
}
								 
void Sprite_Move(sprite_t* s){
	if (s->life == alive) {
		s->x += s->vx;
		s->y += s->vy;
		s->needDraw = 1;
	}
}

int32_t Sprite_Overlap(sprite_t* s1, sprite_t* s2){
	return(
		s1->x <= s2->x + s2->w - 3 &&
		s1->x + s1->w - 3 >= s2->x &&
		s1->y >= s2->y - s2->h + 2 &&
		s1->y - s1->h + 2 <= s2->y
	);
}
