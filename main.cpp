/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
main.cpp
	Driver for Blitz game
*/
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<dirent.h>
#include<fstream>
#include<iostream>
#include<string>
#include"SDL/SDL.h"
#include"SDL/SDL_image.h"
#include"SDL/SDL_mixer.h"
#include"SDL/SDL_ttf.h"
#include"Background.h"
#include"Bullet.h"
#include"Counter.h"
#include"Enemy.h"
#include"Explosion.h"
#include"GraphElement.h"
#include"Level.h"
#include"Player.h"
#include"Powerup.h"
#include"Timer.h"

const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 640;
const int WINDOW_BPP = 32;
const int WINDOW_BUFF = 5;
const std::string WINDOW_TITLE = "Blitz";

const int GAME_FPS = 80;

const int PL_SPEED1 = 2;
const int PL_SPEED2 = 4;
const int PL_SPEED3 = 6;
const int momThresh = 60;

const int BUL_SPEED = 4;
const int BEAM_SPEED = 8;

SDL_Surface * ammoSurface = NULL;
SDL_Surface * ammoLabelSurface = NULL;
SDL_Surface * bombNotification = NULL;
SDL_Surface * healthSurface = NULL;
SDL_Surface * healthLabelSurface = NULL;
SDL_Surface * levelLabelSurface = NULL;
SDL_Surface * livesSurface = NULL;
SDL_Surface * livesLabelSurface = NULL;
SDL_Surface * scoreSurface = NULL;
SDL_Surface * scoreLabelSurface = NULL;
SDL_Surface * screen = NULL;
SDL_Surface * spacePrompt = NULL;
SDL_Surface * splashScreen = NULL;
SDL_Surface * spriteSheet = NULL;

TTF_Font * font = NULL;

Mix_Music * music = NULL;
Mix_Music * starship = NULL;

Mix_Chunk * enemyDown = NULL;
Mix_Chunk * fireAtWill = NULL;
Mix_Chunk * guncock = NULL;
Mix_Chunk * gunfire = NULL;

SDL_Color backgroundColor = {0, 0, 0};
SDL_Color keyColor = {0, 0x2A, 0x88};
SDL_Color textColor = {255, 255, 255};

Counter score(300, (WINDOW_HEIGHT - 30), 0, 0, 1000000, 10);

int enemyCount = 0;
int beamCycles = 0; //what is this?

void applySurface(int x, int y, SDL_Surface * src, SDL_Surface * dest, SDL_Rect * clip = NULL);
int checkCollide(GraphElement * a, GraphElement * b);
void cleanUp();
int collide(GraphElement * GE1, GraphElement * GE2, GEType type1, GEType type2, std::vector<GraphElement *> * elemPtr);
int collideBulletEnemy(int xArg, GraphElement * b, GraphElement * e, std::vector<GraphElement *> * elemPtr);
int collideBulletPlayer(int xArg, GraphElement * b, GraphElement * pl, std::vector<GraphElement *> * elemPtr);
int collideEnemyPlayer(int xArg, GraphElement * e, GraphElement * pl, std::vector<GraphElement *> * elemPtr);
int collidePlayerPowerup(int xArg, GraphElement * pl, GraphElement * po, std::vector<GraphElement *> * elemPtr);
int init();
int loadFiles();
SDL_Surface * loadImage(std::string filename);
double sinDeg(double deg);
double cosDeg(double deg);

