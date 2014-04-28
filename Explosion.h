/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Explosion.h
	Interface of Explosion class (coordinate appropriate display of explosions)
*/
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include"SDL/SDL.h"
#include"GraphElement.h"

typedef enum {
	SPR_EXPL_1,
	SPR_EXPL_2,
	SPR_EXPL_3,
	SPR_EXPL_4,
	SPR_EXPL_5,
	SPR_EXPL_6
} explosionSpriteType;

class Explosion : public GraphElement {
	public:
		Explosion(double, double); //nondefault constructor, passes position and type to GraphElement constructor [xPos, yPos]
		SDL_Rect getSprite();
		int isDone();
	private:
		int displayCount;
		static const int framesPerSprite;
		explosionSpriteType sprite; //enum that stores explosion's sprite
};

#endif //EXPLOSION_H
