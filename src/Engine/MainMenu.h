/*
 * MainMenu.h
 *
 *  Created on: Apr 20, 2017
 *      Author: Carl
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "LTexture.h"
#include "Helper.h"
#include "Options.h"

#include <fstream>
#include <limits>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class MainMenu : public Helper {
public:
	enum MenuResult { Nothing, NewGame, HowToPlay, Options, Credits, Exit };
	void Show(LWindow &gWindow,
			SDL_Renderer *gRenderer,
			MainMenu::MenuResult &result);
	MainMenu::MenuResult mousePressed(SDL_Event event);
	MainMenu::MenuResult mouseReleased(SDL_Event event);
	bool checkCollision(SDL_Rect a, SDL_Rect b);
	MainMenu::MenuResult OnKeyDown( SDL_Keycode sym);
private:
	void load(SDL_Renderer *gRenderer);
	void save();
	void free();
	LTexture gMenuItems;	// Load textures
	SDL_Event event;		// Events
	int x, y;				// Menu start position
	int w, h;				// Menu button size
	int mW, mH;				// Menu margins
	int mx,  my;			// Mouse position
	bool leftClick;			// Mouse click
	bool quit;
	int index;				// menu selection
	int key;			// 0 = mouse, 1 = keyboard

	/* Main Menu items
	 * 0: New Game
	 * 1: HowToPlay
	 * 2: Options
	 * 3: Credits
	 * 4: Quit
	 */
	SDL_Rect button[5];

	/* Texture clips
	 * 0: New Game
	 * 1: HowToPlay
	 * 2: Options
	 * 3: Credits
	 * 4: Quit
	 */
	SDL_Rect clip[5];
};

#endif /* MAINMENU_H_ */
