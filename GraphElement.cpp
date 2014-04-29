/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
GraphElement.cpp
	Implementation of GraphElement class (store location and sprites of graphic element)
History
	03/31/14	Bill Gowans		Define constructor and SDL_graph_function()
	04/09/14	Jon Richelsen	Standardize, combine with Sprite class, define constructor, addSprite(), getSprite(), get[XY](), and set[XY]()
	04/22/14	Jon Richelsen	Define deconstructor, fix typos, implement GE_Ptrs vector, and simplify addSprite() function with enum, rename get[XY]() and set[XY] to get[XY]Pos() and set[XY]Pos, define get[XY]Vel() and setVel[XY]
	04/27/14	Jon Richelsen	Define new nondefault constructor to set velocity, add type declaration to nondefault constructors, declare getType
	04/28/14	Jon Richelsen	Remove all references to GE_Ptrs, define isOnScreen(), throw in screen attributes
*/
#include"GraphElement.h"
#include<algorithm>
#include<string>
#include<vector>
#include<iostream>
#include"SDL/SDL.h"

const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 640;

GraphElement::GraphElement(double xP, double yP, GEType t) { //nondefault constructor, sets position and type [xPos, yPos, type]
	xPos = xP;
	yPos = yP;
	xVel = 0;
	yVel = 0;
	type = t;
}

GraphElement::GraphElement(double xP, double yP, double xV, double yV, GEType t) { //nondefault constructor, sets position, velocity, and type [xPos, yPos, xVel, yVel, type]
	xPos = xP;
	yPos = yP;
	xVel = xV;
	yVel = yV;
	type = t;
}

void GraphElement::addSprite(SDL_Rect clip) { //adds SDL_Rect to sprites array [clip]
	sprites.push_back(clip);
}

GEType GraphElement::getType() {
	return type;
}

double GraphElement::getXPos() {
	return xPos;
}

double GraphElement::getYPos() {
	return yPos;
}

double GraphElement::getXVel() {
	return xVel;
}

double GraphElement::getYVel() {
	return yVel;
}

void GraphElement::setXPos(double xP) {
	xPos = xP;
}

void GraphElement::setYPos(double yP) {
	yPos = yP;
}

void GraphElement::setXVel(double xV) {
	xVel = xV;
}

void GraphElement::setYVel(double yV) {
	yVel = yV;
}

int GraphElement::isOnScreen() {
	if(xPos < 0) { //if left side of Element is off left side of screen,
		return 0; //Element is not on screen
	} else if(yPos < 0) { //else, if top of Element is off top of screen,
		return 0; //Element is not on screen
	}	
	
	SDL_Rect sprite = getSprite(); //extract sprite of Element to read width and height

	if((xPos + sprite.w) > WINDOW_WIDTH) { //if right side of Element is off right side of screen,
		return 0; //Element is not on screen
	} else if((yPos + sprite.h) > WINDOW_HEIGHT) { //else, if bottom of Element is off bottom of screen,
		return 0; //Element is not on screen
	}
	return 1; //Otherwise, Element is on screen
}

GraphElement::~GraphElement() {
	
}

int GraphElement::update() {
	std::cout << "Error: update() called on object where method has not yet been defined" << std::endl;
	return 0;
}

int GraphElement::getOrigin() {
	std::cout << "Error: getOrigin() called on non-Bullet object" << std::endl;
	return -1;
}

int GraphElement::getBulletType() {
	std::cout << "Error: getBulletType() called on non-Bullet object" << std::endl;
	return -1;
}

int GraphElement::isBeam() {
	std::cout << "Error: isBeam() called on a non-player object" << std::endl;
	return -1;
}

void GraphElement::newAmmo() {
	std::cout << "Error: newAmmo() called on non-Player object" << std::endl;
}

void GraphElement::useAmmo(int n) {
	std::cout << "Error: useAmmo() called on non-Player object" << std::endl;
}

void GraphElement::newHealth() {
	std::cout << "Error: newHealth() called on non-Player object" << std::endl;
}

void GraphElement::hitByBullet() {
	std::cout << "Error: hitByBullet() called on non-Player object" << std::endl;
}

void GraphElement::hitByPlane() {
	std::cout << "Error: hitByPlane() called on non-Player object" << std::endl;
}

int GraphElement::getWeapon() {
	std::cout << "Error: getWeapon() called on non-Player object" << std::endl;
	return -1;
}

void GraphElement::setWeapon(int w) {
	std::cout << "Error: setWeapon() called on non-Player object" << std::endl;
}

int GraphElement::getPower() {
	std::cout << "Error: getPower() called on non-Powerup object" << std::endl;
	return -1;
}
