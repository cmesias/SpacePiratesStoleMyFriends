/*
 * Helper.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "Helper.h"


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