int main(int argc, char * argv[]) {
	gameStart:
	int gameRunning = 1;
	int playerIsDead = 0;
	int gameIsOver = 0;
	int lastLevel = 0;

	int isBombing = 0;
	int bombStart = 0;
	int alternate = 0;

	//initialize
	if(!init()) {
		std::cout << "Error: Could not initialize in main()" << std::endl;
		return 1;
	}
		
	//load files
	if(!loadFiles()) {
		std::cout << "Error: Could not load files in main()" << std::endl;
		return 1;
	}
	
	SDL_Event event; //event structure for checking if user has exited and generating bullets
	std::vector<GraphElement *> elements; //vector of pointers to all graphic elements
	std::vector<Level *> levels; //vector of pointers to levels

	int levelBreak = 0;
	int finishLevel = 0;
	int levelTitle = 1;
	
	//Jon wants to comment and understand this later
	std::string temp;
	DIR * pDir;
	struct dirent * entry;
	if(pDir = opendir("./levels")) {
		while(entry = readdir(pDir)) {
			if((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
				temp = std::string("./levels/") + entry->d_name;
				// std::cout << temp << std::endl;
				levels.push_back(new Level(temp));
			}
		// std::cout << entry->d_name << std::endl;
		}
		closedir(pDir);
	}

	Level * lev = levels.front(); //load first level
	lev->init(); //initiate first level
	levels.erase(levels.begin()); //remove first level from levels vector
	int maxShips = lev->getNextGroup(); //set maxShips for first level
	enemyType shipType = lev->getNextType(); //set initial shipType for first level

	Background * bg = lev->getBackground(); //extract background for first level
	int bgX = 0, bgY = 0; //set initial background offsets

	//initialize background
	if(!bg->init()) {
		std::cout << "Error: Could not initialize background" << std::endl;
		return 1;
	}

	//load background files
	if(!bg->load_files()) {
		std::cout << "Error: Could not load background files" << std::endl;
		return 1;
	}

	Counter lives(10, (WINDOW_HEIGHT - 30), 5, 0, 100, 1);
	
	//create initial player
	Player * currentPlayer = new Player((WINDOW_WIDTH / 2), (WINDOW_HEIGHT - 100));
	elements.push_back(currentPlayer);
	
	//generate ships for first level
	for(int i = maxShips; i > 0; i--) { //for maxShips times,
		elements.push_back(new Enemy((50 + (i * 50)), (-50 - (50 * i)), shipType)); //create Enemy with appropriate shipType
		enemyCount++; //increment enemy count
	}

	int addShips = 0; //tell program we are no longer adding ships

	levelLabelSurface = TTF_RenderText_Solid(		font, lev->getLevelText().c_str(),	textColor); //set first level's label

	//create and start timer
	Timer gameTimer;
	gameTimer.start();
	
	int frameTime = 1000 / GAME_FPS; //calculate time (in ms) each frame will be on screen
	
	int shipCounter = 0;
	
	music = starship; //manually set starship as music
	
	//start music
	if(Mix_PlayMusic(music, 5) == -1){
		return 1;
	}
	
	Mix_PlayChannel(-1, guncock, 0); //play guncock

	//render labels
	ammoLabelSurface =	TTF_RenderText_Solid(font, "Ammo",								textColor);
	bombNotification =	TTF_RenderText_Solid(font, "Press X for Air Support",		textColor);
	healthLabelSurface =	TTF_RenderText_Solid(font, "Health",							textColor);
	livesLabelSurface =	TTF_RenderText_Solid(font, "Lives",								textColor);
	scoreLabelSurface =	TTF_RenderText_Solid(font, "Score",								textColor);
	spacePrompt = 			TTF_RenderText_Solid(font, "Press [SPACE] to Continue",	textColor);
	
	applySurface(0, 0, splashScreen, screen); //apply splash screen
	applySurface(20, (WINDOW_HEIGHT - 30), spacePrompt, screen); //apply space prompt
	SDL_Flip(screen); //update screen

	while(1) { //wait...
		if(SDL_PollEvent(&event)) { //if there is event to handle
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) { //if user pressed key and user pressed SPACE,
				break; //break out of wait
			} else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) { //else, if user pressed key and user pressed ESC,
				return 1; //quit game
			}
		}
	}

	while(gameRunning) {
		//reset player's velocity
		currentPlayer->setXVel(0);
		currentPlayer->setYVel(0);
		
		if((gameTimer.get_ticks() % frameTime) == 0) { //if enough time has passed to create a new frame,
			if(levelBreak) { //if at break in level,
				applySurface(50, 350, spacePrompt, screen); //apply space prompt
				if(finishLevel) { //if user has finished level,
					shipCounter = 0; //reset ship counter			
					if(lastLevel || levels.empty()) { //if there are no remaining levels,
						levelLabelSurface = TTF_RenderText_Solid(font, "GAME CLEAR", textColor); //set level label to game end message
						applySurface(150, 300, levelLabelSurface, screen); //apply level label
						applySurface(50, 350, spacePrompt, screen); //apply space prompt
						SDL_Flip(screen); //update screen
						if(SDL_PollEvent(&event)) { //if there is event to handle,
							if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) { //if user pressed key and key is SPACE,
								gameIsOver = 1; //set game is over
								goto gameStart; //restart game
							} else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) { //else, if user pressed key and key is ESC,
								return 1; //quit game
							}
						}
						continue; //restart at top of gameRunning loop
					} else { //else, if there are remaining levels
						finishLevel = 0; //new level will not be finished
						lastLevel = 0; //this is not the last level
						lev = levels.front(); //load next level
						lev->init(); //initialize new level
						levels.erase(levels.begin()); //erase new level from levels vector
					}

					if(levels.empty()) { //if levels vector is empty,
						lastLevel = 1; //this is the last level
					}
					
					maxShips = lev->getNextGroup(); //load maxShips from level
					enemyType shipType = lev->getNextType(); //load shipType from level
					for(int i = maxShips; i > 0; i--) {
						elements.push_back(new Enemy((50 + (i * 50)), (-500 - (50 * i)), shipType));
						enemyCount++;
					}
					bg = lev->getBackground();
					
					//initialize background
					if(!bg->init()) {
						std::cout << "Error: Could not initialize background" << std::endl;
						return 1;
					}

					//load background files
					if(!bg->load_files()) {
						std::cout << "Error: Could not load background files" << std::endl;
						return 1;
					}
					levelLabelSurface = TTF_RenderText_Solid(font, lev->getLevelText().c_str(), textColor);

				}

				if(SDL_PollEvent(&event) && event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_SPACE) {
						levelTitle = 1;
						levelBreak = 0;
						shipCounter = 0;
						addShips = 0;
					}
				}
				continue;
			}
			bgY += 1;
			if(bgY >= bg->background->h) {//if background has scrolled too far,
				bgY = 0; //reset the offset
			}

			//std::cout << shipCounter << std::endl;
			shipCounter++;
			if(shipCounter > 100) {
				if(beamCycles > 0) {
					beamCycles--;
				} else {
					currentPlayer->setWeapon(0);
				}

				if (playerIsDead){
					if (lives.getValue() == 0){
						levelLabelSurface = TTF_RenderText_Solid(font, "GAME OVER",	textColor);
						applySurface(150,300,levelLabelSurface,screen);
						applySurface(50, 350,spacePrompt,screen);
						SDL_Flip(screen);
						gameIsOver = 1;
						if(SDL_PollEvent(&event) && event.type == SDL_KEYDOWN) {
							if (event.key.keysym.sym == SDLK_SPACE) {
								goto gameStart;
							}
						}
						continue;

					} else {
						playerIsDead = 0;
					}
				}
				shipCounter = 0;
				levelTitle = 0;
				if (!alternate) alternate = 1;
				else alternate = 0;
				// std::cout << enemyCount << " " << maxShips <<  std::endl;

				//BEGIN BOMBING
			} else if (isBombing) {
				Mix_PauseMusic();
				//std::cout << "in isBombing " << shipCounter << std::endl;
				if (bombStart == 0) bombStart = shipCounter;
				else if (bombStart -1 == shipCounter){
					Mix_PlayChannel(-1, enemyDown, 0);
					Mix_ResumeMusic();
					isBombing = 0;
					bombStart = 0;
				} else if (shipCounter % 4 == 1){
					//std::cout << "in creating " << shipCounter << std::endl;
					gunfire->volume=20;
					Mix_PlayChannel(-1, gunfire, 0);
					elements.push_back(new Bullet((24 - shipCounter/4)*10, WINDOW_HEIGHT - 20, 0, -BUL_SPEED * 1.5, 1, 0));
					elements.push_back(new Bullet((24 + shipCounter/4)*10, WINDOW_HEIGHT - 20, 0, -BUL_SPEED * 1.5, 1, 0));
				}
			}
			//std::cout << currentPlayer->getBomb() << std::endl;

			if (alternate && currentPlayer->getBomb() > 0){
				applySurface(50,10,bombNotification,screen);
				SDL_Flip(screen);
			}


			if((enemyCount <= (maxShips / 2)) && (shipCounter == 70) && !addShips && (maxShips != 0)) {
				//std::cout << "counting ships" << std::endl;
				maxShips = lev->getNextGroup();
				//std::cout << "ok" << std::endl;
				if(maxShips != 0) {
					shipType = lev->getNextType();
				//std::cout << "not ok" << std::endl;
					addShips = 1;
				} else {
					if (lastLevel == 0 && !levels.empty()) {
						levelLabelSurface = TTF_RenderText_Solid(font, "LEVEL CLEAR", textColor);
						score.increment(12);
					}
					addShips = 0;
					maxShips = 0;
					std::cout << "Done\n";
					finishLevel = 1;
				}
			}

			if(addShips && (shipCounter == 50) && (maxShips != 0)) {
				// std::cout << "counting ships " << enemyCount + 1 << std::endl;
				elements.push_back(new Enemy(50 + enemyCount * 50, -50 - 50*enemyCount, shipType));
				enemyCount++;
				if ((enemyCount >= maxShips) || (maxShips == 0)) {
					addShips = 0;
				}
			}

			//show background
			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
			bg->apply_surface(bgX, bgY,							bg->background, bg->screen);
			bg->apply_surface(bgX, bgY - bg->background->h,	bg->background, bg->screen);
			if (levelTitle) {
				applySurface(150,300,levelLabelSurface,screen);
				//applySurface(20, 350,spacePrompt,screen);
			} else if (finishLevel){
				applySurface(50, 350,spacePrompt,screen);
			}

			if(SDL_PollEvent(&event)) { //if there is event to handle,
				if(event.type == SDL_QUIT) { //if user has Xed out of window,
					gameRunning = 0; //quit game
				}
				else if(event.type == SDL_KEYDOWN) { //else, if user pressed key
					switch (event.key.keysym.sym){ //inspect which key was pressed
						case SDLK_ESCAPE: //if pressed escape,
							gameRunning = 0; //quit game
							break;
						case SDLK_z: //if pressed Z							
							if(currentPlayer->getAmmoCntr().getValue() > 0 || currentPlayer->getWeapon() == 4) { //if player has ammo,
								//isLaser = (beamCycles > 0); //check if laser enabled
								switch(currentPlayer->getWeapon()) {
									case 0: //STANDARD
										//create two normal bullets and subtract 2 ammo from player
										gunfire->volume=20;
										Mix_PlayChannel(-1, gunfire, 0); //play gunfire sound
										score.increment(1);
										elements.push_back(new Bullet((currentPlayer->getXPos() + 16),	currentPlayer->getYPos(), 0, -BUL_SPEED, 1, 0));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 5),	currentPlayer->getYPos(), 0, -BUL_SPEED, 1, 0));
										currentPlayer->useAmmo(2);
										break;
									case 1: //SPREAD
										//create seven spread bullets and subtract 7 ammo from player
										gunfire->volume=20;
										Mix_PlayChannel(-1, gunfire, 0);
										score.increment(3);
										elements.push_back(new Bullet((currentPlayer->getXPos() + 10), currentPlayer->getYPos(), (cosDeg(30) * BUL_SPEED), -(sinDeg(30) * BUL_SPEED), 1, 1));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 10), currentPlayer->getYPos(), (cosDeg(45) * BUL_SPEED), -(sinDeg(45) * BUL_SPEED), 1, 1));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 10), currentPlayer->getYPos(), (cosDeg(70) * BUL_SPEED), -(sinDeg(70) * BUL_SPEED), 1, 1));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 10), currentPlayer->getYPos(), (cosDeg(90) * BUL_SPEED), -(sinDeg(90) * BUL_SPEED), 1, 1));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 10), currentPlayer->getYPos(), (cosDeg(110) * BUL_SPEED), -(sinDeg(110) * BUL_SPEED), 1, 1));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 10), currentPlayer->getYPos(), (cosDeg(135) * BUL_SPEED), -(sinDeg(135) * BUL_SPEED), 1, 1));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 10), currentPlayer->getYPos(), (cosDeg(150) * BUL_SPEED), -(sinDeg(150) * BUL_SPEED), 1, 1));
										currentPlayer->useAmmo(2);
										break;
									case 2: //MISSILE
										std::cout << "Error: Cannot fire missile Bullet because behavior not defined" << std::endl;
										break;
									case 3: //AUTO
										std::cout << "Error: Cannot fire auto Bullet because behavior not defined" << std::endl;
										break;
									case 4: //BEAM
										//create two beam bullets and subtract 2 ammo from Player
										score.increment(3);
										// std::cout << "laser" << std::endl;
										elements.push_back(new Bullet((currentPlayer->getXPos() + 16),	currentPlayer->getYPos(), 0, -BEAM_SPEED, 1, 4));
										elements.push_back(new Bullet((currentPlayer->getXPos() + 5),	currentPlayer->getYPos(), 0, -BEAM_SPEED, 1, 4));
										//currentPlayer->useAmmo(2);
										break;
									default:
										std::cout << "Error: Cannot fire undefined Bullet because behavior not defined" << std::endl;
										break;
								}
							}
							break;
						case SDLK_x:
							if (currentPlayer->getBomb()){
								// std::cout << "bombing" << std::endl;
								Mix_PlayChannel(-1, fireAtWill, 0);
								currentPlayer->setBomb(currentPlayer->getBomb() - 1);
								isBombing = 1;
							}
							break;
						default: //if other key,
							break; //do nothing
					}
				}
			}

			//begin testing keys to control player movement
			Uint8 * keystates = SDL_GetKeyState(NULL);

			int xMom = currentPlayer->getXMom();
			int yMom = currentPlayer->getYMom();

			//handle upward movement
			if(keystates[SDLK_UP] && currentPlayer->getYPos() >= WINDOW_BUFF) { //if up is pressed and player is not about to fly off top of screen,
				if(yMom < -momThresh) { //if y-momentum is significant,
					currentPlayer->setYVel(currentPlayer->getYVel() - PL_SPEED2); //make the y-velocity be PL_SPEED2 pixels/frame upwards (greater than it currently is, 0)
				} else { //otherwise,
					currentPlayer->setYMom(yMom - 1); //give player more y-momentum upwards
					currentPlayer->setYVel(currentPlayer->getYVel() - PL_SPEED1); //make the y-velocity be PL_SPEED1 pixels/frame upwards (greater than it currently is, 0)
				}
			} else if (!keystates[SDLK_DOWN]) { //if down is pressed when up is not pressed,
				currentPlayer->setYMom(0); //reset player's y-momentum
			}

			//handle downward movement
			if(keystates[SDLK_DOWN] && currentPlayer->getYPos() <= (WINDOW_HEIGHT - WINDOW_BUFF - currentPlayer->getSprite().h)) {
				if(yMom > momThresh) {
					currentPlayer->setYVel(currentPlayer->getYVel() + PL_SPEED2);
				} else {
					currentPlayer->setYVel(currentPlayer->getYVel() + PL_SPEED1);
					currentPlayer->setYMom(yMom + 1);
				}
			} else if(!keystates[SDLK_UP]) {
				currentPlayer->setYMom(0);
			}

			//handle left movment
			if(keystates[SDLK_LEFT] && currentPlayer->getXPos() >= WINDOW_BUFF) {
				if(xMom < -momThresh) {
					currentPlayer->setXVel(currentPlayer->getXVel() - PL_SPEED2);
				} else {
					currentPlayer->setXVel(currentPlayer->getXVel() - PL_SPEED1);
					currentPlayer->setXMom(xMom - 1);
				}
			} else if(!keystates[SDLK_RIGHT]) {
				currentPlayer->setXMom(0);
			}

			//handle right movement
			if(keystates[SDLK_RIGHT] && currentPlayer->getXPos() <= (WINDOW_WIDTH - WINDOW_BUFF - currentPlayer->getSprite().w)) {
				if(xMom > momThresh) {
					currentPlayer->setXVel(currentPlayer->getXVel() + PL_SPEED2);
				} else {
					currentPlayer->setXVel(currentPlayer->getXVel() + PL_SPEED1);
					currentPlayer->setXMom(xMom + 1);
				}
			} else if(!keystates[SDLK_LEFT]) {
				currentPlayer->setXMom(0);
			}
			
			int xDeleted; //signals if first element should be deleted
			for(int x = 0; x < elements.size(); x++) { //for every element,
				xDeleted = 0; //first element should not be deleted (yet)
				GEType xType = elements[x]->getType(); //store that element's type
				if(xType == BULLET) { //if element is bullet
					if(elements[x]->update()) { //if bullet's updated status has a signal to process
						//delete bullet object and remove it from elements vector
						delete elements[x];
						elements.erase(std::remove(elements.begin(), elements.end(), elements[x]), elements.end());
						continue;
					}
				} else if(xType == ENEMY) { //if element is enemy
					if(elements[x]->update()) { //if enemy's updated status has a signal to process
						gunfire->volume=5;
						Mix_PlayChannel(-1, gunfire, 0); //play gunfire sound
						//fire enemy bullets
						elements.push_back(new Bullet((elements[x]->getXPos() + 16),	elements[x]->getYPos(), 0, BUL_SPEED, 0, 0));
						elements.push_back(new Bullet((elements[x]->getXPos() + 4),		elements[x]->getYPos(), 0, BUL_SPEED, 0, 0));
						//WHY DO WE NOT CONTINUE HERE? -- because we only continue when something is deleted
					}
				} else if(xType == EXPLOSION) { //if element is explosion
					if(elements[x]->update()) { //if explosion's updated status has a signal to process
						//delete explosion object and remove it from elements vector
						delete elements[x];
						elements.erase(std::remove(elements.begin(), elements.end(), elements[x]), elements.end());
						continue;
					}
				} else if (xType == PLAYER){
					if (elements[x]->getCycles() % 2 || playerIsDead){
						continue;
					}
					if (currentPlayer->getHealthCntr().getValue() <= 0){
						std::cout << "Dead" << std::endl;
						elements.push_back(new Explosion(elements[x]->getXPos(), elements[x]->getYPos()));
						delete elements[x];
						elements.erase(std::remove(elements.begin(), elements.end(), elements[x]), elements.end());
						currentPlayer = new Player((WINDOW_WIDTH / 2), (WINDOW_HEIGHT - 100));
						elements.push_back(currentPlayer);
						lives.increment(-1);
						playerIsDead = 1;
						Mix_PlayChannel(-1,guncock,0);
						continue;
					}
				} else if (xType == POWERUP){
					elements[x]->update();
				}
				for(int y = x + 1; y < elements.size(); y++) { //for every following element,
					GEType yType = elements[y]->getType(); //store that element's type
//moved code from here
					if(xType != yType) { //if the two elements are not of the same type
						if(checkCollide(elements[x], elements[y])) { //if the two objects collide,
							xDeleted = collide(elements[x], elements[y], xType, yType, &elements); //run collision between two items, remembering if the first element was deleted
							if(xDeleted == 1) { //if the first element was deleted,
								break; //stop checking that element against the other elements
							} else if(xDeleted == 0) {
							} else {
								std::cout << "Error: xDeleted not defined" << std::endl;
							}
						}
					}
				}
				
				//update screen positions
				elements[x]->setXPos(elements[x]->getXPos() + elements[x]->getXVel());
				elements[x]->setYPos(elements[x]->getYPos() + elements[x]->getYVel());
				applySurface(elements[x]->getXPos(),elements[x]->getYPos(), spriteSheet, screen, &elements[x]->getSprite());  
			}		     
			//extract temporary versions of ammo and health counters from player
			Counter tempAmmoCntr =		currentPlayer->getAmmoCntr();
			Counter tempHealthCntr =	currentPlayer->getHealthCntr();
			
			//render all counters
			ammoSurface =		tempAmmoCntr.render(		font, textColor);
			healthSurface =	tempHealthCntr.render(	font, textColor);
			livesSurface =		lives.render(				font, textColor);
			scoreSurface =		score.render(				font, textColor);
			
			//apply all labels to screen
			applySurface(tempAmmoCntr.getXPos(),	(tempAmmoCntr.getYPos() - 15),	ammoLabelSurface,		screen);
			applySurface(tempHealthCntr.getXPos(),	(tempHealthCntr.getYPos() - 15),	healthLabelSurface,	screen);
			applySurface(lives.getXPos(),				(lives.getYPos() - 15),				livesLabelSurface,	screen);
			applySurface(score.getXPos(),				(score.getYPos() - 15),				scoreLabelSurface,	screen);
			
			//apply all counters to screen
			applySurface(tempAmmoCntr.getXPos(),	tempAmmoCntr.getYPos(),		ammoSurface,	screen);
			applySurface(tempHealthCntr.getXPos(),	tempHealthCntr.getYPos(),	healthSurface,	screen);
			applySurface(lives.getXPos(),				lives.getYPos(),				livesSurface,	screen);
			applySurface(score.getXPos(),				score.getYPos(),				scoreSurface,	screen);
		
			if (maxShips == 0 && enemyCount == 0 && finishLevel){
				if(!levels.empty()){
					std::cout << "printing" << std::endl;
					applySurface(150,300,levelLabelSurface,screen);
					//applySurface(50, 350,spacePrompt,screen);
				}
				levelBreak = 1;
			}
		
			SDL_Flip(screen);
		} //end frame timing if()
	} //end while(gameRunning)
	cleanUp(); //free surfaces and quit SDL
}

	
void applySurface(int x, int y, SDL_Surface * src, SDL_Surface * dest, SDL_Rect * clip) {
	SDL_Rect offset; //make temporary rectangle to hold offsets
	
	//give offsets to rectangle
	offset.x = x;
	offset.y = y;
	
	SDL_BlitSurface(src, clip, dest, &offset); //blit clipped surface
}


