/*
 * Helper.h
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#ifndef LOCAL_HELPER_H_
#define LOCAL_HELPER_H_

#include "Timers.h"

#include <SDL2/SDL.h>

class Helper {
public:
    Timer fps;
public:
    // fps
    bool cap 					= true;
	int frame 					= 0;
    const int FRAMES_PER_SECOND = 60;
	//const int screenWidth = 2880;
	//const int screenHeight = 1620;
    //const int screenWidth = 2304;
    //const int screenHeight = 1296;
	const int screenWidth = 1920;
	const int screenHeight = 1080;

public:

	// Check sollision
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	void RenderRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);

	void RenderFillRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);

	// Generate a random double number
	double randDouble(double fMin, double fMax);

	// Set Texture clips
	void setClips(SDL_Rect &tempRect, int x, int y, int w, int h);

};

#endif /* LOCAL_HELPER_H_ */
