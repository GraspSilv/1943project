/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Enemy.cpp
	Implementation of Enemy class (base class for derived enemy classes)
History
	03/31/14	Bill Gowans		Create
	04/16/14	Jon Richelsen	Standardize
	04/27/14	Jack Magiera	Define nondefault constructor, getSprite(), and getEnemyType()
*/
#include"Enemy.h"
#include <stdlib.h>
#include <iostream>
#include<vector>
#include"SDL/SDL.h"
#include"GraphElement.h"
#include <cmath>

int Enemy::count = 0;

Enemy::Enemy(double xP, double yP, enemyType e) : GraphElement(xP,yP,ENEMY) {

	AIcount = 0;
	enemy = e;
	count++;

	SDL_Rect rect_red_N; //sprite of red plane heading north
	rect_red_N.x = 45;
	rect_red_N.y = 373;
	rect_red_N.w = 15;
	rect_red_N.h = 15;

	SDL_Rect rect_red_NE; //sprite of red plane heading northeast
	rect_red_NE.x = 69;
	rect_red_NE.y = 381;
	rect_red_NE.w = 15;
	rect_red_NE.h = 15;

	SDL_Rect rect_red_E; //sprite of red plane heading east
	rect_red_E.x = 75;
	rect_red_E.y = 405;
	rect_red_E.w = 15;
	rect_red_E.h = 15;

	SDL_Rect rect_red_SE; //sprite of red plane heading southeast
	rect_red_SE.x = 69;
	rect_red_SE.y = 429;
	rect_red_SE.w = 15;
	rect_red_SE.h = 15;

	SDL_Rect rect_red_S; //sprite of red plane heading south
	rect_red_S.x = 45;
	rect_red_S.y = 437;
	rect_red_S.w = 15;
	rect_red_S.h = 15;

	SDL_Rect rect_red_SW; //sprite of red plane heading southwest
	rect_red_SW.x = 21;
	rect_red_SW.y = 429;
	rect_red_SW.w = 15;
	rect_red_SW.h = 15;

	SDL_Rect rect_red_W; //sprite of red plane heading west
	rect_red_W.x = 15;
	rect_red_W.y = 405;
	rect_red_W.w = 15;
	rect_red_W.h = 15;

	SDL_Rect rect_red_NW; //sprite of red plane heading northwest
	rect_red_NW.x = 21;
	rect_red_NW.y = 381;
	rect_red_NW.w = 15;
	rect_red_NW.h = 15;

	SDL_Rect rect_lilGray_N; //sprite of little gray plane heading north
	rect_lilGray_N.x = 134;
	rect_lilGray_N.y = 382;
	rect_lilGray_N.w = 11;
	rect_lilGray_N.h = 15;

	SDL_Rect rect_lilGray_NE; //sprite of little gray plane heading northeast
	rect_lilGray_NE.x = 150;
	rect_lilGray_NE.y = 389;
	rect_lilGray_NE.w = 13;
	rect_lilGray_NE.h = 13;

	SDL_Rect rect_lilGray_E; //sprite of little gray plane heading east
	rect_lilGray_E.x = 155;
	rect_lilGray_E.y = 407;
	rect_lilGray_E.w = 15;
	rect_lilGray_E.h = 11;

	SDL_Rect rect_lilGray_SE; //sprite of little gray plane heading southeast
	rect_lilGray_SE.x = 150;
	rect_lilGray_SE.y = 423;
	rect_lilGray_SE.w = 13;
	rect_lilGray_SE.h = 13;

	SDL_Rect rect_lilGray_S; //sprite of little gray plane heading south
	rect_lilGray_S.x = 134;
	rect_lilGray_S.y = 428;
	rect_lilGray_S.w = 11;
	rect_lilGray_S.h = 15;

	SDL_Rect rect_lilGray_SW; //sprite of little gray plane heading southwest
	rect_lilGray_SW.x = 116;
	rect_lilGray_SW.y = 423;
	rect_lilGray_SW.w = 13;
	rect_lilGray_SW.h = 13;

	SDL_Rect rect_lilGray_W; //sprite of little gray plane heading west
	rect_lilGray_W.x = 109;
	rect_lilGray_W.y = 407;
	rect_lilGray_W.w = 15;
	rect_lilGray_W.h = 11;

	SDL_Rect rect_lilGray_NW; //sprite of little gray plane heading northwest
	rect_lilGray_NW.x = 116;
	rect_lilGray_NW.y = 389;
	rect_lilGray_NW.w = 13;
	rect_lilGray_NW.h = 13;

	SDL_Rect rect_singGreen_straight; //sprite of single-engine green plane straight
	rect_singGreen_straight.x = 193;
	rect_singGreen_straight.y = 276;
	rect_singGreen_straight.w = 15;
	rect_singGreen_straight.h = 15;

	SDL_Rect rect_singGreen_hVert; //sprite of single-engine green plane halfway vertical
	rect_singGreen_hVert.x = 217;
	rect_singGreen_hVert.y = 278;
	rect_singGreen_hVert.w = 15;
	rect_singGreen_hVert.h = 12;

	SDL_Rect rect_singGreen_fVert; //sprite of single-engine green plane vertical
	rect_singGreen_fVert.x = 241;
	rect_singGreen_fVert.y = 282;
	rect_singGreen_fVert.w = 15;
	rect_singGreen_fVert.h = 6;

	SDL_Rect rect_singGreen_updown; //sprite of single-engine green plane upside-down
	rect_singGreen_updown.x = 263;
	rect_singGreen_updown.y = 278;
	rect_singGreen_updown.w = 15;
	rect_singGreen_updown.h = 15;

	SDL_Rect rect_doubGreen_straight; //sprite of double-engine green plane straight
	rect_doubGreen_straight.x = 192;
	rect_doubGreen_straight.y = 300;
	rect_doubGreen_straight.w = 15;
	rect_doubGreen_straight.h = 15;

	SDL_Rect rect_doubGreen_roll1; //sprite of double-engine green plane in stage 1/7 of barrel roll
	rect_doubGreen_roll1.x = 217;
	rect_doubGreen_roll1.y = 300;
	rect_doubGreen_roll1.w = 8;
	rect_doubGreen_roll1.h = 15;

	SDL_Rect rect_doubGreen_roll2; //sprite of double-engine green plane in stage 2/7 of barrel roll
	rect_doubGreen_roll2.x = 238;
	rect_doubGreen_roll2.y = 300;
	rect_doubGreen_roll2.w = 8;
	rect_doubGreen_roll2.h = 15;

	SDL_Rect rect_doubGreen_roll3; //sprite of double-engine green plane in stage 3/7 of barrel roll
	rect_doubGreen_roll3.x = 257;
	rect_doubGreen_roll3.y = 300;
	rect_doubGreen_roll3.w = 9;
	rect_doubGreen_roll3.h = 16;

	SDL_Rect rect_doubGreen_roll4; //sprite of double-engine green plane in stage 4/7 of barrel roll
	rect_doubGreen_roll4.x = 275;
	rect_doubGreen_roll4.y = 300;
	rect_doubGreen_roll4.w = 15;
	rect_doubGreen_roll4.h = 16;

	SDL_Rect rect_doubGreen_roll5; //sprite of double-engine green plane in stage 5/7 of barrel roll
	rect_doubGreen_roll5.x = 297;
	rect_doubGreen_roll5.y = 300;
	rect_doubGreen_roll5.w = 9;
	rect_doubGreen_roll5.h = 16;

	SDL_Rect rect_doubGreen_roll6; //sprite of double-engine green plane in stage 6/7 of barrel roll
	rect_doubGreen_roll6.x = 317;
	rect_doubGreen_roll6.y = 300;
	rect_doubGreen_roll6.w = 8;
	rect_doubGreen_roll6.h = 15;

	SDL_Rect rect_doubGreen_roll7; //sprite of double-engine green plane in stage 7/7 of barrel roll
	rect_doubGreen_roll7.x = 338;
	rect_doubGreen_roll7.y = 300;
	rect_doubGreen_roll7.w = 8;
	rect_doubGreen_roll7.h = 15;

	SDL_Rect rect_purp4; //sprite of purple plane 1 (largest of 4)
	rect_purp4.x = 192;
	rect_purp4.y = 324;
	rect_purp4.w = 30;
	rect_purp4.h = 28;

	SDL_Rect rect_purp3; //sprite of purple plane 2 (second largest of 4)
	rect_purp3.x = 236;
	rect_purp3.y = 328;
	rect_purp3.w = 24;
	rect_purp3.h = 21;

	SDL_Rect rect_purp2; //sprite of purple plane 3 (second smallest of 4)
	rect_purp2.x = 272;
	rect_purp2.y = 329;
	rect_purp2.w = 16;
	rect_purp2.h = 16;

	SDL_Rect rect_purp1; //sprite of purple plane 4 (smallest of 4)
	rect_purp1.x = 298;
	rect_purp1.y = 331;
	rect_purp1.w = 16;
	rect_purp1.h = 13;

	SDL_Rect rect_bigGray; //sprite of big gray plane
	rect_bigGray.x = 191;
	rect_bigGray.y = 359;
	rect_bigGray.w = 32;
	rect_bigGray.h = 30;

	addSprite(rect_red_N);
	addSprite(rect_red_NE);
	addSprite(rect_red_E);
	addSprite(rect_red_SE);
	addSprite(rect_red_S);
	addSprite(rect_red_SW);
	addSprite(rect_red_W);
	addSprite(rect_red_NW);
	addSprite(rect_lilGray_N);
	addSprite(rect_lilGray_NE);
	addSprite(rect_lilGray_E);
	addSprite(rect_lilGray_SE);
	addSprite(rect_lilGray_S);
	addSprite(rect_lilGray_SW);
	addSprite(rect_lilGray_W);
	addSprite(rect_lilGray_NW);
	addSprite(rect_singGreen_straight);
	addSprite(rect_singGreen_hVert);
	addSprite(rect_singGreen_fVert);
	addSprite(rect_singGreen_updown);
	addSprite(rect_doubGreen_straight);
	addSprite(rect_doubGreen_roll1);
	addSprite(rect_doubGreen_roll2);
	addSprite(rect_doubGreen_roll3);
	addSprite(rect_doubGreen_roll4);
	addSprite(rect_doubGreen_roll5);
	addSprite(rect_doubGreen_roll6);
	addSprite(rect_doubGreen_roll7);
	addSprite(rect_purp4);
	addSprite(rect_purp3);
	addSprite(rect_purp2);
	addSprite(rect_purp1);
	addSprite(rect_bigGray);

	setYVel(2);
}