int checkCollide(GraphElement * a, GraphElement * b) {
	//extract sprites of Elements to read width and height
	SDL_Rect aClip = a->getSprite();
	SDL_Rect bClip = b->getSprite();
	
	//calculate left, right, top, and bottom of elements
	int leftA =		a->getXPos();
	int leftB =		b->getXPos();
	int rightA =	leftA + aClip.w;
	int rightB =	leftB + bClip.w;
	int bottomA =	a->getYPos();
	int bottomB =	b->getYPos();
	int topA = 		bottomA + aClip.h;
	int topB = 		bottomB + bClip.h;

	if(leftA > rightB || rightA < leftB || bottomA > topB || topA < bottomB) { //if there is no way for Elements to overlap,
		return 0; // Elements do not overlap
	}
	return 1; // otherwise, Elements do overlap
}


void cleanUp() {
	//free surfaces
	SDL_FreeSurface(ammoSurface);
	SDL_FreeSurface(ammoLabelSurface);
	SDL_FreeSurface(bombNotification);
	SDL_FreeSurface(healthSurface);
	SDL_FreeSurface(healthLabelSurface);
	SDL_FreeSurface(levelLabelSurface);
	SDL_FreeSurface(livesSurface);
	SDL_FreeSurface(livesLabelSurface);
	SDL_FreeSurface(scoreSurface);
	SDL_FreeSurface(scoreLabelSurface);
	SDL_FreeSurface(spacePrompt);
	SDL_FreeSurface(splashScreen);
	SDL_FreeSurface(spriteSheet);
	
	TTF_CloseFont(font); //close font
	
	//free audio
	Mix_FreeMusic(music);
	Mix_FreeMusic(starship);
	Mix_FreeChunk(enemyDown);
	Mix_FreeChunk(fireAtWill);
	Mix_FreeChunk(guncock);
	Mix_FreeChunk(gunfire);
	
	TTF_Quit(); //quit SDL_ttf
	Mix_CloseAudio(); //quit audio
	SDL_Quit(); //quit SDL
}


