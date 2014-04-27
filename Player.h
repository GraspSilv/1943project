/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Player.h
	Interface of Player class (coordinate movement and sprites of player's plane)
History
	03/31/14	Bill Gowans		Create, declare constructor, Sprite(), scoreCntr(), healthCntr(), ammoCntr(), livesCntr(), get[XY](), get[XY]Vel(), and private variables [xy]_vel
	04/09/14	Jon Richelsen	Start standardization
	04/22/14 Jon Richelsen	Finish standardization, define spriteType and weaponType enums, sprite, and weapon, fix typos, remove get[XY]Vel() and private variables [xy]Vel, rename spriteType to playerSpriteType
	04/23/14 Jon Richelsen	Declare add50_IncAmmo(), sub1_IncAmmo(), add4_IncHealth(), sub1_IncHealth(), and private variables ammo and health
	04/27/14	Jon Richelsen	Change description of nondefault constructor to include type
To Do
	Decide where to put spriteType and weaponType enums
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

class Player : public GraphElement{
	public:
		Player(double, double); //nondefault constructor, passes position and type to GraphElement constructor [xPos, yPos]
		SDL_Rect getSprite();
		void add50_IncAmmo();
		void sub1_IncAmmo();
		void add4_IncHealth();
		void sub1_IncHealth();
	private:
		playerSpriteType sprite; //enum that stores player plane's sprite
		weaponType weapon; //enum that stores player plane's current weapon
		Counter ammo; //[xPos = 100, yPos = 300, value = 100, min = 0, max = 100, interval = 1]
		Counter health; //[xPos = 100, yPos = 100, value = 100, min = 0, max = 100, interval = 5]
};

#endif //PLAYER_H
