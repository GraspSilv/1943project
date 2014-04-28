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
	rect_blt_nrm,
	rect_blt_opn,
	rect_blt_rnd,
	rect_blt_clsd,
	rect_blt_beam
} bulletSpriteType;


class Bullet : public GraphElement{
	public:
		Bullet(double, double, double, double); //nondefault constructor, passes position to GraphElement constructor [xPos, yPos]
		~Bullet();
		SDL_Rect getSprite();
		int getCount();
		static int count;

	private:
		bulletSpriteType sprite; //enum that stores player plane's sprite
		

};

#endif //BULLET_H
