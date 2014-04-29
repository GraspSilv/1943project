/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Explosion.cpp
	Implementation of Explosion class (coordinate appropriate display of explosions)
*/
#include"Explosion.h"
#include<iostream>
#include<vector>
#include"SDL/SDL.h"
#include"GraphElement.h"

const int Explosion::framesPerSprite = 40;

Explosion::Explosion(double xP, double yP) : GraphElement(xP, yP, 0, 0, EXPLOSION) { //nondefault constructor, passes position to GraphElement constructor [xPos, yPos]
	sprite = SPR_EXPL_1;
	displayCount = 0;
	
	SDL_Rect rect_expl_1; //sprite of stage 1/6 of explosion
		rect_expl_1.x = 11;
		rect_expl_1.y = 277;
		rect_expl_1.w = 15;
		rect_expl_1.h = 15;

	SDL_Rect rect_expl_2; //sprite of stage 2/6 of explosion
		rect_expl_2.x = 39;
		rect_expl_2.y = 279;
		rect_expl_2.w = 10;
		rect_expl_2.h = 10;

	SDL_Rect rect_expl_3; //sprite of stage 3/6 of explosion
		rect_expl_3.x = 58;
		rect_expl_3.y = 279;
		rect_expl_3.w = 11;
		rect_expl_3.h = 11;

	SDL_Rect rect_expl_4; //sprite of stage 4/6 of explosion
		rect_expl_4.x = 80;
		rect_expl_4.y = 277;
		rect_expl_4.w = 13;
		rect_expl_4.h = 13;

	SDL_Rect rect_expl_5; //sprite of stage 5/6 of explosion
		rect_expl_5.x = 105;
		rect_expl_5.y = 277;
		rect_expl_5.w = 15;
		rect_expl_5.h = 14;

	SDL_Rect rect_expl_6; //sprite of stage 6/6 of explosion
		rect_expl_6.x = 129;
		rect_expl_6.y = 276;
		rect_expl_6.w = 16;
		rect_expl_6.h = 16;
	
	//add sprites to sprites vector
	addSprite(rect_expl_1);
	addSprite(rect_expl_2);
	addSprite(rect_expl_3);
	addSprite(rect_expl_4);
	addSprite(rect_expl_5);
	addSprite(rect_expl_6);
}

SDL_Rect Explosion::getSprite() {
	int stage = displayCount / framesPerSprite;
	switch(stage) {
		case 0:
			sprite = SPR_EXPL_1;
			break;
		case 1:
			sprite = SPR_EXPL_2;
			break;
		case 2:
			sprite = SPR_EXPL_3;
			break;
		case 3:
			sprite = SPR_EXPL_4;
			break;
		case 4:
			sprite = SPR_EXPL_5;
			break;
		case 5:
			sprite = SPR_EXPL_6;
			break;
		default:
			std::cout << "Error: Explosion sprite is not defined" << std::endl;
			std::cout << stage << std::endl;
			break;
	}
	return sprites[sprite];
}

int Explosion::update() { //returns 1 if animation is complete
	//std::cout << "In Explosion's update!" << std::endl;
	if(displayCount >= ((6 * framesPerSprite) - 1)) {
		return 1;
	}
	displayCount++;
	return 0;
}
