/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Bullet.h
	Interface of Bullet class (coordinate movement and sprites of player and enemy bullets)
History
	04/23/14	Jack Magiera	Create, define bulletSpriteType enum, declare nondefault constructor, getSprite(), and private variable sprite
	04/27/14	Jack Magiera	Declare deconstructor, getCount(), and public variable count
To Do
*/
#ifndef BULLET_H
#define BULLET_H

#include"SDL/SDL.h"
#include"GraphElement.h"

typedef enum {
	SPR_BLT_NRM,
	SPR_BLT_OPN,
	SPR_BLT_RND,
	SPR_BLT_CLSD,
	SPR_BLT_BEAM
} bulletSpriteType;

typedef enum {
	PLAYERO,
	ENEMYO
} bulletOriginType;

class Bullet : public GraphElement {
	public:
		Bullet(double, double, double, double, int); //nondefault constructor, passes position and velocity to GraphElement constructor and sets origin [xPos, yPos, xVel, yVel, origin]
		~Bullet();
		SDL_Rect getSprite();
		int getOrigin();
		int getCount();
	private:
		bulletSpriteType sprite; //enum that stores player bullet's sprite
		int origin; //enum that store's bullet's origin
		static int count;
};

#endif //BULLET_H