int collide(GraphElement * GE1, GraphElement * GE2, GEType type1, GEType type2, std::vector<GraphElement *> * elemPtr) {
	int GE1Destroyed = 0;
	switch(type1) {
		case BULLET:
			switch(type2) {
				case BULLET:
					std::cout << "Error: Trying to collide two Bullet objects" << std::endl;
					break;
				case ENEMY:
					GE1Destroyed = collideBulletEnemy(1, GE1, GE2, elemPtr);
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					GE1Destroyed = collideBulletPlayer(1, GE1, GE2, elemPtr);
					break;
				case POWERUP:
					break;
				default:
					std::cout << "Error: Type of element[y] not defined" << std::endl;
					break;
			}
			break;
		case ENEMY:
			switch(type2) {
				case BULLET:
					GE1Destroyed = collideBulletEnemy(2, GE2, GE1, elemPtr);
					break;
				case ENEMY:
					std::cout << "Error: Trying to collide two Enemy objects" << std::endl;
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					GE1Destroyed = collideEnemyPlayer(1, GE1, GE2, elemPtr);
					break;
				case POWERUP:
					break;
				default:
					std::cout << "Error: Type of element[y] not defined" << std::endl;
					break;
			}		
			break;
		case EXPLOSION:
			switch(type2) {
				case BULLET:
					break;
				case ENEMY:
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					break;
				case POWERUP:
					std::cout << "Error: Trying to collide two Explosion objects" << std::endl;
					break;
				default:
					std::cout << "Error: Type of element[y] not defined" << std::endl;
					break;
			}
		case PLAYER:
			switch(type2) {
				case BULLET:
					GE1Destroyed = collideBulletPlayer(2, GE2, GE1, elemPtr);
					break;
				case ENEMY:
					GE1Destroyed = collideEnemyPlayer(2, GE2, GE1, elemPtr);
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					std::cout << "Error: Trying to collide two Player objects" << std::endl;
					break;
				case POWERUP:
					GE1Destroyed = collidePlayerPowerup(1, GE1, GE2, elemPtr);
					break;
				default:
					std::cout << "Error: Type of element[y] not defined" << std::endl;
					break;
			}			
			break;
		case POWERUP:
			switch(type2) {
				case BULLET:
					break;
				case ENEMY:
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					GE1Destroyed = collidePlayerPowerup(2, GE2, GE1, elemPtr);
					break;
				case POWERUP:
					std::cout << "Error: Trying to collide two powerups" << std::endl;
					break;
				default:
					std::cout << "Error: Type of element[y] not defined" << std::endl;
					break;
			}			
			break;
		default:
			std::cout << "Error: Type of element[x] not defined" << std::endl;
	}
	return GE1Destroyed;
}


