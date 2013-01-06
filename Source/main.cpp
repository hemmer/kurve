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
#include <ctime>
#include <sstream>


TTF_Font *font10 = NULL;
TTF_Font *font20 = NULL;
TTF_Font *font32 = NULL;

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
    unsigned int time_seed = uint( time(NULL) );
    srand ( time_seed );
    
    // quit flag
    bool quit = false;
    bool isGameInProgress = true;
    
    //The frame rate regulator
    Timer fps;
    

    // initialise
    if( init() == false ) return 1;

    // load the files
    if( load_files() == false ) return 1;
    
    
    const int numPlayers = 2;
    
    // lines for each player
    Kurve myKurve[numPlayers];
    int numWins[numPlayers] = {0};
    
    myKurve[0].setKeys(SDLK_LEFT, SDLK_RIGHT);
    myKurve[0].setColour(dot1col);

    myKurve[1].setKeys(SDLK_a, SDLK_s);
    myKurve[1].setColour(dot2col);

        
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
        
        if (isGameInProgress == false)
        {
            Uint8 *keystates = SDL_GetKeyState( NULL );
            
            if (keystates[SDLK_r])
            {
                SDL_FillRect( playAreaSurface, &playArea,
                             SDL_MapRGB( playAreaSurface->format, backgroundColour.r, backgroundColour.g, backgroundColour.b ) );

                for (int k = 0; k < numPlayers; ++k)
                    myKurve[k].restart();
                
                isGameInProgress = true;
            }
        }
        
        if (isGameInProgress)
        {
            // handle input events for each line
            for (int k = 0; k < numPlayers; ++k)
                myKurve[k].handle_input();

            // move each line in the direction it is facing
            for (int k = 0; k < numPlayers; ++k)
                myKurve[k].move();
            
            // check for collisions with other lines
            for (int k = 0; k < numPlayers; ++k)
                myKurve[k].checkCollision();
            
            
            // render each line on the screen
            for (int k = 0; k < numPlayers; ++k)
                myKurve[k].show();
            
            
            
            
            int numAlivePlayers = numPlayers;
            // check for collisions with other lines
            for (int k = 0; k < numPlayers; ++k)
                if (!myKurve[k].isAlive())
                    --numAlivePlayers;
            
            
            // if only one player is left, game has finished
            if (numAlivePlayers == 1 && numPlayers != 1)
            {
                int winner = -1;
                
                // find who has won
                for (int k = 0; k < numPlayers; ++k)
                    if (myKurve[k].isAlive())
                    {
                        winner = k;
                        break;
                    }
                
                // track the number of wins
                numWins[winner]++;
                
                std::stringstream winnerString;
                winnerString << "Player " << winner << " is the winner";
                message = TTF_RenderText_Solid( font32, winnerString.str().c_str(), textColor );
                apply_surface(playAreaGlobal.w /2 - message->w / 2, playAreaGlobal.h / 2 - message->h / 2 - PADDING, message, playAreaSurface);

                
                std::stringstream restartString;
                restartString << "Press R to restart";
                message = TTF_RenderText_Solid( font20, restartString.str().c_str(), textColor );
                apply_surface(playAreaGlobal.w /2 - message->w / 2, playAreaGlobal.h / 2 - message->h / 2 + PADDING, message, playAreaSurface);
                
                if (message == NULL) return 1;

                isGameInProgress = false;
            }

        }
        
        
        // BACKGROUND: fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
        
        
        // PLAYAREA: draw the playing surface onto the screen
        apply_surface(playAreaGlobal.x, playAreaGlobal.y, playAreaSurface, screen);
        
        
        // SIDEBAR: fill the sidebar with black
        SDL_FillRect( screen, &sideBarArea, SDL_MapRGB( screen->format, 0, 0, 0 ) );
        
        // SIDEBAR: game logo
        message = TTF_RenderText_Solid( font32, "KURVE", textColor );
        apply_surface(sideBarArea.x + PADDING, sideBarArea.y + PADDING, message, screen);
        
        // SIDEBAR: player scores
        for (int k = 0; k < numPlayers; ++k)
        {
            std::stringstream scoreText;
            scoreText << "Player " << k << ": won " << numWins[k];
            message = TTF_RenderText_Solid( font20, scoreText.str().c_str(), textColor );
            if (message == NULL) return 1;

            apply_surface(sideBarArea.x + PADDING, sideBarArea.y + 8*PADDING + k*(PADDING + message->h), message, screen);

        }
        

        // update the screen
        if( SDL_Flip( screen ) == -1 ) return 1;
        
        // cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
        
    }

    
    // clean up resources
    clean_up();
    
    return 0;
}
