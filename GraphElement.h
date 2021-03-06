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
		void addSprite(SDL_Rect); //adds SDL_Rect to sprites array [clip]
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
		virtual ~GraphElement(); //do we need this anymore? yes
		
		//virtualized general functions
		virtual int update();
		//virtualized Bullet functions
		virtual int getOrigin();
		virtual int getBulletType();
		virtual int isBeam();
		//virtualized Player functions
		virtual void newAmmo();
		virtual void useAmmo(int);
		virtual void newHealth();
		virtual void hitByBullet();
		virtual void hitByPlane();
		virtual int getWeapon();
		virtual void setWeapon(int);
		virtual int getCycles();
		virtual void setCycles(int);
		//virtualized Powerup functions
		virtual int getPower();
	protected:
		std::vector<SDL_Rect> sprites;
	private:
		GEType type;
		double xPos; //x-position (pixels)
		double yPos; //y-position (pixels)
		double xVel; //x-velocity (pixels/frame)
		double yVel; //y-velocity (pixels/frame)
};

#endif //GRAPHELEMENT_H