int collideBulletEnemy(int xArg, GraphElement * b, GraphElement * e, std::vector<GraphElement *> * elemPtr) {
	int bDestroyed = 0; //bullet is not destroyed (yet)
	int eDestroyed = 0; //enemy is not not destroyed (yet)
	int origin = b->getOrigin(); //extract bullet's origin
	int powProb;

	// score.increment(10);

	srand(time(NULL));

	if(origin == 0) { //if origin of bullet was enemy
	
	} else if(origin == 1) { //if origin of bullet was player,
		//delete bullet object and remove it from elements vector
		if (!b->isBeam()){
			delete b;
			elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), b), elemPtr->end());
			bDestroyed = 1;
			score.increment(10);
		}
		score.increment(5);
		powProb = rand() % 7 + 1;
		//std::cout << powProb << std::endl;
		srand(time(NULL));
		if (powProb == 5) elemPtr->push_back(new Powerup(e->getXPos(), e->getYPos(), 1, 1, rand() % 4));//rand() % 4));

		elemPtr->push_back(new Explosion(e->getXPos(), e->getYPos())); //create explosion at site of enemy's death
		//delete enemy object and remove it from elements vector
		delete e;
		elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), e), elemPtr->end());
		eDestroyed = 1;
		enemyCount--;

		
	} else {
		std::cout << "Error in collideBulletEnemy: Bullet origin not defined" << std::endl;
	}
	
	//return correct xArg value
	if(xArg == 1) {
		return bDestroyed;
	} else if(xArg == 2) {
		return eDestroyed;
	} else {
		std::cout << "Error in collideBulletEnemy: xArg not defined" << std::endl;
		return -1;
	}
}


