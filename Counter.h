/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Counter.h
	Interface of Counter class (tracks and visually displays properties)
History
	03/26/14	Jack Magiera	Create, declare default constructor, nondefault constructor, reset(), increment(), ++, getValue(), render(), and private variables min, max, value, interval, x, and y
	04/16/14	Jon Richelsen	Standardize, change variables to more readable names, declare getXPos(), getYPos(), ++ (prefix), and ++ (postfix)
	04/23/14	Jon Richelsen	Declare decrement()
To Do
	Check that overloaded ++ operator works
	Develop icon-based counter (e.g. would display 5 plane icons instead of "5")
*/
#ifndef COUNTER_H
#define COUNTER_H

#include"SDL/SDL.h"
#include"SDL/SDL_ttf.h"

class Counter {
	public:
		Counter(); //default constructor, sets x-position to 100, y-position to 10, value to 0, minimum to 0, maximum to 10, and interval to 1 [xPos = 100, yPos = 10, value = 0, min = 0, max = 10, interval = 1]
		Counter(int, int, int, int, int, int); //nondefault constructor, sets x-position, y-position, value, minimum, maximum, and interval [xPos, yPos, value, min, max, interval]
		int getValue();
		int getXPos();
		int getYPos();
		void decrement(int); //increments counter by n intervals (not allowing to go negative) [n]
		void increment(int); //increments counter by n intervals [n]
		void reset();
		SDL_Surface * render(TTF_Font *, SDL_Color);
		Counter operator++(); //overloads prefix ++ operator to increase value by one interval
		Counter operator++(int); //overloads postix ++ operator to increase value by one interval
private:
		int xPos;
		int yPos;
		int value;
		int min;
		int max;
		int interval;
};

#endif //CLASSNAME_H
