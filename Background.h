/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Background.h
	Interface of background class (display scrolling background)
To Do
*/
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include<string>
#include"SDL/SDL.h"

class Background {
	public:
		Background(std::string filename);
		SDL_Surface * load_image(std::string filename);
		// SDL_Surface * getBG(int x, int y);
		void apply_surface(int x, int y, SDL_Surface * source, SDL_Surface * destination, SDL_Rect * clip = NULL);
		int init();
		int load_files();
		void clean_up();
		SDL_Surface * background;
		SDL_Surface * screen;
	private:
		std::string backFile;
};

#endif