int collideBulletPlayer(int xArg, GraphElement * b, GraphElement * pl, std::vector<GraphElement *> * elemPtr) {
	int bDestroyed = 0; //bullet is not destroyed (yet)
	int plDestroyed = 0; //player is not destroyed (yet)
	int origin = b->getOrigin(); //extract bullet's origin
	
	if(origin == 1) { //if origin of bullet was player 
	
	}	else if(origin == 0) { //if origin of bullet was enemy,
		//delete bullets object and remove it from elements vector
		delete b;
		elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), b), elemPtr->end());
		bDestroyed = 1;
		
		pl->hitByBullet(); //notify player that it was hit by a bullet (decrement health a little)
	} else {
		std::cout << "Error in collideBulletPlayer: Bullet origin not defined" << std::endl;
	}
	
	//return correct xArg value
	if(xArg == 1) {
		return bDestroyed;
	} else if(xArg == 2) {
		return plDestroyed;
	} else {
		std::cout << "Error in collideBulletPlayer: xArg not defined" << std::endl;
		return -1;
	}
}


int collideEnemyPlayer(int xArg, GraphElement * e, GraphElement * pl, std::vector<GraphElement *> * elemPtr) {
	int eDestroyed = 0; //enemy is not destroyed (yet)
	int plDestroyed = 0; //player is not destroyed (yet)

	score.increment(15);
	std::cout << "enemyPlayer" << std::endl;
	
	elemPtr->push_back(new Explosion(e->getXPos(), e->getYPos())); //create explosion at site of enemy's death
	
	//delete enemy object and remove it from elements vector
	delete e;
	elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), e), elemPtr->end());
	eDestroyed = 1;
	
	pl->hitByPlane(); //notify player that it was hit by a plane (decrement health a lot)
	enemyCount--;
	//return correct xArg value
	if(xArg == 1) {
		return eDestroyed;
	} else if(xArg == 2) {
		return plDestroyed;
	} else {
		std::cout << "Error in collideEnemyPlayer: xArg not defined" << std::endl;
		return -1;
	}
}


