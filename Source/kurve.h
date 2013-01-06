//
//  Dot.h
//  SDLTest
//
//  Created by Ewan Hemingway on 03/01/2013.
//  Copyright (c) 2013 Ewan Hemingway. All rights reserved.
//

#ifndef __SDLTest__Dot__
#define __SDLTest__Dot__

#include <iostream>
#include "SDL/SDL.h"

//The dot that will move around on the screen
class Kurve
{
private:
    
    double speed;
    bool alive;
    
    // SDL keycodes for turning left / right
    int leftKeycode, rightKeycode;
    
    Uint32 lineColour;
public:
    
    //The X and Y offsets of the dot
    double x, y;
    
    //The velocity of the dot
    double xVel, yVel;
    
    //Initializes the variables
    Kurve(int, int, SDL_Color);
    
    //Takes key presses and adjusts the dot's velocity
    void handle_input();
    
    //Moves the dot
    void move();
    
    void checkCollision();
    bool isAlive() { return alive; }
    
    //Shows the dot on the screen
    void show();
};

#endif /* defined(__SDLTest__Dot__) */
