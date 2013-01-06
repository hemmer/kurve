//
//  functions.cpp
//  SDLTest
//
//  Created by Ewan Hemingway on 03/01/2013.
//  Copyright (c) 2013 Ewan Hemingway. All rights reserved.
//

#include "functions.h"
#include "constants.h"
#include "globals.h"
#include <fstream>
#include <cstdlib>


//File stream for the game log.
std::ofstream logger( "log.txt" );

void log( std::string message ) {
    //Write message to file
    logger << message << std::endl;
    //Flush the buffer
    logger.flush();
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

// python like modulo operator
int pmod(int n, int M)
{
    return ((n % M) + M) % M;
}


Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
        return -1;
    
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}


int randomRange (unsigned int min, unsigned int max)
{
    int base_random = rand(); /* in [0, RAND_MAX] */
    if (RAND_MAX == base_random) return randomRange(min, max);
    /* now guaranteed to be in [0, RAND_MAX) */
    int range       = max - min,
    remainder   = RAND_MAX % range,
    bucket      = RAND_MAX / range;
    /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
    if (base_random < RAND_MAX - remainder) {
        return min + base_random/bucket;
    } else {
        return randomRange (min, max);
    }
}

float randomFloat()
{
    float r = (float)rand()/(float)RAND_MAX;
    return r;
}




bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    
    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    //If there was an error in setting up the screen
    if( screen == NULL ) return false;
    
    playAreaSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, playArea.w, playArea.h, SCREEN_BPP, 0, 0, 0, 0);
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 ) return false;
    
    //Set the window caption
    SDL_WM_SetCaption( "ACHTUNG DIE KURVE", NULL );
    
    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Open the font
    font = TTF_OpenFont( "visitor1.ttf", 10 );
    
    if (font == NULL)
    {
        return false;
    }
    
    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( playAreaSurface );
    SDL_FreeSurface( message );
    
    //Close the font that was used
    TTF_CloseFont( font );
    
    TTF_Quit();
    SDL_Quit();
}