int collidePlayerPowerup(int xArg, GraphElement * pl, GraphElement * po, std::vector<GraphElement *> * elemPtr) {
	int plDestroyed = 0; //player is not destroyed (yet)
	int poDestroyed = 0; //powerup is not destroyed (yet)
	score.increment(5);
	std::cout << "playerPowerup" << std::endl;
	switch(po->getPower()) {
		case 0: //if powerup is pow,
			pl->newAmmo(); //notify player that it gets more ammo
			break;
		case 1: //if powerup is spread,
			beamCycles += 10;
			pl->setWeapon(1);
			break;
		case 2: //if powerup is missile,
			pl->setWeapon(2);
			break;
		case 3: //if powerup is beam,
			beamCycles += 10;
			//std::cout << beamCycles << std::endl;
			pl->setWeapon(4);
			break;
		case 4: //if powerup is auto,
			
			break;
		case 5: //if powerup is cow,
			pl->newAmmo(); //notify player that it gets more ammo
			pl->newAmmo(); //notify player that it gets more ammo
			pl->newAmmo(); //notify player that it gets more ammo
			pl->newAmmo(); //notify player that it gets more ammo
			pl->newAmmo(); //notify player that it gets more ammo
			break;
		default:
			std::cout << "Error in collidePlayerPowerup: Powerup power not defined" << std::endl;
			break;
	}
	
	//delete powerup object and remove it from elements vector
	delete po;
	elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), po), elemPtr->end());
	poDestroyed = 1;
	
	//return correct xArg value
	if(xArg == 1) {
		return plDestroyed;
	} else if(xArg == 2) {
		return poDestroyed;
	} else {
		std::cout << "Error in collidePlayerPowerup: xArg not defined" << std::endl;
		return -1;
	}
}


