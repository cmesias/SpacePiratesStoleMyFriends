/*
 * SplashScreen.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>

#include "Engine/LTexture.h"
#include "Engine/LWindow.h"
#include "Engine/Helper.h"
#include "SplashScreen.h"

void SplashScreen::Show(LWindow &gWindow, SDL_Renderer *gRenderer, SplashScreen::SplashResult &result) {

	// Events
	SDL_Event event;

	// Timer
	int alpha;

	// Upon entry
	alpha = 0;
	gSplash.loadFromFile(gRenderer, "resource/gfx/splash/splash.png");

	bool fadeOut = false;

	bool fade = true;

	// While loop
	while (true) {

		// fps timer
		fps.start();

		while (SDL_PollEvent(&event)) {

			// Global Events
			if (event.type == SDL_QUIT) {
				result = Exit;
				free();
				return;
			}

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Key Pressed
			if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				if (alpha != 255) {
					alpha = 255;
					fadeOut = false;
					fade = false;
				}else{
					result = Play;
					free();
					return;
				}
			}
			// Mouse Pressed
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (alpha != 255) {
					alpha = 255;
					fadeOut = false;
					fade = false;
				}else{
					result = Play;
					free();
					return;
				}
			}
		}

		// Fade
		if (fade) {
			// Increase transparency
			if (!fadeOut) {
				if (alpha < 255) {
					alpha++;
				}
			}
			if (alpha == 255) {
				fadeOut = true;
			}
			// Fade out splash screen
			if (fadeOut) {
				if (alpha > 0) {
					alpha-=5;
				}
				if (alpha <= 0) {
					fadeOut = false;
					fade = false;
					result = Play;
					free();
					return;
				}
			}
		}

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			gSplash.setAlpha(alpha);
			gSplash.render(gRenderer, 0, 0, screenWidth, screenHeight);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if (cap && fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
			SDL_Delay((1000/FRAMES_PER_SECOND ) - fps.get_ticks());
		}
	}
	free();
}



void SplashScreen::free() {
	std::cout << std::endl;
	// Free resources
	gSplash.free();
}
