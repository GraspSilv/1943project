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
#include"Counter.h"
#include"GraphElement.h"
#include "Player.h"
#include "Bullet.h"
#include "Powerup.h"
#include "Enemy.h"

//screen attributes
const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 640;
const int WINDOW_BPP = 32;
const std::string WINDOW_TITLE = "Blitz";

//surfaces that will be used
SDL_Surface * spriteSheet = NULL;
SDL_Surface * screen = NULL;

TTF_Font * font; //font that will be used

//colors that will be used
SDL_Color backgroundColor = {0, 0, 0};
SDL_Color keyColor = {0, 0x2A, 0x88};
SDL_Color textColor = {255, 255, 255};


void applySurface(int x, int y, SDL_Surface * src, SDL_Surface * dest, SDL_Rect * clip = NULL);
void cleanUp();
bool checkCollide(SDL_Rect A, SDL_Rect B);
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

	Counter score;
	score.increment(5);
    
	// Pass zero for width and height to draw the whole surface
	SDL_Rect scoreLocation = {5, (WINDOW_HEIGHT - 100), 0, 0};

	SDL_Event event;
	
	SDL_Surface * scoreSurface = score.render(font, textColor);

	std::vector<GraphElement *> elements;	

	Player newPlayer(0,0);
	Powerup newPowerUp(100,100,COWP);
	newPowerUp.setYVel(.1);

	
	elements.push_back(new Enemy(200, 200, red));
	elements.push_back(new Enemy(200, 250, red));
	elements.push_back(new Enemy(200, 150, red));
	elements.push_back(new Enemy(200, 100, red));
	elements.push_back(new Enemy(200, 300, red));
	elements.push_back(&newPowerUp);
	elements.push_back(&newPlayer);
	
	while (gameRunning) {
		newPlayer.setXVel(0);
		newPlayer.setYVel(0);
		scoreSurface = score.render(font, textColor);
		
		if(SDL_PollEvent(&event)) { //can change to while to make faster?
			if(event.type == SDL_QUIT) {
				gameRunning = 0;
			}
			else if(event.type == SDL_KEYDOWN) {
				SDLKey keyPressed = event.key.keysym.sym;
				switch (keyPressed){
					case SDLK_ESCAPE:
						gameRunning = false;
						break;
					case SDLK_z:
						if (Bullet::count < 8){
							elements.push_back(new Bullet(newPlayer.getXPos()+16,newPlayer.getYPos(),0,-.5));
							elements.push_back(new Bullet(newPlayer.getXPos()+5,newPlayer.getYPos(),0,-.5));
							score.increment(1);
						}
						break;
					case SDLK_x:
						score.increment(10);

						break;
					default:
						break;
				}
			}
		}

		//Begin testing movement
		Uint8 * keystates = SDL_GetKeyState(NULL);

		int xm = newPlayer.getXMom();
		int ym = newPlayer.getYMom();

		if(keystates[SDLK_UP] && newPlayer.getYPos() > 1) {
			if (ym < -1000){
				newPlayer.setYVel(newPlayer.getYVel() - .2);
			} else {
				newPlayer.setYMom(ym - 1);
				newPlayer.setYVel(newPlayer.getYVel() - .1);
			}
		} else if (!keystates[SDLK_DOWN]) {
			newPlayer.setYMom(0);
		}

		if(keystates[SDLK_DOWN] && newPlayer.getYPos() < (WINDOW_HEIGHT - 10)) {
			if (ym > 1000){
				newPlayer.setYVel(newPlayer.getYVel() + .2);
			} else {
				newPlayer.setYVel(newPlayer.getYVel() + .1);
				newPlayer.setYMom(ym + 1);
			}
		} else if (!keystates[SDLK_UP]) {
			newPlayer.setYMom(0);
		}

		if(keystates[SDLK_LEFT] && newPlayer.getXPos() > 1) {
			if (xm < -1000){
				newPlayer.setXVel(newPlayer.getXVel() - .2);
			} else {
				newPlayer.setXVel(newPlayer.getXVel() - .1);
				newPlayer.setXMom(xm - 1);
			}
		} else if (!keystates[SDLK_RIGHT]) {
			newPlayer.setXMom(0);
		}

		if(keystates[SDLK_RIGHT] && newPlayer.getXPos() < (WINDOW_WIDTH - 10)) {
			if (xm > 1000){
				newPlayer.setXVel(newPlayer.getXVel() + .2);
			} else {
				newPlayer.setXVel(newPlayer.getXVel() + .1);
				newPlayer.setXMom(xm + 1);
			}
		} else if (!keystates[SDLK_LEFT]) {
			newPlayer.setXMom(0);
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
        
		SDL_BlitSurface(scoreSurface, NULL, screen, &scoreLocation);
	


		for (int x = 0; x < elements.size(); x++){
			//THIS IS OUR LOOP FOR EVERYTHING
			if (elements[x]->getYPos() < 0 && elements[x]->getType() == BULLET){
				delete elements[x];
				elements.erase(elements.begin()+x);
				continue;
			}

								//maybe change this V to (y < x) for efficiency?
			for (int y = 0; y < x; y++){
				if (elements[x]->getType() == elements[y]->getType()) continue;
				// std::cout << "checking" << std::endl;
				if (x != y && checkCollide(elements[x]->getSprite(), elements[y]->getSprite()) == true){
					// std::cout << "Colliding!" << std::endl;
				}
			}

			elements[x]->setXPos(elements[x]->getXPos()+elements[x]->getXVel());
			elements[x]->setYPos(elements[x]->getYPos()+elements[x]->getYVel());
			applySurface(elements[x]->getXPos(),elements[x]->getYPos(),spriteSheet, screen, &elements[x]->getSprite());        
		}
			
       SDL_Flip(screen);
    }
    
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

bool checkCollide(SDL_Rect A, SDL_Rect B){
	int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;

	if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB){
		return false;
	}
	return true;

}


void cleanUp() {
	SDL_FreeSurface(spriteSheet); //free surface
	TTF_CloseFont(font); //close font
	TTF_Quit(); //quit SDL_ttf
	SDL_Quit(); //quit SDL
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