/*Enemy::Enemy(double xP, double yP, double xV, double yV, enemyType e) : Enemy(xP,yP,e) {
	setYVel(yV);
	setXVel(xV);
}*/

SDL_Rect Enemy::getSprite(){
	if (enemy == RED){
		double xv = getXVel();
		double yv = getYVel();
//		std::cout << xv << ", " << yv << std::endl;
		if (std::abs(xv) - std::abs(yv) >=  .15) {
			if (xv > 0) return sprites[SPR_RED_E];
			else return sprites[SPR_RED_W];
		} else if (std::abs(xv) - std::abs(yv) <= -.15){
//			std::cout << yv << std::endl;
			if (yv > 0) return sprites[SPR_RED_S];
			else return sprites[SPR_RED_N];
		} else {
			if (yv > 0 && xv > 0) return sprites[SPR_RED_SE];
			if (yv < 0 && xv > 0) return sprites[SPR_RED_NE];
			if (yv > 0 && xv < 0) return sprites[SPR_RED_SW];
			if (yv < 0 && xv < 0) return sprites[SPR_RED_NW];
		}
	} else if (enemy == LITTLEGRAY){
		double xv = getXVel();
		double yv = getYVel();
//		std::cout << xv << ", " << yv << std::endl;
		if (std::abs(xv) - std::abs(yv) >=  .15) {
			if (xv > 0) return sprites[SPR_LILGRAY_E];
			else return sprites[SPR_LILGRAY_W];
		} else if (std::abs(xv) - std::abs(yv) <= -.15){
//			std::cout << yv << std::endl;
			if (yv > 0) return sprites[SPR_LILGRAY_S];
			else return sprites[SPR_LILGRAY_N];
		} else {
			if (yv > 0 && xv > 0) return sprites[SPR_LILGRAY_SE];
			if (yv < 0 && xv > 0) return sprites[SPR_LILGRAY_NE];
			if (yv > 0 && xv < 0) return sprites[SPR_LILGRAY_SW];
			if (yv < 0 && xv < 0) return sprites[SPR_LILGRAY_NW];
		}
	}
	return sprites[SPR_RED_N];
}

