/*
 * MainMenu.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "MainMenu.h"
#include "LWindow.h"

void MainMenu::Show(LWindow &gWindow, SDL_Renderer *gRenderer, MainMenu::MenuResult &result) {

	// Upon entry
	quit 		= false;
	leftClick 	= false;
	index 		= 0;
	key 		= 0;

	// Load resources
	load(gRenderer);

	// While loop
	while (!quit) {

		// fps
		fps.start();

		// Get mouse position
		SDL_GetMouseState(&mx, &my);

		mx = (screenWidth*mx)/gWindow.getWidth();					// New mouse coordinates, no relation to camera
		my = (screenHeight*my)/gWindow.getHeight();				// New mouse coordinates, no relation to camera

		// Set buttons
		for (int i=0; i<5; i++) {
			button[i].w = w;
			button[i].h = h;
			button[i].x = x+i*mW;
			button[i].y = y+i*mH;
		}

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Exit Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				free();
				return;
			}else{
				// Mouse moved
				if (event.type == SDL_MOUSEMOTION) {
					key = 0;
				}

				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					//key = 1;
					result = OnKeyDown(event.key.keysym.sym);
				}

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				result = mouseReleased(event);
				switch (result)  {
					case NewGame:
						quit = true;
						break;
					case HowToPlay:
						quit = true;
						break;
					case Options:
						quit = true;
						break;
					case Credits:
						quit = true;
						break;
					case Exit:
						quit = true;
						break;
					case Nothing:

						break;
				}
			}
		}

		//Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

		// Render mouse selection
		if (key == 0) {
			// Render Main Menu item
			for (int i=0; i<5; i++) {
				// Check Mouse hover w/ Menu item
				SDL_Rect a = {mx, my, 1, 1};
				SDL_Rect b = {button[i].x, button[i].y, button[i].w, button[i].h};
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(a, b)) {
					// Mouse click down
					if (leftClick) {
						gMenuItems.setAlpha(180);
						gMenuItems.setColor(250, 144, 20);
						gMenuItems.render(gRenderer, button[i].x+button[i].w/2-w/2, button[i].y+button[i].h/2-h/2, w, h, &clip[i]);
					}
					// No mouse click down, just hovering
					else if (!leftClick){
						gMenuItems.setAlpha(210);
						gMenuItems.setColor(20, 190, 22);
						gMenuItems.render(gRenderer, button[i].x+button[i].w/2-w/2, button[i].y+button[i].h/2-h/2, w, h, &clip[i]);
					}
				}
				// If mouse is NOT hovering over menu item then render normally
				else{
					gMenuItems.setAlpha(255);
					gMenuItems.setColor(255, 255, 255);
					gMenuItems.render(gRenderer, button[i].x+button[i].w/2-w/2, button[i].y+button[i].h/2-h/2, w, h, &clip[i]);
				}

				//SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				//SDL_RenderDrawRect(gRenderer, &button[i]);if (!leftClick || index == i){
			}
		}

		// Render keyboard selection
		else{
			// Darken specific menu selection if hovering a Title Name's box
			for (int i=0; i<5; i++) {
				if (index == i){
					gMenuItems.setAlpha(210);
					gMenuItems.setColor(20, 190, 22);
					gMenuItems.render(gRenderer, button[i].x+button[i].w/2-w/2, button[i].y+button[i].h/2-h/2, w, h, &clip[i]);
				}else{
					gMenuItems.setAlpha(255);
					gMenuItems.setColor(255, 255, 255);
					gMenuItems.render(gRenderer, button[i].x+button[i].w/2-w/2, button[i].y+button[i].h/2-h/2, w, h, &clip[i]);
				}
			}
		}

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


// Key Pressed
MainMenu::MenuResult MainMenu::OnKeyDown( SDL_Keycode sym )
{
	MainMenu::MenuResult result = Nothing;
	switch (sym){
	case SDLK_x:
		x = mx;
		y = my;
		save();
		break;
	case SDLK_UP:
		if (index > 0) {
			index--;
		}
		break;
	case SDLK_DOWN:
		if (index < 4) {
			index++;
		}
		break;
	case SDLK_RETURN:
		if (index == 0) {			// New Game
			result = NewGame;
		}else if (index == 1) {		// Load Game
			result = HowToPlay;
		}else if (index == 2) {		// Options
			result = Options;
		}else if (index == 3) {		// Credits
			result = Credits;
		}else if (index == 4) {		// Quit
			result = Exit;
		}
		break;
	}
	return result;
}

// Mouse Pressed
MainMenu::MenuResult MainMenu::mousePressed(SDL_Event event){
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
MainMenu::MenuResult MainMenu::mouseReleased(SDL_Event event){
	MainMenu::MenuResult result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			for (int i=0; i<5; i++) {
				// Check Mouse hover w/ Menu item
				SDL_Rect a = {mx, my, 1, 1};
				SDL_Rect b = {button[i].x, button[i].y, button[i].w, button[i].h};
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(a, b)) {
					if (i == 0) {			// New Game
						result = NewGame;
					}else if (i == 1) {		// Load Game
						result = HowToPlay;
					}else if (i == 2) {		// Options
						result = Options;
					}else if (i == 3) {		// Credits
						result = Credits;
					}else if (i == 4) {		// Quit
						result = Exit;
					}
				}
			}
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return result;
}

bool MainMenu::checkCollision(SDL_Rect a, SDL_Rect b){
	bool collide;
	if (a.x+a.w > b.x && a.x < b.x + b.w &&
		a.y+a.h > b.y && a.y < b.y + b.h){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}


void MainMenu::load(SDL_Renderer *gRenderer) {
	// Load Texture scenes
	gMenuItems.loadFromFile(gRenderer, "resource/gfx/splash/main_menu.png");
	for (int i=0; i<5; i++) {
		clip[i].x = 0;
		clip[i].y = 0+i*96;
		clip[i].w = 384;
		clip[i].h = 96;
	}
	// Load Menu starting coordinates
	std::ifstream fileTileNumL("resource/data/cfg/menu.cfg");
	if (fileTileNumL.is_open()) {
		fileTileNumL >> x >> y >>
					 w >> h>>
					 mW >> mH;
	}
}


void MainMenu::save() {
	// Load Menu starting coordinates
	std::ofstream fileTileNumL("resource/data/cfg/menu.cfg");
	if (fileTileNumL.is_open()) {
		fileTileNumL << x  << " " <<
					    y  << " " <<
						w  << " " <<
						h  << " " <<
						mW << " " <<
						mH;
	}
}

void MainMenu::free() {
	std::cout << std::endl;
	// Free resources
	gMenuItems.free();
}
