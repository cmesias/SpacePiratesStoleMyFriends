/*
 * Helper.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "Helper.h"
#include <math.h>

float Helper::getDistance( float targetX, float targetY, float selfX, float selfY ) {

	float distance = 1;

	distance = sqrt((targetX - selfX) * (targetX - selfX)+
					(targetY - selfY) * (targetY - selfY));

	return distance;
}

bool Helper::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2) {
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2) {
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

bool Helper::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}


void Helper::RenderRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color) {
	SDL_Rect tempRect = {x, y, w, h};
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);
}

void Helper::RenderFillRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color) {
	SDL_Rect tempRect = {x, y, w, h};
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);
}

// Generate a random double number
double Helper::randDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

// Set Texture clips
void Helper::setClips(SDL_Rect &tempRect, int x, int y, int w, int h){
	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = w;
	tempRect.h = h;
}

