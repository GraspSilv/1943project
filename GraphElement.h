/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
GraphElement.h
	Interface of GraphElement class (store location and sprites of graphic element)
History
	03/31/14	Bill Gowans		Declare constructor, SDL_graph_function(), and private variables x_Pos and y_Pos
	04/09/14	Jon Richelsen	Standardize, combine with Sprite class, declare addSprite(), getSprite(), get[XY](), set[XY](), setY(), and private variable sprites
	04/22/14	Jon Richelsen	Declare deconstructor and private variable GE_Ptrs, remove sprite struct, and change sprites vector to a vector of SDL_Rects, rename get[XY]() and set[XY] to get[XY]Pos() and set[XY]Pos, declare get[XY]Vel() and setVel[XY] and prvate variables xVel and yVel
	04/27/14	Jon Richelsen	Declare new nondefault constructor to set velocity, change descriptions of nondefault constructors to include type, declare getType
	04/27/14	Jack Magiera	Add BULLET to GEType enum, declare virtual deconstructor, remove GE_Ptrs from private data
	04/28/14	Jon Richelsen	Declare isOnScreen()
To Do
	Remove screen attributes from source file
*/
#ifndef GRAPHELEMENT_H
#define GRAPHELEMENT_H

#include<string>
#include<vector>
#include "SDL/SDL.h"


typedef enum {
	BULLET,
	ENEMY,
	EXPLOSION,
	PLAYER,
	POWERUP
} GEType;

class GraphElement {
	public:
		GraphElement(double, double, GEType); //nondefault constructor, sets position and type [xPos, yPos, type]
		GraphElement(double, double, double, double, GEType); //nondefault constructor, sets position, velocity, and type [xPos, yPos, xVel, yVel, type]
		void addSprite(SDL_Rect); //adds sprite struct to sprites array [clip]
		virtual SDL_Rect getSprite() = 0; //returns SDL_Rect of sprite based on derived class behavior
		GEType getType();
		double getXPos();
		double getYPos();
		double getXVel();
		double getYVel();
		void setXPos(double);
		void setYPos(double);
		void setXVel(double);
		void setYVel(double);
		int isOnScreen();
		std::vector<SDL_Rect> sprites;
		virtual ~GraphElement(); //deconstructor, removes this pointer from pointers vector
		
		//following are functions that we were forced to virtualize
		virtual int getOrigin();
		virtual int isDone();
		//virtual void sub1_IncHealth();
		//virtual void add4_IncHealth();
	private:
		GEType type;
		double xPos;
		double yPos;
		double xVel;
		double yVel;
};

#endif //GRAPHELEMENT_H
