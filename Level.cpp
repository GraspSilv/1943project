#include "Level.h"
#include <iostream>
#include <fstream>

Level::Level (std::string str) {
	levelFile = str;
	levelNum = 0;
}

std::string Level::getLevelText(){
	std::stringstream s;
	s << "Level " << levelNum;
	return s.str();
}

Background * Level::getBackground(){
	return back;
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
			h = intString[i] - '0';
			//std::cout << h << std::endl;
			groups.push_back(h);
		}
		groups.push_back(0);
		// std::cout << "4" << std::endl;

		std::string hold;
		while (j > 0){
			getline(lFile,hold);
			if (hold == "red"){
				types.push_back(RED);
			} else if (hold == "grey"){
				types.push_back(LITTLEGRAY);
			} else if (hold == "green1"){
				types.push_back(SINGLEGREEN);
			} else if (hold == "green2"){
				types.push_back(DOUBLEGREEN);
			}
			j--;
		}

		return 1;

	} else {return 0;}
}

int Level::getLevelNum(){
	return levelNum;
}

int Level::getNextGroup(){
	int n = groups[0];
	groups.erase(groups.begin());
	return n;
}

enemyType Level::getNextType(){
	enemyType n = types[0];
	types.erase(types.begin());
	return n;
}