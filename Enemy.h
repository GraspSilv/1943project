/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Enemy.h
	Interface of Enemy class (base class for derived enemy classes)
History
	03/31/14	Bill Gowans		Create, declare default constructor, forward(), slightR(), sharpR(), slightL(), sharpR(), getX(), getY(), getXVel(), getYVel(), and private variable healthCntr
	04/16/14	Jon Richelsen	Standardize
	04/27/14	Jack Magiera	Define enemySpriteType and enemyType sprites, remove default constructor, forward(), slightR(), sharpR(), slightL(), sharpR(), getX(), getY(), getXVel(), getYVel(), declare nondefault constructor, getSprite(), getEnemyType(), and private variables sprite and enemy
	04/28/14	Jon Richelsen	Capitalize enums
To Do
*/
#ifndef ENEMY_H
#define ENEMY_H

#include<string>
#include "GraphElement.h"

typedef enum {
	SPR_RED_N,
	SPR_RED_NE,
	SPR_RED_E,
	SPR_RED_SE,
	SPR_RED_S,
	SPR_RED_SW,
	SPR_RED_W,
	SPR_RED_NW,
	SPR_LILGRAY_N,
	SPR_LILGRAY_NE,
	SPR_LILGRAY_E,
	SPR_LILGRAY_SE,
	SPR_LILGRAY_S,
	SPR_LILGRAY_SW,
	SPR_LILGRAY_W,
	SPR_LILGRAY_NW,
	SPR_SINGGREEN_STRAIGHT,
	SPR_SINGGREEN_HVERT,
	SPR_SINGGREEN_FVERT,
	SPR_SINGGREEN_UPDOWN,
	SPR_DOUBGREEN_STRAIGHT,
	SPR_DOUBGREEN_ROLL1,
	SPR_DOUBGREEN_ROLL2,
	SPR_DOUBGREEN_ROLL3,
	SPR_DOUBGREEN_ROLL4,
	SPR_DOUBGREEN_ROLL5,
	SPR_DOUBGREEN_ROLL6,
	SPR_DOUBGREEN_ROLL7,
	SPR_PURP4,
	SPR_PURP3,
	SPR_PURP2,
	SPR_PURP1,
	SPR_BIGGRAY
} enemySpriteType;

typedef enum {
	RED,
	LITTLEGRAY,
	SINGLEGREEN,
	DOUBLEGREEN,
	PURPLE,
	BIGGRAY
} enemyType;

class Enemy : public GraphElement {
	public:
		Enemy(double, double, enemyType);
		// Enemy (double, double, double, double, enemyType);
		SDL_Rect getSprite();
		enemyType getEnemyType();
		int update();	
		int canFire();
	private:
		enemySpriteType sprite;
		enemyType enemy;
		int AIcount;
};

#endif //ENEMY_H
