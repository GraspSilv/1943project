#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Enemy.h"
#include "Background.h"
#include <sstream>

#ifndef LEVEL_H
#define LEVEL_H



class Level {
	public:
		Level (std::string);
		int getNextGroup();
		//enemyType getNextType();
		int getLevelNum();
		int init();
		enemyType getNextType();
		std::string getLevelText();
		Background* getBackground();
	private:
		Background *back;
		int levelNum;
		std::vector<int> groups;
		std::vector<enemyType> types;
		std::string levelFile;
};

#endif