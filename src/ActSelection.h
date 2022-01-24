/*
 * ActSelection.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef ACTSELECTION_H_
#define ACTSELECTION_H_

#include "LTexture.h"
#include <fstream>
#include <limits>
#include <sstream>

class ActSelection {
public:
	enum Result { Back, Nothing, StartGame, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, ActSelection::Result &result);
	ActSelection::Result mousePressed(SDL_Event event);
	ActSelection::Result mouseReleased(SDL_Event event);
public:

	// Initialize
	void Init();

	// Load resources
	void Load(SDL_Renderer *gRenderer);

	// Free resources
	void Free();
private:
	int mx, my;
	bool leftClick;
	bool quit;
	bool shift;
	SDL_Event event;
};

#endif /* ACTSELECTION_H_ */
