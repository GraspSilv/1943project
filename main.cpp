/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
main.cpp
	Driver for Blitz game
*/
#include<algorithm>
#include<iostream>
#include<string>
#include"SDL/SDL.h"
#include"SDL/SDL_image.h"
#include"SDL/SDL_ttf.h"
#include"SDL/SDL_mixer.h"
#include"Background.h"
#include"Bullet.h"
#include"Counter.h"
#include"Enemy.h"
#include"Explosion.h"
#include"GraphElement.h"
#include"Player.h"
#include"Powerup.h"
#include"Timer.h"

const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 640;
const int WINDOW_BPP = 32;
const int WINDOW_BUFF = 5;
const std::string WINDOW_TITLE = "Blitz";

const int GAME_FPS = 80;

const int playerSpeed1 = 2;
const int playerSpeed2 = 4;
const int playerSpeed3 = 6;
const int momThresh = 600;

SDL_Surface * spriteSheet = NULL;
SDL_Surface * screen = NULL;
SDL_Surface * ammoSurface = NULL;
SDL_Surface * healthSurface = NULL;
SDL_Surface * scoreSurface = NULL;

TTF_Font * font = NULL;
Mix_Music * music = NULL;
Mix_Chunk * gunfire = NULL;

SDL_Color backgroundColor = {0, 0, 0};
SDL_Color keyColor = {0, 0x2A, 0x88};
SDL_Color textColor = {255, 255, 255};

Counter score(5, (WINDOW_HEIGHT - 100), 0, 0, 1000000, 1);

void applySurface(int x, int y, SDL_Surface * src, SDL_Surface * dest, SDL_Rect * clip = NULL);
void cleanUp();
int checkCollide(GraphElement * a, GraphElement * b);
int collide(GraphElement * GE1, GraphElement * GE2, GEType type1, GEType type2, std::vector<GraphElement *> * elemPtr);
//the following collide functions deal with consequences for both objects, including whether the operand specified by the first argument or second argument (1 or 2) was destroyed
int collideBulletEnemy(int xArg, GraphElement * b, GraphElement * e, std::vector<GraphElement *> * elemPtr);
int collideBulletPlayer(int xArg, GraphElement * b, GraphElement * pl, std::vector<GraphElement *> * elemPtr);
int collideEnemyPlayer(int xArg, GraphElement * e, GraphElement * pl, std::vector<GraphElement *> * elemPtr);
int collidePlayerPowerup(int xArg, GraphElement * pl, GraphElement * po, std::vector<GraphElement *> * elemPtr);
int init();
int loadFiles();
SDL_Surface * loadImage(std::string filename);


