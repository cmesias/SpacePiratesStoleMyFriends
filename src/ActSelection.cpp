/*
 * ActSelection.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Engine/LWindow.h"
#include "ActSelection.h"

void ActSelection::Init() {

}

void ActSelection::Load(SDL_Renderer *gRenderer) {

}

void ActSelection::Free() {

}

void ActSelection::Show(LWindow &gWindow, SDL_Renderer *gRenderer, ActSelection::Result &result) {

	// Upon entry
	quit = false;
	leftClick = false;
	shift = false;

	// Load resources
	Load(gRenderer);

	std::cout << "Inside Act Selection Screen" << std::endl;

	// While loop
	while (!quit) {

		// Get mouse position
		SDL_GetMouseState(&mx, &my);

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Back Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					}
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}
				}

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				result = mouseReleased(event);

				// Customize Character results
				/*switch (result)  {
					case Back:				// Back (to Main Menu)
						quit = true;
						break;
					case Nothing:			// Nothing

						break;
					case StartGame:			// Start Game
						quit = true;
						break;
				}*/
			}
		}

		// Update Variables


		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 80, 80, 0, 255);
		SDL_RenderClear(gRenderer);

			SDL_Rect tempr = {gWindow.getWidth()/2-64, gWindow.getHeight()/2-64, 128, 128};
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderFillRect(gRenderer, &tempr);

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	// Free everything
	Free();
}


// Mouse Pressed
ActSelection::Result ActSelection::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return Nothing;
}

// Mouse Released
ActSelection::Result ActSelection::mouseReleased(SDL_Event event){
	ActSelection::Result result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			/*for (int i=0; i<3; i++) {
				// Check Mouse hover w/ Menu item
				SDL_Rect a = {mx, my, 1, 1};
				SDL_Rect b = {button[i].x, button[i].y, button[i].w, button[i].h};
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(a, b)) {
					if (i == 0) {			// Back
						result = Back;
					}else if (i == 1) {		// Save Character
						// function here to save customized character to file
						result = Nothing;
					}else if (i == 2) {		// Start Game
						result = StartGame;
					}
				}
			}*/
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return result;
}