int init() {
	//initialize all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return 1;
	}
		
	//set up screen
	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, SDL_SWSURFACE);
	if(!screen) {
		return 1;
	}
	
	//initialize SDL_ttf
	if(TTF_Init() == -1) {
		return 0;
	}

	//intialize audio
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
		return 0;
	}
	
	SDL_WM_SetCaption(WINDOW_TITLE.c_str(), NULL); //set window caption
	
	return 1; //if everything intialized fine, init() succeeded
}


int loadFiles() {
	//load enemyDown.wav (with possibility of erro
	enemyDown = Mix_LoadWAV("enemyDown.wav");
	if (!enemyDown) {
		std::cout << "Error: Could not load enemyDown.wav" << std::endl;
		return 0;
	}
	
	//load fireAtWill.wav (with possibility of error)
	fireAtWill = Mix_LoadWAV("fireAtWill.wav");
	if (!fireAtWill) {
		std::cout << "Error: Could not load fireAtWill.wav" << std::endl;
		return 0;
	}
	
	//load guncock.wav (with possibility of error)
	guncock = Mix_LoadWAV("guncock.wav");
	if (!guncock) {
		std::cout << "Error: Could not load guncock.wav" << std::endl;
		return 0;
	}
	
	//load gunfire.wav (with possibility of error)
	gunfire = Mix_LoadWAV("gunfire.wav");
	if(!gunfire) {
		std::cout << "Error: Could not load gunfire.wav" << std::endl;
		return 0;
	}
	
	//load joystix.ttf (with possibility of error)
	font = TTF_OpenFont("joystix.ttf", 18); 
	if(!font) {
		std::cout << "Error: Could not load joystix.ttf" << std::endl;
		return 0;
	}
	
	//load splash.png (with possibility of error)
	splashScreen = loadImage("splash.png");
	if(!splashScreen) {
		std::cout << "Error: Could not load splash.png" << std::endl;
		return 0;
	}
	
	//load sprites.png (with possibility of error)
	spriteSheet = loadImage("sprites.png");
	if(!spriteSheet) {
		std::cout << "Error: Could not load sprites.png" << std::endl;
		return 0;
	}
	
	//load starship.wav (with possibility of error)
	starship = Mix_LoadMUS("starship.wav");
	if(!starship) {
		std::cout << "Error: Could not load starship.wav" << std::endl;
		return 0;
	}
	
	return 1; //if all files loaded fine, loadFiles() succeeded
}


SDL_Surface * loadImage(std::string filename) {
	SDL_Surface * loadedImage = NULL; //temporary storage for loaded image
	SDL_Surface * optImage = NULL; //optimized image that will be used
	
	loadedImage = IMG_Load(filename.c_str()); //load image
	if(loadedImage) { //if image loads correctly,
		optImage = SDL_DisplayFormat(loadedImage); //create optimized image
		SDL_FreeSurface(loadedImage); //free old image
		if(optImage) { //if image optimized correctly,
			Uint32 colorkey = SDL_MapRGB(optImage->format, keyColor.r, keyColor.g, keyColor.b); //map color key
			SDL_SetColorKey(optImage, SDL_SRCCOLORKEY, colorkey); //set all pixels of color R0, G0xFF, B0xFF, to be transparent
		}
	}
	return optImage; //return optimized image
}


double sinDeg(double deg) {
	double rad = deg * (M_PI / 180.0);
	return sin(rad);
}


double cosDeg(double deg) {
	double rad = deg * (M_PI / 180.0);
	return cos(rad);
}
