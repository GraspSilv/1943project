/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Powerup.cpp
	Implementation of Powerup class (coordinate movement, type, and sprite of powerup)
History
	04/22/14 Jon Richelsen	Create, copying everything from player
*/
#include"Powerup.h"
#include<vector>
#include"SDL/SDL.h"
#include"GraphElement.h"

Powerup::Powerup(double xP, double yP, powerType p) : GraphElement(xP, yP) { //nondefault constructor, passes position to GraphElement constructor [xPos, yPos, power]
	xVel = 0;
	yVel = 0;
	power = p;
	
	SDL_Rect rect_pwrup_pow; //sprite of POW powerup
		rect_pwrup_pow.x = 10;
		rect_pwrup_pow.y = 327;
		rect_pwrup_pow.w = 16;
		rect_pwrup_pow.h = 16;

	SDL_Rect rect_pwrup_sprd; //sprite of spread shot powerup
		rect_pwrup_sprd.x = 33;
		rect_pwrup_sprd.y = 330;
		rect_pwrup_sprd.w = 16;
		rect_pwrup_sprd.h = 10;

	SDL_Rect rect_pwrup_mssl; //sprite of missile powerup
		rect_pwrup_mssl.x = 58;
		rect_pwrup_mssl.y = 328;
		rect_pwrup_mssl.w = 14;
		rect_pwrup_mssl.h = 14;

	SDL_Rect rect_pwrup_beam; //sprite of beam powerup
		rect_pwrup_beam.x = 79;
		rect_pwrup_beam.y = 327;
		rect_pwrup_beam.w = 16;
		rect_pwrup_beam.h = 16;

	SDL_Rect rect_pwrup_auto; //sprite of automatic fire powerup
		rect_pwrup_auto.x = 102;
		rect_pwrup_auto.y = 327;
		rect_pwrup_auto.w = 16;
		rect_pwrup_auto.h = 16;

	SDL_Rect rect_pwrup_cow; //sprite of cow powerup
		rect_pwrup_cow.x = 126;
		rect_pwrup_cow.y = 329;
		rect_pwrup_cow.w = 16;
		rect_pwrup_cow.h = 13;
	
	//add sprites to sprites vector
	addSprite(rect_pwrup_pow);
	addSprite(rect_pwrup_sprd);
	addSprite(rect_pwrup_mssl);
	addSprite(rect_pwrup_beam);
	addSprite(rect_pwrup_auto);
	addSprite(rect_pwrup_cow);
}

SDL_Rect Powerup::getSprite() {
	return sprites[power];
}

void Powerup::setXVel(double xV) {
	xVel = xV;
}

void Powerup::setYVel(double yV) {
	yVel = yV;
}

double Powerup::getXVel() {
	return xVel;
}

double Powerup::getYVel() {
	return yVel;
}
