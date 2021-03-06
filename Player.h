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

// weapon
// 0	STANDARD
// 1	SPREAD
// 2	MISSILE
// 3	AUTO
// 4	BEAM


class Player : public GraphElement {
	public:
		Player(double, double); //nondefault constructor, passes position and type to GraphElement constructor [xPos, yPos]
		SDL_Rect getSprite();
		Counter getAmmoCntr();
		Counter getHealthCntr();
		void newAmmo();
		void useAmmo(int);
		void newHealth();
		void hitByBullet();
		void hitByPlane();
		int getWeapon();
		void setWeapon(int);
		int getCycles();
		void setCycles(int);
		int getXMom();
		int getYMom();
		void setXMom(int);
		void setYMom(int);
		void setBomb(int);
		int getBomb();
	private:
		playerSpriteType sprite; //enum that stores player's sprite
		Counter ammo; //[xPos = 100, yPos = (640 - 30), value = 100, min = 0, max = 100, interval = 1]
		Counter health; //[xPos = 180, yPos = (640 - 30), value = 100, min = 0, max = 100, interval = 5]
		int weapon;
		int cycles;
		int xMom; //x-momentum (unitless)
		int yMom; //y-momentum (unitless)
		int hasBomb;
};

#endif //PLAYER_H
