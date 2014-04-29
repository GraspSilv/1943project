/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Bullet.cpp
	Implementation of Bullet class (coordinate movement and sprites of player and enemy bullets)
History
	04/23/14	Jack Magiera	Create, define nondefault constructor and getSprite()
	04/27/14	Jack Magiera	Add type declaration to nondefault constructor, define deconstructor and getCount(), add count to nondefault constructor, add cout message to deconstructor
*/
#include"Bullet.h"
#include<vector>
#include<iostream>
#include"SDL/SDL.h"
#include"GraphElement.h"

int Bullet::count = 0;

Bullet::Bullet(double xP, double yP, double xV, double yV, int o) :
	GraphElement(xP, yP, BULLET) {
	sprite = SPR_BLT_NRM;
	origin = o;
	// 1 for player, 0 for enemy

	SDL_Rect rect_blt_nrm; //sprite of normal bullet
	rect_blt_nrm.x = 13;
	rect_blt_nrm.y = 220;
	rect_blt_nrm.w = 2;
	rect_blt_nrm.h = 14;

	SDL_Rect rect_blt_opn; //sprite of open circle bullet
	rect_blt_opn.x = 40;
	rect_blt_opn.y = 223;
	rect_blt_opn.w = 8;
	rect_blt_opn.h = 8;

	SDL_Rect rect_blt_rnd; //sprite of rounded bullet
	rect_blt_rnd.x = 61;
	rect_blt_rnd.y = 220;
	rect_blt_rnd.w = 3;
	rect_blt_rnd.h = 16;

	SDL_Rect rect_blt_clsd; //sprite of closed circle bullet
	rect_blt_clsd.x = 85;
	rect_blt_clsd.y = 225;
	rect_blt_clsd.w = 6;
	rect_blt_clsd.h = 6;

	SDL_Rect rect_blt_beam; //sprite of beam bullet
	rect_blt_beam.x = 98;
	rect_blt_beam.y = 200;
	rect_blt_beam.w = 5;
	rect_blt_beam.h = 44;
	
	//add sprites to sprites vector
	addSprite(rect_blt_nrm);
	addSprite(rect_blt_opn);
	addSprite(rect_blt_rnd); 
	addSprite(rect_blt_clsd);
	addSprite(rect_blt_beam);

	setXVel(xV);
	setYVel(yV);

	if (origin) count++;
}

SDL_Rect Bullet::getSprite() {
	return sprites[sprite];
}

int Bullet::getOrigin() {
	return origin;
}

int Bullet::update() { //returns 1 if bullet if off the screen
	if(isOnScreen()) {
		return 0;
	}
	return 1;
}
