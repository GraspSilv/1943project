#include "Level.h"
#include <iostream>
#include <fstream>
#include"SDL/SDL.h"
#include"SDL/SDL_image.h"
#include"SDL/SDL_ttf.h"
#include"SDL/SDL_mixer.h"

int main() {
	Level lev("level1.lev");
	lev.init();
	std::cout << lev.getLevelNum() << std::endl;
}