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

void GraphElement::addSprite(SDL_Rect clip) { //adds sprite struct to sprites array [name, clip]
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
	if(xPos < 0) {
		return 0;
	} else if(yPos < 0) {
		return 0;
	}	
	
	SDL_Rect sprite = getSprite();

	if((xPos + sprite.w) > WINDOW_WIDTH) {
		return 0;
	} else if((yPos + sprite.h) > WINDOW_HEIGHT) {
		return 0;
	}
	
	return 1;
}

GraphElement::~GraphElement() { //deconstructor, removes this pointer from pointers vector

}

int GraphElement::getOrigin() {
	std::cout << "You should not be in this getOrigin()!" << std::endl;
	return 0;
}

int GraphElement::isDone() {
	return 0;
}

//void GraphElement::sub1_IncHealth() {
//	std::cout << "You should not be in this sub1_IncHealth()!" << std::endl;
//}

//void GraphElement::add4_IncHealth() {
//	std::cout << "You should not be in this add4_IncHealth()!" << std::endl;
//}
