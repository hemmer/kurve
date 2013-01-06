//
//  functions.h
//  SDLTest
//
//  Created by Ewan Hemingway on 03/01/2013.
//  Copyright (c) 2013 Ewan Hemingway. All rights reserved.
//

#ifndef __SDLTest__functions__
#define __SDLTest__functions__

#include <iostream>

//The headers
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include <string>

void log( std::string message );

//Surface blitter
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

Uint32 get_pixel32( SDL_Surface *surface, int x, int y );
void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

int pmod(int n, int M);
int randomRange(unsigned int, unsigned int);
float randomFloat();

//Initialization
bool init();

//File loading
bool load_files();

//Clean up
void clean_up();

#endif /* defined(__SDLTest__functions__) */
