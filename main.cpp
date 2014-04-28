/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
main.cpp
	Driver for Blitz game
History
	03/26/14	Jack Magiera	Create, insert basic structure from Lazy Foo' SDL Tutorials
	04/08/14	Jack Magiera	Add functionality to adjust counter using arrow keys
	04/08/14	Jack Magiera	Implement simply flying plane sprite (does not use classes)
	04/14/14	Jack Magiera	Edit to make compatible with GraphElement
	04/16/14	Jon Richelsen	Start standardization
	04/22/14 Jon Richelsen	Add default case to counter case structure, continue standardization,
To Do
*/
#include<iostream>
#include<string>
#include"SDL/SDL.h"
#include"SDL/SDL_image.h"
#include"SDL/SDL_ttf.h"
#include"Bullet.h"
#include"Counter.h"
#include"Enemy.h"
#include"GraphElement.h"
#include"Player.h"
#include"Powerup.h"
#include"Timer.h"

const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 640;
const int WINDOW_BPP = 32;
const std::string WINDOW_TITLE = "Blitz";

const int GAME_FPS = 1;

SDL_Surface * spriteSheet = NULL;
SDL_Surface * screen = NULL;

TTF_Font * font;

SDL_Color backgroundColor = {0, 0, 0};
SDL_Color keyColor = {0, 0x2A, 0x88};
SDL_Color textColor = {255, 255, 255};


