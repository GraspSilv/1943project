/* Test program for scrolling background */

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include "Background.h"
#include "Timer.h"

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 20;

SDL_Event event;

int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //The offsets of the background
    int bgX = 0, bgY = 0;

    //The frame rate regulator
    Timer fps;
    Background bg("water_bg.jpg");

    //Initialize
    if( bg.init() == false )
    {
        return 1;
    }

    //Load the files
    if( bg.load_files() == false )
    {
        return 1;
    }

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Scroll background
        bgY += 2;

        //If the background has gone too far
        if( bgY >= bg.background->h )
        {
            //Reset the offset
            bgY = 0;
        }

        //Show the background
        bg.apply_surface( bgX, bgY, bg.background, bg.screen );
        bg.apply_surface( bgX, bgY - bg.background->h, bg.background, bg.screen );


        //Update the screen
        if( SDL_Flip( bg.screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    //Clean up
    bg.clean_up();

    return 0;
}