int main(int argc, char * argv[]) {
	int gameRunning = 1;
	
	//initialize
	if(!init()) {
		return 1;
	}
		
	//load files
	if(!loadFiles()) {
		return 1;
	}

	SDL_Event event; //event structure for checking if user has exited and generating bullets
	std::vector<GraphElement *> elements; //vector of pointers to all graphic elements
	
	Background bg("testback.png");
	int bgX = 0, bgY = 0;

	//initialize background
	if(!bg.init()) {
		return 1;
	}

	//load background files
	if(!bg.load_files()) {
		return 1;
	}

	int nLives = 10;
	Player * currentPlayer = new Player((WINDOW_WIDTH / 2), (WINDOW_HEIGHT - 100));
	elements.push_back(currentPlayer);
	
	//create test enemies and powerup
	elements.push_back(new Enemy(200, -50, RED));
	elements.push_back(new Enemy(280, -50, RED));
	elements.push_back(new Powerup(200, 200, 1, 1, COWP));
	elements.push_back(new Explosion(300, 300));
	
	Timer gameTimer;
	gameTimer.start();
	int frameTime = 1000 / GAME_FPS;
	
	if (Mix_PlayMusic(music,5) == -1){
		return 1;
	}
	
	while(gameRunning) {
		//reset player's velocity
		currentPlayer->setXVel(0);
		currentPlayer->setYVel(0);
		
		if((gameTimer.get_ticks() % frameTime) == 0) { //if enough time has passed to create a new frame,
			bgY += 1;
			if(bgY >= bg.background->h) {//if background has scrolled too far,
				bgY = 0; //reset the offset
			}

			//show background
			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
			bg.apply_surface(bgX, bgY, bg.background, bg.screen);
			bg.apply_surface(bgX, bgY - bg.background->h, bg.background, bg.screen);
			
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
							Mix_PlayChannel(-1, gunfire, 0); //play gunfire sound
							elements.push_back(new Bullet((currentPlayer->getXPos() + 16), currentPlayer->getYPos(), 0, -4, 1)); //create left bullet
							elements.push_back(new Bullet((currentPlayer->getXPos() + 5), currentPlayer->getYPos(), 0, -4, 1)); //create right bullet
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

			if(keystates[SDLK_UP] && currentPlayer->getYPos() >= WINDOW_BUFF) { //if up is pressed and player is not about to fly off top of screen,
				if(yMom < -600) { //if y-momentum is significant,
					currentPlayer->setYVel(currentPlayer->getYVel() - playerSpeed2); //make the y-velocity be 4 pixels/frame upwards (greater than it currently is, 0)
				} else { //otherwise,
					currentPlayer->setYMom(yMom - 1); //give player more y-momentum upwards
					currentPlayer->setYVel(currentPlayer->getYVel() - playerSpeed1); //make the y-velocity be 2 pixels/frame upwards (greater than it currently is, 0)
				}
			} else if (!keystates[SDLK_DOWN]) { //if down is pressed when up is not pressed,
				currentPlayer->setYMom(0); //reset player's y-momentum
			}

			if(keystates[SDLK_DOWN] && currentPlayer->getYPos() <= (WINDOW_HEIGHT - WINDOW_BUFF - currentPlayer->getSprite().h)) {
				if(yMom > 600) {
					currentPlayer->setYVel(currentPlayer->getYVel() + 4);
				} else {
					currentPlayer->setYVel(currentPlayer->getYVel() + 2);
					currentPlayer->setYMom(yMom + 1);
				}
			} else if(!keystates[SDLK_UP]) {
				currentPlayer->setYMom(0);
			}

			if(keystates[SDLK_LEFT] && currentPlayer->getXPos() >= WINDOW_BUFF) {
				if(xMom < -600) {
					currentPlayer->setXVel(currentPlayer->getXVel() - 4);
				} else {
					currentPlayer->setXVel(currentPlayer->getXVel() - 2);
					currentPlayer->setXMom(xMom - 1);
				}
			} else if(!keystates[SDLK_RIGHT]) {
				currentPlayer->setXMom(0);
			}

			if(keystates[SDLK_RIGHT] && currentPlayer->getXPos() < 460) {
				if(xMom > 600) {
					currentPlayer->setXVel(currentPlayer->getXVel() + 4);
				} else {
					currentPlayer->setXVel(currentPlayer->getXVel() + 2);
					currentPlayer->setXMom(xMom + 1);
				}
			} else if(!keystates[SDLK_LEFT]) {
				currentPlayer->setXMom(0);
			}
			
			//render all counters
			ammoSurface = currentPlayer->ammo.render(font, textColor);
			healthSurface = currentPlayer->health.render(font, textColor);
			scoreSurface = score.render(font, textColor);
			
			//apply all counters to screen
			applySurface(currentPlayer->ammo.getXPos(), currentPlayer->ammo.getYPos(), ammoSurface, screen);
			applySurface(currentPlayer->health.getXPos(), currentPlayer->health.getYPos(), healthSurface, screen);
			applySurface(score.getXPos(), score.getYPos(), scoreSurface, screen);
			
			for (int x = 0; x < elements.size(); x++) { //for every element,
				GEType xType = elements[x]->getType();
				for (int y = x + 1; y < elements.size(); y++) { //for every following element,
					GEType yType = elements[y]->getType();
					if(xType != yType) {
						if(checkCollide(elements[x], elements[y])) { //if the two objects collide,
							int xDeleted = collide(elements[x], elements[y], xType, yType, &elements);
							if(xDeleted) {
								break;
							}
						}
					}
				}
				//if(elements[x]->isDone()) {
					//delete elements[x];
					//elements.erase(std::remove(elements.begin(), elements.end(), elements[x]), elements.end());
					//break;
				//}
				elements[x]->setXPos(elements[x]->getXPos() + elements[x]->getXVel());
				elements[x]->setYPos(elements[x]->getYPos() + elements[x]->getYVel());
				applySurface(elements[x]->getXPos(),elements[x]->getYPos(), spriteSheet, screen, &elements[x]->getSprite());  
			}		     
			SDL_Flip(screen);
		} //end frame timing if()
	} //end while(gameRunning)
	SDL_FreeSurface(scoreSurface);
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
	SDL_Rect aClip = a->getSprite();
	SDL_Rect bClip = b->getSprite();
	
	int leftA = a->getXPos();
	int leftB = b->getXPos();
	int rightA = leftA + aClip.w;
	int rightB = leftB + bClip.w;
	int bottomA = a->getYPos();
	int bottomB = b->getYPos();
	int topA = bottomA + aClip.h;
	int topB = bottomB + bClip.h;

	if(leftA >= rightB || rightA <= leftB || bottomA >= topB || topA <= bottomB) {
		return 0;
	}
	return 1;
}


void cleanUp() {
	SDL_FreeSurface(spriteSheet); //free surface
	TTF_CloseFont(font); //close font
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	TTF_Quit(); //quit SDL_ttf
	SDL_Quit(); //quit SDL

}


