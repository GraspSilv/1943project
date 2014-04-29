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
#include<iostream>
#include<vector>
#include"SDL/SDL.h"
#include"GraphElement.h"

Bullet::Bullet(double xP, double yP, double xV, double yV, int o, int bT) : GraphElement(xP, yP, xV, yV, BULLET) { //nondefault constructor, passes position and velocity to GraphElement constructor and sets origin [xPos, yPos, xVel, yVel, origin. bulletType]
	switch(bT) {
		case 0:
			sprite = SPR_BLT_NRM;
			break;
		case 1:
			sprite = SPR_BLT_OPN;
			break;
		case 2:
			sprite = SPR_BLT_RND;
			break;
		case 3:
			sprite = SPR_BLT_CLSD;
			break;
		case 4:
			sprite = SPR_BLT_BEAM;
			break;
		default:
			std::cout << "Error: Invalid bulletType pased to Bullet constructor" << std::endl;
			sprite = SPR_BLT_NRM;
			break;
	}
	//sprite = SPR_BLT_NRM;
	origin = o;
	bulletType = bT;

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
}

SDL_Rect Bullet::getSprite() {
	return sprites[sprite];
}

int Bullet::update() { //returns 1 if bullet if off the screen
	if(isOnScreen()) {
		return 0;
	}
	return 1;
}

int Bullet::getOrigin() {
	return origin;
}

int Bullet::getBulletType() {
	return bulletType;
}

int Bullet::isBeam(){
	return bulletType == 4;
}
