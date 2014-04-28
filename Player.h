/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Player.h
	Interface of Player class (coordinate movement and sprites of player's plane)
*/
#ifndef PLAYER_H
#define PLAYER_H

#include"SDL/SDL.h"
#include"GraphElement.h"
#include"Counter.h"

typedef enum {
	SPR_PLYR_HLTHY_FROLLLEFT,
	SPR_PLYR_HLTHY_HROLLLEFT,
	SPR_PLYR_HLTHY_STRAIGHT,
	SPR_PLYR_HLTHY_HROLLRIGHT,
	SPR_PLYR_HLTHY_FROLLRIGHT,
	SPR_PLYR_DMGD_FROLLLEFT,
	SPR_PLYR_DMGD_HROLLLEFT,
	SPR_PLYR_DMGD_STRAIGHT,
	SPR_PLYR_DMGD_HROLLRIGHT,
	SPR_PLYR_DMGD_FROLLRIGHT,
	SPR_PLYR_INTR_1,
	SPR_PLYR_INTR_2,
	SPR_PLYR_INTR_3,
	SPR_PLYR_INTR_4,
	SPR_PLYR_INTR_5,
	SPR_PLYR_INTR_6,
	SPR_PLYR_INTR_7
} playerSpriteType;

typedef enum {
			STANDARD,
			SPREAD,
			MISSILE,
			BEAM,
			AUTO
} weaponType;

class Player : public GraphElement {
	public:
		Player(double, double); //nondefault constructor, passes position and type to GraphElement constructor [xPos, yPos]
		SDL_Rect getSprite();
		void add50_IncAmmo();
		void sub1_IncAmmo();
		void add4_IncHealth();
		void sub1_IncHealth();
		int getXMom();
		int getYMom();
		void setXMom(int);
		void setYMom(int);
		Counter ammo; //[xPos = 100, yPos = 300, value = 100, min = 0, max = 100, interval = 1]
		Counter health; //[xPos = 100, yPos = 100, value = 100, min = 0, max = 100, interval = 5]
	private:
		playerSpriteType sprite; //enum that stores player plane's sprite
		weaponType weapon; //enum that stores player plane's current weapon
		int xMom;
		int yMom;
};

#endif //PLAYER_H