enemyType Enemy::getEnemyType(){
	return enemy;
}

Enemy::~Enemy(){
	count--;
}

int Enemy::getCount(){
	return count;
}

int Enemy::canFire(){
	return (std::abs(getXVel()) - std::abs(getYVel()) <= -.15 && getYVel() > 0);
}

int Enemy::update(){

	AIcount = AIcount++ % 7000;
	if (AIcount > 7000) AIcount = 0;

	double yP = getYPos();
	double xP = getXPos();

/*	if (yP < 200 && yP > 0){
		setYVel(getYVel() + .005);
	} else if (yP > 300){
		setYVel(getYVel() - .005);
	} else if (yP < 0 ){
		setYVel(getYVel() + .005);
	}

	if (yP < 100 && getYVel() < 0){
		setYVel(getYVel() + std::abs(100 - yP)/100000);
	}

	if (xP < 220 ){
		if (getXVel() < 0){
			setXVel(getXVel() - .01);
		} else {
			setXVel(getXVel() + .01);
		}
			
	} else if (xP > 260) {
		if (getXVel() < 0){
			setXVel(getXVel() + .02);
		} else {
			setXVel(getXVel() - .02);
		}
	} else {
		setXVel(getXVel() - .02);
	}

	if (xP < 120){
		setXVel(getXVel() + .01);
	} else if (xP > 360){
		setXVel(getXVel() - .01);
	}
	*/
	//std::cout << getCount() << std::endl;

	setXVel(cos(AIcount*6.28*5/7000));
	if ((getYVel()<0) && (yP>0)) setYVel(-2);
	else if (yP > 200) setYVel((-yP)/150 + (200-yP)/80+sin(AIcount*6.28/7000)*5 + 1);
	else if ((yP>-150)&&(getYVel()<0)) setYVel(-2);
	else setYVel(2);

	if (getYVel() > 4) setYVel(4);
	if (getYVel() < -4) setYVel(-4);
	if (getXVel() > 4) setXVel(4);
	if (getXVel() < -4) setXVel(-4);
	if ((getYVel()>-1) && (getYVel()<0)) setYVel(-1);
	if ((getYVel()<1) && (getYVel()>0)) setYVel(1);
	//std::cout << AIcount << std::endl;
	return ((AIcount % 1000 == 200 || AIcount % 1000 == 300 ) && canFire());
}
