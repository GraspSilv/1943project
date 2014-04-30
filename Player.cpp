/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Player.cpp
	Implementation of Player class (coordinate movement and sprites of player's plane)
*/
#include"Player.h"
#include<vector>
#include"SDL/SDL.h"
#include"GraphElement.h"
#include"Counter.h"

Player::Player(double xP, double yP) :
	GraphElement(xP, yP, PLAYER),
	ammo(100, (640 - 30), 100, 0, 200, 1),
	health(180, (640 - 30), 100, 0, 100, 5) { //nondefault constructor, passes position to GraphElement constructor [xPos, yPos]
	sprite = SPR_PLYR_HLTHY_STRAIGHT;
	weapon = 0;
	cycles = 200;
	xMom = 0;
	yMom = 0;
	hasBomb = 0;

	SDL_Rect rect_plyr_hlthy_fRollLeft; //sprite of healthy player full roll left
		rect_plyr_hlthy_fRollLeft.x = 15;
		rect_plyr_hlthy_fRollLeft.y = 28;
		rect_plyr_hlthy_fRollLeft.w = 16;
		rect_plyr_hlthy_fRollLeft.h = 16;
	
	SDL_Rect rect_plyr_hlthy_hRollLeft; //sprite of healthy player half roll left
		rect_plyr_hlthy_hRollLeft.x = 38;
		rect_plyr_hlthy_hRollLeft.y = 28;
		rect_plyr_hlthy_hRollLeft.w = 20;
		rect_plyr_hlthy_hRollLeft.h = 16;

	SDL_Rect rect_plyr_hlthy_straight; //sprite of healthy player straight
		rect_plyr_hlthy_straight.x = 65;
		rect_plyr_hlthy_straight.y = 28;
		rect_plyr_hlthy_straight.w = 24;
		rect_plyr_hlthy_straight.h = 16;
	
	SDL_Rect rect_plyr_hlthy_hRollRight; //sprite of healthy player half roll right
		rect_plyr_hlthy_hRollRight.x = 96;
		rect_plyr_hlthy_hRollRight.y = 28;
		rect_plyr_hlthy_hRollRight.w = 20;
		rect_plyr_hlthy_hRollRight.h = 16;
	
	SDL_Rect rect_plyr_hlthy_fRollRight; //sprite of healthy player full roll right
		rect_plyr_hlthy_fRollRight.x = 123;
		rect_plyr_hlthy_fRollRight.y = 28;
		rect_plyr_hlthy_fRollRight.w = 16;
		rect_plyr_hlthy_fRollRight.h = 16;
	
	SDL_Rect rect_plyr_dmgd_fRollLeft; //sprite of damaged player full roll left
		rect_plyr_dmgd_fRollLeft.x = 14;
		rect_plyr_dmgd_fRollLeft.y = 82;
		rect_plyr_dmgd_fRollLeft.w = 16;
		rect_plyr_dmgd_fRollLeft.h = 16;
	
	SDL_Rect rect_plyr_dmgd_hRollLeft; //sprite of damaged player half roll left
		rect_plyr_dmgd_hRollLeft.x = 37;
		rect_plyr_dmgd_hRollLeft.y = 82;
		rect_plyr_dmgd_hRollLeft.w = 20;
		rect_plyr_dmgd_hRollLeft.h = 16;

	SDL_Rect rect_plyr_dmgd_straight; //sprite of damaged player straight
		rect_plyr_dmgd_straight.x = 64;
		rect_plyr_dmgd_straight.y = 82;
		rect_plyr_dmgd_straight.w = 24;
		rect_plyr_dmgd_straight.h = 16;

	SDL_Rect rect_plyr_dmgd_hRollRight; //sprite of damaged player half roll right
		rect_plyr_dmgd_hRollRight.x = 95;
		rect_plyr_dmgd_hRollRight.y = 82;
		rect_plyr_dmgd_hRollRight.w = 20;
		rect_plyr_dmgd_hRollRight.h = 16;

	SDL_Rect rect_plyr_dmgd_fRollRight; //sprite of damaged player full roll right
		rect_plyr_dmgd_fRollRight.x = 122;
		rect_plyr_dmgd_fRollRight.y = 82;
		rect_plyr_dmgd_fRollRight.w = 16;
		rect_plyr_dmgd_fRollRight.h = 16;
	
	SDL_Rect rect_plyr_intr_1; //sprite of stage 1/7 of player intro sequence
		rect_plyr_intr_1.x = 12;
		rect_plyr_intr_1.y = 141;
		rect_plyr_intr_1.w = 13;
		rect_plyr_intr_1.h = 8;

	SDL_Rect rect_plyr_intr_2; //sprite of stage 2/7 of player intro sequence
		rect_plyr_intr_2.x = 39;
		rect_plyr_intr_2.y = 141;
		rect_plyr_intr_2.w = 21;
		rect_plyr_intr_2.h = 13;

	SDL_Rect rect_plyr_intr_3; //sprite of stage 3/7 of player intro sequence
		rect_plyr_intr_3.x = 76;
		rect_plyr_intr_3.y = 138;
		rect_plyr_intr_3.w = 28;
		rect_plyr_intr_3.h = 7;

	SDL_Rect rect_plyr_intr_4; //sprite of stage 4/7 of player intro sequence
		rect_plyr_intr_4.x = 119;
		rect_plyr_intr_4.y = 139;
		rect_plyr_intr_4.w = 30;
		rect_plyr_intr_4.h = 6;

	SDL_Rect rect_plyr_intr_5; //sprite of stage 5/7 of player intro sequence
		rect_plyr_intr_5.x = 163;
		rect_plyr_intr_5.y = 134;
		rect_plyr_intr_5.w = 30;
		rect_plyr_intr_5.h = 16;

	SDL_Rect rect_plyr_intr_6; //sprite of stage 6/7 of player intro sequence
		rect_plyr_intr_6.x = 205;
		rect_plyr_intr_6.y = 132;
		rect_plyr_intr_6.w = 28;
		rect_plyr_intr_6.h = 16;

	SDL_Rect rect_plyr_intr_7; //sprite of stage 7/7 of player intro sequence
		rect_plyr_intr_7.x = 241;
		rect_plyr_intr_7.y = 130;
		rect_plyr_intr_7.w = 24;
		rect_plyr_intr_7.h = 15;
	
	//add sprites to sprites vector
	addSprite(rect_plyr_hlthy_fRollLeft);
	addSprite(rect_plyr_hlthy_hRollLeft);
	addSprite(rect_plyr_hlthy_straight);
	addSprite(rect_plyr_hlthy_hRollRight);
	addSprite(rect_plyr_hlthy_fRollRight);
	addSprite(rect_plyr_dmgd_fRollLeft);
	addSprite(rect_plyr_dmgd_hRollLeft);
	addSprite(rect_plyr_dmgd_straight);
	addSprite(rect_plyr_dmgd_hRollRight);
	addSprite(rect_plyr_dmgd_fRollRight);
	addSprite(rect_plyr_intr_1);
	addSprite(rect_plyr_intr_2);
	addSprite(rect_plyr_intr_3);
	addSprite(rect_plyr_intr_4);
	addSprite(rect_plyr_intr_5);
	addSprite(rect_plyr_intr_6);
	addSprite(rect_plyr_intr_7);
}

SDL_Rect Player::getSprite() {
	if(getXVel() < 0) { //if plane is moving left,
		if (getXMom() < -60){
			sprite = SPR_PLYR_HLTHY_FROLLLEFT;
		} else {
		sprite = SPR_PLYR_HLTHY_HROLLLEFT;
		}
	} else if(getXVel() > 0) { //if plane is moving right,
		if (getXMom() > 60) {
			sprite = SPR_PLYR_HLTHY_FROLLRIGHT;
		} else {
		sprite = SPR_PLYR_HLTHY_HROLLRIGHT;
		}
	} else {
		sprite = SPR_PLYR_HLTHY_STRAIGHT;
	}
	
	return sprites[sprite];
}

Counter Player::getAmmoCntr() {
	return ammo;
}

Counter Player::getHealthCntr() {
	return health;
}

void Player::newAmmo() {
	ammo.increment(100);
}

void Player::useAmmo(int n) {
	ammo.increment(-n);
}

void Player::newHealth() {
	health.increment(4);
}

void Player::hitByBullet() {
	health.increment(-2);
}

void Player::hitByPlane() {
	health.increment(-3);
}

int Player::getWeapon() {
	return weapon;
}

void Player::setWeapon(int w) {
	if (w == 2){
		setBomb(1);
		w = 0;
	}
	weapon = w;
}

int Player::getCycles() {
	if (cycles > 0) return --cycles;
	return 0;
}

void Player::setCycles(int c) {
	if(c > 0) {
		cycles = c;
	} else {
		cycles = 0;
	}
}

int Player::getXMom(){
	return xMom;
}

int Player::getYMom(){
	return yMom;
}

int Player::getBomb(){
	return hasBomb;
}

void Player::setXMom(int xM){
	xMom = xM;
}

void Player::setBomb(int b){
	hasBomb = b;
}

void Player::setYMom(int yM){
	yMom = yM;
}
