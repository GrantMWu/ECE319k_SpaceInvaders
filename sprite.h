// sprite.h

#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include "images.h"

typedef enum {dead,alive} status_t;

// setup sprite struct
struct sprite{
	int32_t x; 		// x coordinate
	int32_t y; 		// y coordinate
	int32_t vx, vy;		// pixels/30Hz
	const unsigned short *image;  //ptr->image
	const unsigned short *black;  
	status_t life;		// dead/alive
	uint32_t w; 	//width
	uint32_t h; 	//height
	uint32_t needDraw; 		// true if need to draw
};

typedef struct sprite sprite_t;

// Functions
void Sprite_Init(
	sprite_t* s, 
	int32_t x, 
	int32_t y, 
	int32_t vx, 
	int32_t vy, 
	const unsigned short* image, 
	const unsigned short* black,
	uint32_t w,
	uint32_t h
);
	
void Sprite_Move(sprite_t* s);
int32_t Sprite_Overlap(sprite_t* s1, sprite_t* s2);
void Sprite_Draw(sprite_t* s, const uint16_t *image);


#endif