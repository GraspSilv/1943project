#include "Level.h"
#include <iostream>
#include <fstream>

Level::Level (std::string str) {
	levelFile = str;
	levelNum = 0;
}


int Level::init(){
	// std::cout << "1" << std::endl;
	std::ifstream lFile;
	lFile.open(levelFile.c_str());
	if (lFile.is_open()){
		// std::cout << "2" << std::endl;
		std::string bgfile;
		getline(lFile,bgfile);
		back = new Background(bgfile);
		// std::cout << "3" << std::endl;
		//std::getline(lFile,levelNum);
		lFile >> levelNum;
		char x;
		lFile >> x;
		std::string intString;
		getline(lFile,intString);
		int h;
		int j = intString.size();
		for (int i = 0; i < j; i++){
			h << intString[i];
			groups.push_back(h);
		}
		// std::cout << "4" << std::endl;
		std::vector<std::string> types;
		std::string hold;
		while (j > 0){
			getline(lFile,hold);
			// std::cout << hold << std::endl;
			types.push_back(hold);
			// std::cout << "5" << std::endl;
			j--;
		}
		// std::cout << types[1] << std::endl;
		// std::cout << "5" << std::endl;
		return 1;

	} else {return 0;}
}

int Level::getLevelNum(){
	return levelNum;
}

int Level::getNextGroup(){
	int n = groups.front();
	groups.erase(groups.begin());
	return n;
}