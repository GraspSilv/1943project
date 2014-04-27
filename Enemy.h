/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Enemy.h
	Interface of Enemy class (base class for derived enemy classes)
History
	03/31/14	Bill Gowans		Create, declare default constructor, forward(), slightR(), sharpR(), slightL(), sharpR(), getX(), getY(), getXVel(), getYVel(), and private variable healthCntr
	04/16/14	Jon Richelsen	Standardize
To Do
	Completely revamp class
*/
#ifndef ENEMY_H
#define ENEMY_H

#include<string>
#include "GraphElement.h"

typedef enum {
	rect_red_N,
	rect_red_NE,
	rect_red_E,
	rect_red_SE,
	rect_red_S,
	rect_red_SW,
	rect_red_W,
	rect_red_NW,
	rect_lilGray_N,
	rect_lilGray_NE,
	rect_lilGray_E,
	rect_lilGray_SE,
	rect_lilGray_S,
	rect_lilGray_SW,
	rect_lilGray_W,
	rect_lilGray_NW,
	rect_singGreen_straight,
	rect_singGreen_hVert,
	rect_singGreen_fVert,
	rect_singGreen_updown,
	rect_doubGreen_straight,
	rect_doubGreen_roll1,
	rect_doubGreen_roll2,
	rect_doubGreen_roll3,
	rect_doubGreen_roll4,
	rect_doubGreen_roll5,
	rect_doubGreen_roll6,
	rect_doubGreen_roll7,
	rect_purp4,
	rect_purp3,
	rect_purp2,
	rect_purp1,
	rect_bigGray
} enemySpriteType;

typedef enum {
	red,
	lilGray,
	singGreen,
	doubGreen,
	purp,
	bigGray
} enemyType;

class Enemy : public GraphElement {
	public:
		Enemy (double, double, enemyType);
		// Enemy (double, double, double, double, enemyType);
		SDL_Rect getSprite();
		enemyType getEnemyType();	
	private:
		enemySpriteType sprite;
		enemyType enemy;
};

#endif //ENEMY_H
