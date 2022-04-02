/*
 * Options.h
 *
 *  Created on: May 24, 2017
 *      Author: Carl
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "LTexture.h"
#include "LWindow.h"
#include "Players.h"
#include "Helper.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Options {
public:

	// Instantiate a helper object so that we can reference it's functions
	Helper helper;

	enum OptionsResult { Back, Nothing, StartGame, ShowingMenu, Exit };
	OptionsResult optionsResult = Nothing;
public:
	// don't declare
	int mx, my;
	SDL_Event e;
	// declare these
	bool pauseLoop;
	bool leftclick;
	int index;
	int key;
	bool confirm;
	int timer;

public:	// Settings that can be changed by user
	// Textures
	LTexture gText;
	LTexture gParticles;
	SDL_Rect cParticles = {32, 0, 8, 8};
	// Fonts
	TTF_Font *gFont 			= NULL;
	TTF_Font *gFont13 			= NULL;
	TTF_Font *gFont26 			= NULL;
	// Audio
	Mix_Music *sAmbientMusic 	= NULL;
	Mix_Chunk *sRockBreak 		= NULL;
	Mix_Chunk *sLazer 			= NULL;
	Mix_Chunk *sAtariBoom 		= NULL;
	Mix_Chunk *sGrenade 		= NULL;
	Mix_Chunk *sGrenadeExplode 	= NULL;
	Mix_Chunk *sGrenadePickup 	= NULL;
	Mix_Chunk *sPistolReload 	= NULL;
	Mix_Chunk *sPirateHurt 		= NULL;
	// Audio, Video Settings
	int MASTER_VOL;
	int MUSIC_VOL;
	int SFX_VOL;
	int RESOLUTION;
	int ANTI_ALIAS;
	int VSYNC;
	int FULLSCREEN;

public: // Bar sliders and text

	// Show different options based on type
	int type;	// [0: PauseMenu], [1: HowToPlay], [2: Options], [3: Exit Main Menu]

	// Sub menu options
	int HIGHLIGHT_INDEX;

	// Set Game items
	bool mouseTitle[5];

	// Determine which applied button pressed
	std::string button;

	// Apply Bar Settings button
	SDL_Rect applyButton[2];		// 0: audio, 1: video (buttons)
	bool applyMouse[2];				// 0: audio, 1: video (buttons)

	// keep & Revert button
	SDL_Rect confirmButton[2];
	bool confirmMouse[2];

	// Items
	class Title{
	public:
		std::string name;
		int x;
		int y;
		int w;
		int h;
	};

	// Max number of items in the bar
	static const int barMax = 7;

	// Bar class
	class Bar{
	public:
		SDL_Rect rect;
		bool mouse;
		bool alive;
		bool highlight;
		int value;
		std::string name;

		void init(Bar bar[]){
			for (int i=0; i<barMax; i++){
				bar[i].rect.w   	= 540;
				bar[i].rect.h   	= 48;
				bar[i].highlight 	= false;
				bar[i].alive 		= false;
			}
		}
		void create(Bar bar[], std::string name){
			for (int i=0; i<barMax; i++){
				if (!bar[i].alive){
					bar[i].name 	= name;
					bar[i].mouse 	= false;
					bar[i].alive 	= true;
					break;
				}
			}
		}
		void update(Bar bar[], int posX, int posY, int mx, int my, bool leftclick, int HIGHLIGHT_INDEX){

			// Set position for all audio sliders
			for (int i=0; i<3; i++){
				bar[i].rect.x = posX;
				bar[i].rect.y = posY+i*(bar[i].rect.h+8);
			}

			// Master volume sliders
			if (bar[0].alive){

				if (mx>=bar[0].rect.x && mx <= bar[0].rect.x+bar[0].rect.w && my>=bar[0].rect.y && my <= bar[0].rect.y+bar[0].rect.h){
					bar[0].mouse = true;
				}else{
					bar[0].mouse = false;
				}

				if (bar[0].mouse){
					if (leftclick){
						bar[0].value = 100*(mx - bar[0].rect.x)/bar[0].rect.w;
					}
				}
			}

			// Music and SFX volume sliders
			for (int i=1; i<3; i++){
				if (bar[i].alive){

					if (mx>=bar[i].rect.x && mx <= bar[i].rect.x+bar[i].rect.w && my>=bar[i].rect.y && my <= bar[i].rect.y+bar[i].rect.h){
						bar[i].mouse = true;
					}else{
						bar[i].mouse = false;
					}

					if (bar[i].mouse){
						if (leftclick){
							bar[i].value = 128*(mx - bar[i].rect.x)/bar[i].rect.w;
						}
					}
				}
			}

			// Video sliders and switches
			for (int i=3; i<barMax; i++){
				if (bar[i].alive){
					bar[i].rect.x = posX;
					bar[i].rect.y = posY+i*(bar[i].rect.h+8);

					if (mx>bar[i].rect.x && mx < bar[i].rect.x+bar[i].rect.w && my>bar[i].rect.y && my < bar[i].rect.y+bar[i].rect.h){
						bar[i].mouse = true;
					}else{
						bar[i].mouse = false;
					}

					if (bar[i].mouse){
						if (leftclick){
							//bar[3].value = 4*(mx - bar[i].rect.x)/bar[i].rect.w;
						}
					}
				}
			}

			/* RESOLUTION BAR */
			if (leftclick){
				int individualBoxWidth = bar[0].rect.w/5;
				if (mx>bar[3].rect.x && mx < bar[3].rect.x+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 1;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 2;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth*2)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 3;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth*3)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 4;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth*4)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 5;
				}
			}

			/* ANTI_ALIASING BAR */
			if (leftclick){
				// Bar variables neater
				int x = bar[4].rect.x;
				int w = bar[4].rect.w;
				int y = bar[4].rect.y;
				int h = bar[4].rect.h;
				if (mx>x && mx < x+w/2 && my>y && my < y+h){
					bar[4].value = 1;
				}else if (mx>x && mx < (x+w/2)+w/2 && my>y && my < y+h){
					bar[4].value = 0;
				}
			}

			/* VSYNC BAR */
			if (leftclick){
				// Bar variables neater
				int x = bar[5].rect.x;
				int w = bar[5].rect.w;
				int y = bar[5].rect.y;
				int h = bar[5].rect.h;
				if (mx>x && mx < x+w/2 && my>y && my < y+h){
					bar[5].value = 1;
				}else if (mx>x && mx < (x+w/2)+w/2 && my>y && my < y+h){
					bar[5].value = 0;
				}
			}

			/* FULLSCREEN BAR */
			if (leftclick){
				// Bar variables neater
				int x = bar[6].rect.x;
				int w = bar[6].rect.w;
				int y = bar[6].rect.y;
				int h = bar[6].rect.h;
				if (mx>x && mx < x+w/2 && my>y && my < y+h){
					bar[6].value = 1;
				}else if (mx>x && mx < (x+w/2)+w/2 && my>y && my < y+h){
					bar[6].value = 0;
				}
			}

			// Highlights
			if (HIGHLIGHT_INDEX == 0){
				for (int i=0; i<3; i++){
					bar[i].highlight = true;
				}
				for (int i=3; i<barMax; i++){
					bar[i].highlight = false;
				}
			}else if (HIGHLIGHT_INDEX == 1){
				for (int i=0; i<3; i++){
					bar[i].highlight = false;
				}
				for (int i=3; i<barMax; i++){
					bar[i].highlight = true;
				}
			}else {
				for (int i=0; i<barMax; i++){
					bar[i].highlight = false;
				}
			}
		}
	};

