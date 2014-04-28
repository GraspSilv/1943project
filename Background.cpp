/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Background.cpp
	Implementation of background class (display scrolling background)
History
	03/31/14	Bill Gowans		Create
	04/08/14	Jon Richelsen	Standardize
	04/16/14	Jon Richelsen	update header to remove To Do section
	04/23/14	Bill Gowan		Define nondefault constructor, load_image(), apply_surface(), init(), load_files(), and clean_up()
	04/28/14	Jon Richelsen	Update header to include all changes
*/

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include "Background.h"
#include "Timer.h"

using namespace std;

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 20;

Background::Background (string filename)
{
	file = filename;
	background = NULL;
	screen = NULL;
}

SDL_Surface * Background::load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

SDL_Surface * Background::getBG(int x, int y){
    SDL_Surface * tempSurf;
    SDL_Rect bgRect;
    bgRect.x = x;
    bgRect.y = y;
    bgRect.w = 480;
    bgRect.h = 640;
    SDL_BlitSurface(background,&bgRect,tempSurf,&bgRect);
    return tempSurf;
}

void Background::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool Background::init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Background", NULL );

    //If everything initialized fine
    return true;
}

bool Background::load_files()
{
    //Load the background
    background = load_image(file);

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void Background::clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );

    //Quit SDL
    SDL_Quit();
}
