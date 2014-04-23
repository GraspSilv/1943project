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
						score.increment(1);
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
		if(keystates[SDLK_UP] && newPlayer.getYPos() > 1) {
			newPlayer.setYVel(newPlayer.getYVel() - .1);
		}
		if(keystates[SDLK_DOWN] && newPlayer.getYPos() < (WINDOW_HEIGHT - 10)) {
			newPlayer.setYVel(newPlayer.getYVel() + .1);
		}
		if(keystates[SDLK_LEFT] && newPlayer.getXPos() > 1) {
			newPlayer.setXVel(newPlayer.getXVel() - .1);
		}
		if(keystates[SDLK_RIGHT] && newPlayer.getXPos() < (WINDOW_WIDTH - 10)) {
			newPlayer.setXVel(newPlayer.getXVel() + .1);
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
        
		SDL_BlitSurface(scoreSurface, NULL, screen, &scoreLocation);
	
		newPlayer.setXPos(newPlayer.getXPos()+newPlayer.getXVel());
		newPlayer.setYPos(newPlayer.getYPos()+newPlayer.getYVel());

		applySurface(newPlayer.getXPos(),newPlayer.getYPos(),spriteSheet, screen, &newPlayer.getSprite());        
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