void applySurface(int x, int y, SDL_Surface * src, SDL_Surface * dest, SDL_Rect * clip = NULL);
void cleanUp();
//the following functions deal with consequences for both objects, including whether the operand specified by the first argument (0 or 1) was destroyed
int collideBulletBullet(int main, Bullet * b1, Bullet * b2, std::vector<GraphElement *> * elemPtr);
int collideBulletEnemy(Bullet * b, Enemy * e, std::vector<GraphElement *> * elemPtr);
int collideBulletPlayer(Bullet * b, Player * pl, std::vector<GraphElement *> * elemPtr);
int collideBulletPowerup(Bullet * b, Powerup * po, std::vector<GraphElement *> * elemPtr);
int collideEnemyEnemy(Enemy * e1, Enemy * e2, std::vector<GraphElement *> * elemPtr);
int collideEnemyPlayer(Enemy * e, Player * pl, std::vector<GraphElement *> * elemPtr);
int collideEnemyPowerup(Enemy * e, Powerup * po, std::vector<GraphElement *> * elemPtr);
int collidePlayerPlayer(Player * pl1, Player * pl2, std::vector<GraphElement *> * elemPtr);
int collidePlayerPowerup(Player * pl, Powerup * po, std::vector<GraphElement *> * elemPtr);
int collidePowerupPowerup(Powerup * po1, Powerup * po2, std::vector<GraphElement *> * elemPtr);
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

	SDL_Event event; //event structure for all user input
	std::vector<GraphElement *> elements; //vector of pointers to all graphic elements
	
	//create and initialize score and health counters
	Counter score(5, (WINDOW_HEIGHT - 100), 0, 0, 1000000, 1);
	Counter health(5, (WINDOW_HEIGHT - 50), 100, 0, 100, -5);
	
	SDL_Surface * scoreSurface = score.render(font, textColor);
	SDL_Surface * healthSurface = health.render(font, textColor);

	Player * currentPlayer = new Player(0,0);
	elements.push_back(currentPlayer);

	elements.push_back(new Enemy(200, 200, RED));
	elements.push_back(new Enemy(200, 250, RED));
	elements.push_back(new Enemy(200, 150, RED));
	elements.push_back(new Enemy(200, 100, RED));
	elements.push_back(new Enemy(200, 300, RED));
	elements.push_back(new Powerup(200, 200, 0.1, 0.1, COWP));
	
	Timer gameTimer;
	int frameTime = 1000 / GAME_FPS;
	
	while(gameRunning) {
		//reset player's velocity
		currentPlayer->setXVel(0);
		currentPlayer->setYVel(0);
		
		if((gameTimer.get_ticks() % frameTime) == 0) { //if enough time has passed to create a new frame,
			//render all counters
			scoreSurface = score.render(font, textColor);
			healthSurface = health.render(font, textColor);
		
			if(SDL_PollEvent(&event)) { //if there is event to handle, (can change to while to make faster?)
				if(event.type == SDL_QUIT) { //if user has Xed out of window,
					gameRunning = 0; //quit game
				}
				else if(event.type == SDL_KEYDOWN) { //else, if user pressed key
					switch (event.key.keysym.sym){ //inspect which key was pressed
						case SDLK_ESCAPE:
							gameRunning = 0;
							break;
						case SDLK_z:
							if(Bullet::count < 8) { //(why is this variable public?)
								elements.push_back(new Bullet((currentPlayer->getXPos() + 16), currentPlayer->getYPos(), 0, -0.5));
								elements.push_back(new Bullet((currentPlayer->getXPos() + 5), currentPlayer->getYPos(), 0, -0.5));
								score.increment(1);
							}
							break;
						default:
							break;
					}
				}
			}

			//begin testing movement
			Uint8 * keystates = SDL_GetKeyState(NULL);

			int xMom = currentPlayer->getXMom();
			int yMom = currentPlayer->getYMom();

			if(keystates[SDLK_UP] && currentPlayer->getYPos() > 1) {
				if(yMom < -1000) {
					currentPlayer->setYVel(currentPlayer->getYVel() - .2);
				} else {
					currentPlayer->setYMom(yMom - 1);
					currentPlayer->setYVel(currentPlayer->getYVel() - .1);
				}
			} else if (!keystates[SDLK_DOWN]) {
				currentPlayer->setYMom(0);
			}

			if(keystates[SDLK_DOWN] && currentPlayer->getYPos() < 620) {
				if(yMom > 1000) {
					currentPlayer->setYVel(currentPlayer->getYVel() + .2);
				} else {
					currentPlayer->setYVel(currentPlayer->getYVel() + .1);
					currentPlayer->setYMom(yMom + 1);
				}
			} else if(!keystates[SDLK_UP]) {
				currentPlayer->setYMom(0);
			}

			if(keystates[SDLK_LEFT] && currentPlayer->getXPos() > 1) {
				if(xMom < -1000) {
					currentPlayer->setXVel(currentPlayer->getXVel() - .2);
				} else {
					currentPlayer->setXVel(currentPlayer->getXVel() - .1);
					currentPlayer->setXMom(xMom - 1);
				}
			} else if(!keystates[SDLK_RIGHT]) {
				currentPlayer->setXMom(0);
			}

			if(keystates[SDLK_RIGHT] && currentPlayer->getXPos() < 460) {
				if(xMom > 1000) {
					currentPlayer->setXVel(currentPlayer->getXVel() + .2);
				} else {
					currentPlayer->setXVel(currentPlayer->getXVel() + .1);
					currentPlayer->setXMom(xMom + 1);
				}
			} else if(!keystates[SDLK_LEFT]) {
				currentPlayer->setXMom(0);
			}

			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
        
			applySurface(score.getXPos(), score.getYPos(), scoreSurface, screen);
			applySurface(health.getXPos(), health.getYPos(), healthSurface, screen);

			
			//HERE BEGINS JON'S COLLISION STUFF
			for (int x = 0; x < elements.size(); x++) { //for every element,
				GEType xType = elements[x]->getType();
				for (int y = x + 1; y < elements.size(); y++) { //for every following element,
					if(checkCollide(elements[x], elements[y])) { //if the two objects collide,
						GEType yType = elements[y]->getType();
						switch(xType) {
							case BULLET:
								switch(yType) {
									case BULLET: //BULLET and BULLET
										xDestroyed = collideBulletBullet(elements[x], elements[y])
										break;
									case ENEMY: //BULLET and ENEMY
										collideBulletEnemy
										break;
									case PLAYER: //BULLET and PLAYER
										collideBulletPlayer
										break;
									case POWERUP: //BULLET and POWERUP
										collideBulletPowerup
										break;
									default:
										std::cout << "Error: type of element[y] not defined"
										break;
								}
								break;
							case ENEMY:
								switch(yType) {
									case BULLET: //ENEMY and BULLET (ALREADY DEFINED! COPY FROM ABOVE!)
										break;
									case ENEMY: //ENEMY and ENEMY
										break;
									case PLAYER: //ENEMY and PLAYER
										break;
									case POWERUP: //ENEMY and POWERUP
										break;
									default:
										std::cout << "Error: type of element[y] not defined"
										break;
								}								
								break;
							case PLAYER:
								switch(yType) {
									case BULLET: //PLAYER and 
										break;
									case ENEMY: //PLAYER and 
										break;
									case PLAYER: //PLAYER and 
										break;
									case POWERUP: //PLAYER and 
										break;
									default:
										std::cout << "Error: type of element[y] not defined"
										break;
								}								
								break;
							case POWERUP:
								switch(yType) {
									case BULLET: //BULLET and
										break;
									case ENEMY: //BULLET and
										break;
									case PLAYER: //BULLET and
										break;
									case POWERUP: //BULLET and
										break;
									default:
										std::cout << "Error: type of element[y] not defined"
										break;
								}								
								break;
							default:
								std::cout << "Error: type of element[x] not defined"
						}
					}
				}
			}
			//HERE ENDS JON'S COLLISION STUFF
			
			for (int x = 0; x < elements.size(); x++){
				int toErase = 0;
				//THIS IS OUR LOOP FOR EVERYTHING
				// Lot of really important stuff goes here

				
				if (elements[x]->getYPos() < 0 && elements[x]->getType() == BULLET){
					delete elements[x];
					elements.erase(elements.begin()+x);
					continue;
				}

								//maybe change this V to (y < x) for efficiency?
				for (int y = 0; y < x; y++){
					if (elements[x]->getType() == elements[y]->getType()) continue;
					// std::cout << "checking" << std::endl;
					if (x != y && checkCollide(elements[x], elements[y]) == true){
						//std::cout << "Colliding!" << std::endl;
						if (elements[x]->getType() == BULLET){
							if (elements[y]->getType() == ENEMY){
								delete elements[y];
								elements.erase(elements.begin()+y);
								std::cout << "okay 1" << std::endl;
								toErase = true;
								break;
							}
						}
					}
				}
				if (toErase){
					std::cout << "okay 2" << std::endl;
					//delete elements[x];
					std::cout << "okay 3" << std::endl;
	
					elements.erase(elements.begin()+x);
					std::cout << "okay 4" << std::endl;
					break;
				}
	
				elements[x]->setXPos(elements[x]->getXPos()+elements[x]->getXVel());
				elements[x]->setYPos(elements[x]->getYPos()+elements[x]->getYVel());
				applySurface(elements[x]->getXPos(),elements[x]->getYPos(),spriteSheet, screen, &elements[x]->getSprite());        
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
		return 0;switch(xType) {
							case BULLET:
								break;
							case ENEMY:
								break;
							case PLAYER:
								break;
							case POWERUP:
								break;
							default:
								std::cout << "Error: type of element[x] not defined"
	}
	return true;
}


void cleanUp() {
	SDL_FreeSurface(spriteSheet); //free surface
	TTF_CloseFont(font); //close font
	TTF_Quit(); //quit SDL_ttf
	SDL_Quit(); //quit SDL
}


int collideBulletBullet(Bullet * b1, Bullet * b2, std::vector<GraphElement *> * elemPtr) {

}


int collideBulletEnemy(Bullet * b, Enemy * e, std::vector<GraphElement *> * elemPtr) {

}


int collideBulletPlayer(Bullet * b, Player * pl, std::vector<GraphElement *> * elemPtr) {

}


int collideBulletPowerup(Bullet * b, Powerup * po, std::vector<GraphElement *> * elemPtr) {

}


int collideEnemyEnemy(Enemy * e1, Enemy * e2, std::vector<GraphElement *> * elemPtr) {

}


int collideEnemyPlayer(Enemy * e, Player * pl, std::vector<GraphElement *> * elemPtr) {

}


int collideEnemyPowerup(Enemy * e, Powerup * po, std::vector<GraphElement *> * elemPtr) {

}


int collidePlayerPlayer(Player * pl1, Player * pl2, std::vector<GraphElement *> * elemPtr) {

}


int collidePlayerPowerup(Player * pl, Powerup * po, std::vector<GraphElement *> * elemPtr) {

}


int collidePowerupPowerup(Powerup * po1, Powerup * po2, std::vector<GraphElement *> * elemPtr) {

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
		std::cerr << "Could not load sprites.png" << std::endl;
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
