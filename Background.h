/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Background.h
	Interface of background class (display scrolling background)
History
	03/31/14	Bill Gowans		Create
	04/02/14	Jon Richelsen	Standardize
	04/23/14	Bill Gowan		Declare nondefault constructor, load_image(), apply_surface(), init(), load_files(), clean_up(), public variables background and screen, and private variable file
	04/28/14	Jon Richelsen	Update header to include all changes
To Do
*/
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include "Timer.h"

using namespace std;

class Background {
	public:
		Background (string filename);
		SDL_Surface *load_image(string filename);
		void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
		bool init();
		bool load_files();
		void clean_up();
		SDL_Surface *background;
		SDL_Surface *screen;
	private:
		string file;
};

#endif
