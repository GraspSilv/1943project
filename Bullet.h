// Bullet.h
#ifndef BULLET_H
#define BULLET_H

#include"SDL/SDL.h"
#include"GraphElement.h"


typedef enum {
	rect_blt_nrm,
	rect_blt_opn,
	rect_blt_rnd,
	rect_blt_clsd,
	rect_blt_beam
} bulletSpriteType;


class Bullet : public GraphElement{
	public:
		Bullet(double, double, double, double); //nondefault constructor, passes position to GraphElement constructor [xPos, yPos]
		SDL_Rect getSprite();

	private:
		bulletSpriteType sprite; //enum that stores player plane's sprite

};

#endif //BULLET_H