public:	// classes
	Title title[5];
	Bar bar[7];
	Bar au;

public: // Load Resources

	// Load audio files
	void LoadAudioFiles();

	// Free audio files
	void FreeAudioFiles();

public:	// functions

	// Save configurations
	void saveCFG();

	// Load video configurations
	void loadVideoCFG();

	// Load audio configurations
	void loadAudioCFG();

	// Apply video configurations
	void applyVideoCFG(LWindow &gWindow);

	// Apply audio configurations
	void applyCustomAudioCFG(int MUSIC_VOL, int SFX_VOL);
	void applyOldAudioCFG();
	void applyMasterAudioCFG();

	// Load Options menu
	void start(LWindow &gWindow, SDL_Renderer *gRenderer, Players &player);

	// Save a level to a directory
	void SaveLevel(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop,
				   std::string SpawnCoordinatesData,
				   std::string TileSaveData,
				   std::string PirateSaveData);

	// Return a string the user has entered (hint is used to give the Editor an idea of what to write)
	std::string GetInput(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop, std::string hint);


	// User input
	void OnKeyDown(SDL_Keycode sysm, Players &player);
	void mousePressed(Players &player);
	void mouseReleased(LWindow &gWindow);

	//bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);
};

#endif /* OPTIONS_H_ */