int collide(GraphElement * GE1, GraphElement * GE2, GEType type1, GEType type2, std::vector<GraphElement *> * elemPtr) {
	int GE1Destroyed = 0;
	switch(type1) {
		case BULLET:
			switch(type2) {
				case BULLET:
					std::cout << "Error: Trying to collide two bullets" << std::endl;
					break;
				case ENEMY:
					if (GE1->getOrigin()){
						GE1Destroyed = collideBulletEnemy(1, GE1, GE2, elemPtr);
						score.increment(100);
					}
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					if (GE1->getOrigin() == 0){
						GE1Destroyed = collideBulletPlayer(1, GE1, GE2, elemPtr);
					}
					break;
				case POWERUP:
					break;
				default:
					std::cout << "Error: type of element[y] not defined" << std::endl;
					break;
			}
			break;
		case ENEMY:
			switch(type2) {
				case BULLET:
					if (GE2->getOrigin()){
						GE1Destroyed = collideBulletEnemy(2, GE2, GE1, elemPtr);
						score.increment(100);
					}
					break;
				case ENEMY:
					std::cout << "Error: Trying to collide two enemies" << std::endl;
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					GE1Destroyed = collideEnemyPlayer(1, GE1, GE2, elemPtr);
					break;
				case POWERUP:
					break;
				default:
					std::cout << "Error: type of element[y] not defined" << std::endl;
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
					std::cout << "Error: Trying to collide two explosions" << std::endl;
					break;
				default:
					std::cout << "Error: type of element[y] not defined" << std::endl;
					break;
			}
		case PLAYER:
			switch(type2) {
				case BULLET:
					if (GE2->getOrigin() == 0){
						GE1Destroyed = collideBulletPlayer(2, GE2, GE1, elemPtr);
					}
					break;
				case ENEMY:
					GE1Destroyed = collideEnemyPlayer(2, GE2, GE1, elemPtr);
					break;
				case EXPLOSION:
					break;
				case PLAYER:
					std::cout << "Error: Trying to collide two players" << std::endl;
					break;
				case POWERUP:
					GE1Destroyed = collidePlayerPowerup(1, GE1, GE2, elemPtr);
					break;
				default:
					std::cout << "Error: type of element[y] not defined" << std::endl;
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
					std::cout << "Error: type of element[y] not defined" << std::endl;
					break;
			}			
			break;
		default:
			std::cout << "Error: type of element[x] not defined" << std::endl;
	}
	return GE1Destroyed;
}


int collideBulletEnemy(int xArg, GraphElement * b, GraphElement * e, std::vector<GraphElement *> * elemPtr) {
	int bDestroyed = 0;
	int eDestroyed = 0;
	
	delete b;
	elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), b), elemPtr->end());
	bDestroyed = 1;
	
	elemPtr->push_back(new Explosion(e->getXPos(), e->getYPos()));
	delete e;
	elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), e), elemPtr->end());
	eDestroyed = 1;
	
	
	if(xArg == 1) {
		return bDestroyed;
	} else if(xArg == 2) {
		return eDestroyed;
	} else {
		std::cout << "Error in collideBulletEnemy" << std::endl;
	}
}


int collideBulletPlayer(int xArg, GraphElement * b, GraphElement * pl, std::vector<GraphElement *> * elemPtr) {
	int bDestroyed = 0;
	int plDestroyed = 0;
	
	if(1) {
		delete b;
		elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), b), elemPtr->end());
		bDestroyed = 1;
		
		//pl->sub1_IncHealth();
	}
	
	if(xArg == 1) {
		return bDestroyed;
	} else if(xArg == 2) {
		return plDestroyed;
	} else {
		std::cout << "Error in collideBulletPlayer" << std::endl;
	}
}


int collideEnemyPlayer(int xArg, GraphElement * e, GraphElement * pl, std::vector<GraphElement *> * elemPtr) {
	int eDestroyed = 0;
	int plDestroyed = 0;
	
	delete e;
	elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), e), elemPtr->end());
	eDestroyed = 1;
	//pl->sub1_IncHealth();
	
	if(xArg == 1) {
		return eDestroyed;
	} else if(xArg == 2) {
		return plDestroyed;
	} else {
		std::cout << "Error in collideEnemyPlayer" << std::endl;
	}
}


int collidePlayerPowerup(int xArg, GraphElement * pl, GraphElement * po, std::vector<GraphElement *> * elemPtr) {
	int plDestroyed = 0;
	int poDestroyed = 0;
	
	delete po;
	elemPtr->erase(std::remove(elemPtr->begin(), elemPtr->end(), po), elemPtr->end());
	poDestroyed = 1;
	//pl->add4_IncHealth();
	
	if(xArg == 1) {
		return plDestroyed;
	} else if(xArg == 2) {
		return poDestroyed;
	} else {
		std::cout << "Error in collidePlayerPowerup" << std::endl;
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

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
		return 0;
	}
	
	SDL_WM_SetCaption(WINDOW_TITLE.c_str(), NULL); //set window caption
	
	return 1; //if everything intialized fine
}


int loadFiles(){
	//open font
	font = TTF_OpenFont("arial.ttf", 28); 
	if(!font) {
		return 0;
	}
	
	//load spriteSheet
	spriteSheet = loadImage("sprites.png");
	if(!spriteSheet) {
		std::cout << "Could not load sprites.png" << std::endl;
		return 0;
	}
	music = Mix_LoadMUS("starship.wav");
	gunfire = Mix_LoadWAV("gunfire.wav");

	if (music == NULL || gunfire == NULL){
		std::cout << "Could not load sounds." << std::endl;
		return 0;
	}

	
	return 1;
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
