//
//  Kurve.cpp
//  SDLTest
//
//  Created by Ewan Hemingway on 03/01/2013.
//  Copyright (c) 2013 Ewan Hemingway. All rights reserved.
//

#include "kurve.h"
#include "constants.h"
#include "functions.h"
#include "globals.h"


#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"

#include <cmath>
#include <sstream>

Kurve::Kurve(int left, int right, SDL_Color lineCol)
{
    //Initialize the offsets
    x = 50;
    y = 50;
    
    speed = 2.0;
    
    //Initialize the velocity
    xVel = speed;
    yVel = 0.0;
    
    leftKeycode = left;
    rightKeycode = right;
    
    lineColour = SDL_MapRGB(screen->format, lineCol.r, lineCol.g, lineCol.b);

    alive = true;
}

void Kurve::handle_input()
{
    //Get the keystates
    Uint8 *keystates = SDL_GetKeyState( NULL );
    
    
    if (keystates[rightKeycode])
    {
        // find the current angle we are pointing in
        double prevAngle = atan2(yVel, xVel);
        // and increment
        double newAngle = prevAngle + 0.1;
        
        xVel = speed * cos(newAngle);
        yVel = speed * sin(newAngle);
    }
    if (keystates[leftKeycode])
    {
        double prevAngle = atan2(yVel, xVel);
        double newAngle = prevAngle - 0.1;
        
        xVel = speed * cos(newAngle);
        yVel = speed * sin(newAngle);
    }
    
    if (keystates[SDLK_r] && !alive)
    {
        // start off in a new position
        x = randomRange(playArea.x, playArea.x + playArea.w);
        y = randomRange(playArea.y, playArea.y + playArea.h);
        
        // pointing in a new direction
        double newAngle = randomFloat() * 2.0 * M_PI;
        xVel = speed * cos(newAngle);
        yVel = speed * sin(newAngle);
        
        alive = true;
    }
}

void Kurve::move()
{
    if (!alive) return;
    
    //Move the dot left or right
    x += xVel;
    
    //Move the dot up or down
    y += yVel;
}

void 

Kurve::checkCollision()
{
    // find where we will be at the next timestep
    double newX = x + 2*xVel;
    double newY = y + 2*yVel;

    // TODO: need proper error checking here
    Uint32 pixel = get_pixel32(screen, pmod(newX, SCREEN_WIDTH), pmod(newY, SCREEN_HEIGHT));
    
    // check if the dot went too far to the left or right (and kill it if so)
    if (( x < playArea.x ) || ( x > playArea.w + playArea.x )) alive = false;
    if (( y < playArea.y ) || ( y > playArea.h + playArea.y )) alive = false;
        
    
    if (pixel != SDL_MapRGB( screen->format, backgroundColour.r, backgroundColour.g, backgroundColour.b ))
    {
        alive = false;
        xVel = yVel = 0.0;
    }
}

void Kurve::show()
{
    // the pixel at the centre of the head
    int midX = x; int midY = y;
    
    // colour a 3x3 pixel area
    for (int i = midX-1; i <= midX+1; ++i)
        for (int j = midY-1; j <= midY+1; ++j)
            put_pixel32(screen, pmod(i, SCREEN_WIDTH), pmod(j, SCREEN_HEIGHT), lineColour);
}
