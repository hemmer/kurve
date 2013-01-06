// SDL headers
#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"

// project headers
#include "kurve.h"
#include "constants.h"
#include "functions.h"
#include "globals.h"
#include "timer.h"

// system headers
#include <string>
#include <sstream>


TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 200, 0, 0 };
SDL_Color backgroundColour = { 200, 200, 200 };


SDL_Color dot1col = { 0, 0, 200 };
SDL_Color dot2col = { 0, 200, 0 };


//The surfaces
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *playAreaSurface = NULL;

//The event structure
SDL_Event event;

// TODO neaten this!
SDL_Rect playAreaGlobal = {static_cast<Sint16>(PADDING), static_cast<Sint16>(PADDING),
                           static_cast<Uint16>(SCREEN_WIDTH - 3*PADDING - SIDEBAR_WIDTH),
                           static_cast<Uint16>(SCREEN_HEIGHT - 2*PADDING)};

SDL_Rect playArea = {0 ,0, playAreaGlobal.w, playAreaGlobal.h};

SDL_Rect sideBarArea = {static_cast<Sint16>(playAreaGlobal.x + playAreaGlobal.w + PADDING), static_cast<Sint16>(PADDING),
                        static_cast<Uint16>(SIDEBAR_WIDTH), static_cast<Uint16>(playArea.h)};



int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;
    
    //The frame rate regulator
    Timer fps;
    

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    
    //Load the files
    if( load_files() == false )
    {
        return 1;
    }
    
    const int numPlayers = 2;
    
    // lines for each player
    Kurve myKurve[numPlayers];
    
    myKurve[0].setKeys(SDLK_LEFT, SDLK_RIGHT);
    myKurve[0].setColour(dot1col);

    myKurve[1].setKeys(SDLK_a, SDLK_s);
    myKurve[1].setColour(dot2col);

    // fill the whole screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
        
    SDL_FillRect( playAreaSurface, &playArea,
                 SDL_MapRGB( playAreaSurface->format, backgroundColour.r, backgroundColour.g, backgroundColour.b ) );
        
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
        
        // handle input events for each line
        for (int k = 0; k < numPlayers; ++k)
            myKurve[k].handle_input();

        // move each line in the direction it is facing
        for (int k = 0; k < numPlayers; ++k)
            myKurve[k].move();
        
        // check for collisions with other lines
        for (int k = 0; k < numPlayers; ++k)
            myKurve[k].checkCollision();
        
        
        
        // BACKGROUND: fill the screen white
        SDL_FillRect( screen, &sideBarArea, SDL_MapRGB( screen->format, 0, 0, 0 ) );
        
        // redner each line on the screen
        for (int k = 0; k < numPlayers; ++k)
            myKurve[k].show();
        
        
        // draw the playing surface onto the screen
        apply_surface(playAreaGlobal.x, playAreaGlobal.y, playAreaSurface, screen);
        
        
        std::stringstream position;
        //Convert the position time to a string
        position << "x, y: " << myKurve[0].x << " " << myKurve[0].y;
        message = TTF_RenderText_Solid( font, position.str().c_str(), textColor );
        if (message == NULL) return 1;
    
        
        apply_surface(playAreaGlobal.x + playAreaGlobal.w + PADDING, PADDING, message, screen);
        
        //Update the screen
        if( SDL_Flip( screen ) == -1 ) return 1;
        
        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
        
    }
    
    //Clean up
    clean_up();
    
    return 0;
}
