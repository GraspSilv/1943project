#include <algorithm>
#include <iostream>
#include <string>
#include <stdlib>
#include "Enemy.h"

#ifndef LEVEL_H
#define LEVEL_H



class Level {
	public:
		Level (string);
		int getNextGroup();
		enemyType getNextType();
		int getLevelNum();
		int init();
	private:
		Background back;
		int levelNum;
		std::vector<int> groups;
		std::vector<enemyType> types;
		string levelFile;
};

#endif