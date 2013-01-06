// SDL headers
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
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
SDL_Surface *dot = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

// TODO neaten this!
SDL_Rect playArea = {static_cast<Sint16>(PADDING), static_cast<Sint16>(PADDING),
    static_cast<Uint16>(SCREEN_WIDTH - 3*PADDING - SIDEBAR_WIDTH),
    static_cast<Uint16>(SCREEN_HEIGHT - 2*PADDING)};

SDL_Rect sideBarArea = {static_cast<Sint16>(playArea.x + playArea.w + PADDING), static_cast<Sint16>(PADDING),
    static_cast<Uint16>(SIDEBAR_WIDTH),
    static_cast<Uint16>(playArea.h)};





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

    //The dot that will be used
    Kurve myKurve(SDLK_LEFT, SDLK_RIGHT, dot1col);
    //Dot myDot2(SDLK_a, SDLK_s, dot2col);

    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    
    SDL_FillRect( screen, &playArea, SDL_MapRGB( screen->format, backgroundColour.r, backgroundColour.g, backgroundColour.b ) );
        
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
        
        //Handle events for the dot
        myKurve.handle_input();
        //myDot2.handle_input();

        
        //Move the dot
        myKurve.move();
        //myDot2.move();
        
        myKurve.checkCollision();
        //myDot2.checkCollision();
        
        //Fill the screen white
        SDL_FillRect( screen, &sideBarArea, SDL_MapRGB( screen->format, 0, 0, 0 ) );
        
        //Show the dot on the screen
        myKurve.show();
        //myDot2.show();
        
        //The timer's time as a string
        std::stringstream time;
        
        //Convert the timer's time to a string
        time << "x, y: " << myKurve.x << " " << myKurve.y;
        //time << "xvel, yvel: " << myDot.xVel << " " << myDot.yVel;
        //time << "xacc, yacc: " << myDot.xAcc << " " << myDot.yAcc;
        //time << "ticks" << SDL_GetTicks();
        message = TTF_RenderText_Solid( font, time.str().c_str(), textColor );
        
        if (message == NULL)
        {
            return 1;
        }
                
        apply_surface(playArea.x + playArea.w + PADDING, PADDING, message, screen);
        
        //Update the screen
        if( SDL_Flip( screen ) == -1 )
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
    clean_up();
    
    return 0;
}
