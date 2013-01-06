//
//  globals.h
//  SDLTest
//
//  Created by Ewan Hemingway on 03/01/2013.
//  Copyright (c) 2013 Ewan Hemingway. All rights reserved.
//

#ifndef SDLTest_globals_h
#define SDLTest_globals_h

//The header
#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"


//The surfaces
extern SDL_Surface *screen;
extern SDL_Surface *message;
extern SDL_Surface *playAreaSurface;

extern SDL_Color backgroundColour;

//The event structure
extern SDL_Event event;

//The font that's going to be used
extern TTF_Font *font;

//File stream for the game log.
extern std::ofstream logger;

extern SDL_Rect playAreaGlobal, playArea;
extern SDL_Rect sideBarArea;

#endif
