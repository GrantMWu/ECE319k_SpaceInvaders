// images.h
// contains image buffers for simple space invaders
// Jonathan Valvano, March 5, 2018
// Capture image dimensions from BMP files

#ifndef __image_h
#define __image_h
#include <stdint.h>

extern const unsigned short BlackEnemy[];
extern const unsigned short SmallEnemy10pointA[];




// enemy ship that starts at the top of the screen (arms/mouth open)
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// includes one blacked out row on the top and bottom sides of the image to prevent smearing when moved 1 pixels to the up or down
// width=16 x height=10
extern const unsigned short SmallEnemy10pointB[];

// enemy ship that starts in the middle of the screen (arms together)
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// includes one blacked out row on the top and bottom sides of the image to prevent smearing when moved 1 pixels to the up or down
// width=16 x height=10
extern const unsigned short SmallEnemy20pointA[];

// enemy ship that starts in the middle of the screen (arms apart)
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// includes one blacked out row on the top and bottom sides of the image to prevent smearing when moved 1 pixels to the up or down
// width=16 x height=10
extern const unsigned short SmallEnemy20pointB[];


// enemy ship that starts at the bottom of the screen (arms down)
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// includes one blacked out row on the top and bottom sides of the image to prevent smearing when moved 1 pixels to the up or down
// width=16 x height=10
extern const unsigned short SmallEnemy30pointA[];

// enemy ship that starts at the bottom of the screen (arms up)
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// includes one blacked out row on the top and bottom sides of the image to prevent smearing when moved 1 pixels to the up or down
// width=16 x height=10
extern const unsigned short SmallEnemy30pointB[];

// image of the player's ship
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
extern const unsigned short PlayerShip0[];
// image of the player's ship, some damage
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
extern const unsigned short PlayerShip1[];
// image of the player's ship, more damage
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
extern const unsigned short PlayerShip2[];
// image of the player's ship, lots of damage
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
extern const unsigned short PlayerShip3[];

// small shield floating in space to cover the player's ship from enemy fire (undamaged)
// width=18 x height=5
extern const unsigned short Bunker0[];

extern const unsigned short PlayerBeam[];

extern const unsigned short BlackBeam[];

extern const unsigned short Heart[];

extern const unsigned short Bomb[];


#endif /* __image_h */
