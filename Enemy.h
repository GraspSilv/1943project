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
	RECT_RED_N,
	RECT_RED_NE,
	RECT_RED_E,
	RECT_RED_SE,
	RECT_RED_S,
	RECT_RED_SW,
	RECT_RED_W,
	RECT_RED_NW,
	RECT_LILGRAY_N,
	RECT_LILGRAY_NE,
	RECT_LILGRAY_E,
	RECT_LILGRAY_SE,
	RECT_LILGRAY_S,
	RECT_LILGRAY_SW,
	RECT_LILGRAY_W,
	RECT_LILGRAY_NW,
	RECT_SINGGREEN_STRAIGHT,
	RECT_SINGGREEN_HVERT,
	RECT_SINGGREEN_FVERT,
	RECT_SINGGREEN_UPDOWN,
	RECT_DOUBGREEN_STRAIGHT,
	RECT_DOUBGREEN_ROLL1,
	RECT_DOUBGREEN_ROLL2,
	RECT_DOUBGREEN_ROLL3,
	RECT_DOUBGREEN_ROLL4,
	RECT_DOUBGREEN_ROLL5,
	RECT_DOUBGREEN_ROLL6,
	RECT_DOUBGREEN_ROLL7,
	RECT_PURP4,
	RECT_PURP3,
	RECT_PURP2,
	RECT_PURP1,
	RECT_BIGGRAY
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
		Enemy (double, double, enemyType);
		// Enemy (double, double, double, double, enemyType);
		SDL_Rect getSprite();
		enemyType getEnemyType();
		void update();	
	private:
		enemySpriteType sprite;
		enemyType enemy;
};

#endif //ENEMY_H
