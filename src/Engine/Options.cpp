/*
 * Options.cpp
 *
 *  Created on: May 24, 2017
 *      Author: Carl
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Options.h"

// TODO [ ] - Code an Options Menu inside options to be used in both MainMenu and during GamePlay
//			  After coding of Options Menu, the goal is to remove the Pause.cpp class.

void Options::LoadAudioFiles() {
	sRockBreak = Mix_LoadWAV("sounds/rock_break.wav");
	sLazer = Mix_LoadWAV("sounds/snd_gun.wav");
	sAtariBoom = Mix_LoadWAV("sounds/atari_boom.wav");
	sGrenade = Mix_LoadWAV("sounds/snd_grenade.wav");
	sGrenadeExplode = Mix_LoadWAV("sounds/snd_grenade_explode.wav");
	sGrenadePickup = Mix_LoadWAV("sounds/snd_grenade_pickup.wav");
	sPistolReload = Mix_LoadWAV("sounds/snd_reload_pistol.wav");
	sPirateHurt = Mix_LoadWAV("sounds/cmesias/snd_pirate_hurt.wav");

	sAmbientMusic = Mix_LoadMUS("sounds/ambient_space.mp3");
}

void Options::FreeAudioFiles() {
	Mix_FreeChunk(sRockBreak);
	Mix_FreeChunk(sLazer);
	Mix_FreeMusic(sAmbientMusic);
	Mix_FreeChunk(sAtariBoom);
	Mix_FreeChunk(sGrenade);
	Mix_FreeChunk(sGrenadeExplode);
	Mix_FreeChunk(sGrenadePickup);
	Mix_FreeChunk(sPistolReload);
	Mix_FreeChunk(sPirateHurt);
	sRockBreak 		= NULL;
	sAtariBoom 		= NULL;
	sLazer 			= NULL;
	sAmbientMusic 	= NULL;
	sGrenade 		= NULL;
	sGrenadeExplode = NULL;
	sGrenadePickup = NULL;
	sPirateHurt = NULL;
}


void Options::saveCFG(){
	/*std::ofstream fileSettings;
	fileSettings.open( "cfg/config.cfg" );
		std::stringstream tempss;
		tempss << MASTER_VOL << " "
			   << MUSIC_VOL  << " "
			   << SFX_VOL 	 << " "
			   << RESOLUTION << " "
			   << ANTI_ALIAS << " "
			   << VSYNC 	 << " "
			   << FULLSCREEN;
		fileSettings << tempss.str().c_str();
	fileSettings.close();*/
}

// Load video configurations
void Options::loadVideoCFG(){
	// open config file
	std::fstream fileSettings( "cfg/video.cfg" );
	if (fileSettings.is_open()) {
		fileSettings >>  RESOLUTION >> ANTI_ALIAS >> VSYNC >> FULLSCREEN;
		//std::cout << "Loaded video.cfg" << std::endl;
	}else{
		std::cout << "Error opening video.cfg." << std::endl;
		std::cout << "\tCreating default video.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/video.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created video.cfg." << std::endl;
			newConfigFile << "2 1 0 0";
		}
		// Opening and creating default config file failed, set Settings manually
		else{
			std::cout << "\t\tError creating default video.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default video.cfg settings." << std::endl;
			RESOLUTION			= 2;
			ANTI_ALIAS			= 1;
			VSYNC				= 0;
			FULLSCREEN			= 0;
		}
		newConfigFile.close();
	}
	fileSettings.close();
}

// Load audio configurations
void Options::loadAudioCFG(){
	// open config file
	std::fstream fileSettings( "cfg/audio.cfg" );
	if (fileSettings.is_open()) {
		fileSettings >>  MASTER_VOL >> MUSIC_VOL >> SFX_VOL;
		//std::cout << "Loaded audio.cfg" << std::endl;
	}else{
		std::cout << "Error opening audio.cfg." << std::endl;
		std::cout << "\tCreating default audio.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/audio.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created audio.cfg." << std::endl;
			newConfigFile << "100 128 128";
		}
		// Opening and creating default config file failed, set Settings manually
		else{
			std::cout << "\t\tError creating default audio.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default audio.cfg settings." << std::endl;
			MASTER_VOL			= 100;
			MUSIC_VOL			= 128;
			SFX_VOL				= 128;
		}
		newConfigFile.close();
	}
	fileSettings.close();
}


/* Set Video Settings */
void Options::applyVideoCFG(LWindow &gWindow) {
	gWindow.applySettings(RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN);
}

/* Set Audio Settings */
void Options::applyCustomAudioCFG(int MUSIC_VOL, int SFX_VOL) {
	Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sLazer, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sAtariBoom, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sGrenade, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sGrenadeExplode, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sGrenadePickup, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sPistolReload, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sPirateHurt, SFX_VOL*(MASTER_VOL*0.01));
}

void Options::applyOldAudioCFG() {
	Mix_VolumeMusic(MUSIC_VOL);
	Mix_VolumeChunk(sRockBreak, SFX_VOL);
	Mix_VolumeChunk(sLazer, 	SFX_VOL);
	Mix_VolumeChunk(sAtariBoom, SFX_VOL);
	Mix_VolumeChunk(sGrenade, SFX_VOL);
	Mix_VolumeChunk(sGrenadeExplode, SFX_VOL);
	Mix_VolumeChunk(sGrenadePickup, SFX_VOL);
	Mix_VolumeChunk(sPistolReload, SFX_VOL);
	Mix_VolumeChunk(sPirateHurt, SFX_VOL);
}

void Options::applyMasterAudioCFG() {
	Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sLazer, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sAtariBoom, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sGrenade, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sGrenadeExplode, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sGrenadePickup, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sPistolReload, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sPirateHurt, SFX_VOL*(MASTER_VOL*0.01));
}

//Get's input from user and returns it
void Options::start(LWindow &gWindow, SDL_Renderer *gRenderer, Players &player)
{

	// Create title bar names
	au.init(bar);
	au.create(bar, "MASTER VOLUME");
	au.create(bar, "MUSIC VOLUME");
	au.create(bar, "SFX VOLUME");
	au.create(bar, "RESOLUTION");
	au.create(bar, "ANTI ALIASING");
	au.create(bar, "VSYNC");
	au.create(bar, "FULLSCREEN");

	// Reset upon entering
	for (int i=0; i<5; i++) {mouseTitle[i] = false;}
	title[0].name 		= "RESUME";
	title[1].name 		= "HOW TO PLAY";
	title[2].name 		= "SETTINGS";
	title[3].name 		= "EXIT MAIN MENU";
	title[4].name 		= "EXIT TO DESKTOP";
	pauseLoop 			= true;
	index 				= 0;
	key 				= 0;
	type 				= 0;
	HIGHLIGHT_INDEX		= -1;
	leftclick			= false;
	applyMouse[0]		= false;
	applyMouse[1]		= false;
	confirmMouse[0]		= false;
	confirmMouse[1]		= false;
	confirm				= false;
	timer				= 2552;
	//frame 				= 0;
	//SDL_ShowCursor(true);

	// Load Bar values
	bar[0].value 	= MASTER_VOL;
	bar[1].value 	= MUSIC_VOL;
	bar[2].value 	= SFX_VOL;

	// Load Video values
	bar[3].value 	= RESOLUTION;
	bar[4].value 	= ANTI_ALIAS;
	bar[5].value 	= VSYNC;
	bar[6].value 	= FULLSCREEN;

	Helper helper;

	// Set to False so Player can see their mouse
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_ShowCursor(true);

	while (pauseLoop) {

		// Start FPS cap
		helper.fps.start();

		// Start FPS cap
		//fps.start();

		// Set Game items
		for (int i=0; i<5; i++) {
			title[i].x = 72;
			title[i].y = helper.screenHeight/2-14/2-130+i*29;
		}

		// Apply Bar button
		applyButton[0].w = 125;
		applyButton[0].h = 48;
		applyButton[0].x = 72;
		applyButton[0].y = helper.screenHeight-applyButton[0].h-72;

		// Apply Video button
		applyButton[1].w = 125;
		applyButton[1].h = 48;
		applyButton[1].x = 207;
		applyButton[1].y = helper.screenHeight-applyButton[1].h-72;

		// Get mouse coordinates
		SDL_GetMouseState(&mx, &my);

		/* get render width and height
		 * but (mostly used to get actual render height)
		 */
		int renderW = 0;
		int renderHDummy = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
		int en = renderW * 0.4375;
		int renderH = renderW - en;

		// Grid like coordinates of mouse
		// get new mouse coordinates based on render size, and actual screen size
		mx = (helper.screenWidth*mx)/renderW;	// New mouse coordinates, no relation to camera
		my = (helper.screenHeight*my)/renderH;	// New mouse coordinates, no relation to camera

		while (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				optionsResult	= Exit;
				pauseLoop 	= false;
				//gameLoop 	= false;
				//selection 	= -1;
			}

			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				key = 0;
				OnKeyDown(e.key.keysym.sym, player);
			}
			//If Key Released
			else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:
					//
					break;
				case SDLK_DOWN:
					//
					break;
				}
			}

			// Mouse clicked
			if (e.type==SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					leftclick = true;
					mousePressed(player);
				}
			}
			if (e.type==SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					leftclick = false;

					mouseReleased(gWindow);
				}
			}

			// Mouse motion?
			if (e.type == SDL_MOUSEMOTION) {
				key = 1;
			}
		}

		// Update
		au.update(bar, helper.screenWidth-bar[0].rect.w-72, title[0].y, mx, my, leftclick, HIGHLIGHT_INDEX);

		for (int i=0; i<5; i++) {
			if (helper.checkCollision(mx, my, 1, 1, title[i].x-3, title[i].y-3, title[i].w+3, title[i].h+3)) {
				mouseTitle[i] = true; }else{ mouseTitle[i] = false;
			}
		}

		/* Mouse on Apply Bar or Video button */
		for (int i=0; i<5; i++) {
			if (helper.checkCollision(mx, my, 1, 1, applyButton[i].x, applyButton[i].y, applyButton[i].w, applyButton[i].h)) {
				applyMouse[i] = true; }else{ applyMouse[i] = false;
			}
		}

		// Generate Stars
		//part.genStars(particle, helper.screenWidth, helper.screenHeight);

		// Update Particles
		//part.updateStarParticles(particle, 0, 0, helper.screenWidth, helper.screenHeight);

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render particles (some particles are used as actual bullets)
			//part.renderStarParticle(particle, (60*distanceW)/500, (60*distanceH)/500, 1, gRenderer);

			// Welcome text
			std::stringstream tempss;
			tempss << "Fullscreen slider does not work. Press F11 for Fullscreen! " << index;
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26);
			gText.render(gRenderer, helper.screenWidth-gText.getWidth(), 0,gText.getWidth(), gText.getHeight());

			// Render Game items
			int typeShown;
			if (type==0) {
				typeShown = 5;
			}if (type==2) {
				typeShown = 3;
			}
			for (int i=0; i<typeShown; i++) {
				if (key==0)
				{
					// Get PauseMenu item name
					std::stringstream tempss;
					tempss << title[i].name;

					// Determine color
					if (index==i) {
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {244,144,20}, gFont26);
					}else{
						gText.loadFromRenderedText(gRenderer,tempss.str().c_str(), {255,255,255}, gFont26);
					}

					// Render text
					title[i].w = gText.getWidth();
					title[i].h = gText.getHeight();
					gText.render(gRenderer, title[i].x,title[i].y, title[i].w, title[i].h);
				}else if (key==1)
				{
					// Set index if mouseAudio is over a menu item
					if (mouseTitle[i]) {
						index=i;
					}

					// Get PauseMenu item name
					std::stringstream tempss;
					tempss << title[i].name;

					// Determine color
					if (index==i) {
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {244,144,20}, gFont26);
					}else{
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26);
					}

					// Render text
					title[i].w = gText.getWidth();
					title[i].h = gText.getHeight();
					gText.render(gRenderer, title[i].x,title[i].y, title[i].w, title[i].h);
				}
			}

			// Render audio options
			if (type==2) {
				for (int i=0; i<barMax; i++) {
					// Draw bg
					SDL_Rect tempr {bar[i].rect.x, bar[i].rect.y,
						 bar[i].rect.w, bar[i].rect.h};
					SDL_SetRenderDrawColor(gRenderer, 25, 25, 25, 155);
					SDL_RenderFillRect(gRenderer, &tempr);

					////////////////////////////////////////////////////////////////////////////////////////
					//-------------------------------------- Volume Sliders ------------------------------//
					if (i==0) {
						// Render Volume sliders
						SDL_Rect tempr {bar[i].rect.x, bar[i].rect.y, bar[i].rect.w*(bar[i].value)/100, bar[i].rect.h};
						SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
						SDL_RenderFillRect(gRenderer, &tempr);

						// Render Volume number
						tempss.str(std::string());
						tempss << bar[i].value;
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26);
						gText.render(gRenderer, bar[i].rect.x+bar[i].rect.w-gText.getHeight()-10,
													 bar[i].rect.y+bar[i].rect.h/2-gText.getHeight()/2,
													 gText.getWidth(), gText.getHeight());
					}
					if (i>=1 && i<3) {
						// Render Volume sliders
						SDL_Rect tempr {bar[i].rect.x, bar[i].rect.y,
							bar[i].rect.w*(bar[i].value)/128, bar[i].rect.h};
						SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
						SDL_RenderFillRect(gRenderer, &tempr);

						// Render Volume number
						tempss.str(std::string());
						tempss << bar[i].value;
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26);
						gText.render(gRenderer, bar[i].rect.x+bar[i].rect.w-gText.getHeight()-10,
													 bar[i].rect.y+bar[i].rect.h/2-gText.getHeight()/2,
													 gText.getWidth(), gText.getHeight());
					}

					// Render PauseMenu text
					std::stringstream tempss;
					tempss.str(std::string());
					tempss << bar[i].name;
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26);
					gText.render(gRenderer, bar[i].rect.x-gText.getWidth()-10,
												 bar[i].rect.y+bar[i].rect.h/2-gText.getHeight()/2,
												 gText.getWidth(), gText.getHeight());
				}

				/* Resolution Bar */
				// Actual value bar
				std::stringstream tempsss;
				int mAdd;
				if (bar[3].value==5) {
					tempsss << "1920x1080";
					mAdd = 4;
				}else if (bar[3].value==4) {
					tempsss << "1600x900";
					mAdd = 3;
				}else if (bar[3].value==3) {
					tempsss << "1336x768";
					mAdd = 2;
				}else if (bar[3].value==2) {
					tempsss << "1280x720";
					mAdd = 1;
				}else if (bar[3].value==1) {
					tempsss << "1024x576";
					mAdd = 0;
				}

				// Background bg
				SDL_Rect tempr {bar[3].rect.x+108*mAdd, bar[3].rect.y, bar[3].rect.w/5, bar[3].rect.h};
				SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
				SDL_RenderFillRect(gRenderer, &tempr);

				// Render the current Resolution name being selected
				gText.loadFromRenderedText(gRenderer, tempsss.str().c_str(), {255,255,255}, gFont26);
				int individualBoxWidth = bar[0].rect.w/5;
				gText.render(gRenderer,  ((bar[3].rect.x+individualBoxWidth*4)+individualBoxWidth) - individualBoxWidth/2 - gText.getWidth()/2,
											 bar[3].rect.y+bar[3].rect.h/2-gText.getHeight()/2,
											 gText.getWidth(), gText.getHeight());

				// Border
				for (int i=0; i<5; i++) {
					SDL_Rect tempr {bar[3].rect.x+i*108, bar[3].rect.y, 108, bar[3].rect.h};
					SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 155);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}

				/* FULLSCREEN & VSYNC BAR */
				// Actual value bar
				for (int j=4; j<barMax; j++) {
					std::stringstream ssF;
					if (bar[j].value==1) {
						ssF << "On";
						// Render if it is selected
						SDL_Rect tempr {bar[j].rect.x, bar[j].rect.y, bar[j].rect.w/2, bar[j].rect.h};
						SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
						SDL_RenderFillRect(gRenderer, &tempr);
					}else if (bar[j].value==0) {
						ssF << "Off";
						SDL_Rect tempr {bar[j].rect.x+bar[j].rect.w/2, bar[j].rect.y, bar[j].rect.w/2, bar[j].rect.h};
						SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
						SDL_RenderFillRect(gRenderer, &tempr);
					}
					gText.loadFromRenderedText(gRenderer, ssF.str().c_str(), {255,255,255}, gFont26);
					gText.render(gRenderer, bar[j].rect.x+bar[j].rect.w-65-10,
												 bar[j].rect.y+bar[j].rect.h/2-gText.getHeight()/2,
												 gText.getWidth(), gText.getHeight());

					// Border
					/*for (int i=0; i<2; i++) {
						SDL_Rect tempr {bar[j].rect.x+i*(bar[0].rect.w/2), bar[j].rect.y, (bar[0].rect.w/2), bar[j].rect.h};
						SDL_SetRenderDrawColor(gRenderer, 99, 99, 99, 155);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}*/
				}

				// Applies to all PauseMenu text
				for (int i=0; i<barMax; i++) {
					// Mouse on PauseMenu text
					if (bar[i].mouse) {
						SDL_SetRenderDrawColor(gRenderer, 255, 144, 20, 155);
						SDL_RenderDrawRect(gRenderer, &bar[i].rect);
					}

					// PauseMenu option highlighted
					if (bar[i].highlight) {
						SDL_Rect tempr {bar[i].rect.x, bar[i].rect.y, bar[i].rect.w, bar[i].rect.h};
						SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 155);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}
				}

				/* Render Apply Bar and Video button */
				for (int i=0; i<2; i++) {
					SDL_Rect temprV {applyButton[i].x, applyButton[i].y, applyButton[i].w, applyButton[i].h};
					// Hover
					if (applyMouse[i]) {
						SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 155);
						if (leftclick) {
							SDL_SetRenderDrawColor(gRenderer, 0, 125, 0, 155);
						}
					// No Hover
					}else if (!applyMouse[i]) {
						SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 155);
					}
					SDL_RenderDrawRect(gRenderer, &temprV);
					if (i==0) {
						gText.loadFromRenderedText(gRenderer, "APPLY AUDIO", {255,255,255}, gFont26);
					}
					if (i==1) {
						gText.loadFromRenderedText(gRenderer, "APPLY VIDEO", {255,255,255}, gFont26);
					}
					gText.render(gRenderer, applyButton[i].x+applyButton[i].w/2-gText.getWidth()/2,
							applyButton[i].y+applyButton[i].h/2-gText.getHeight()/2,
												 gText.getWidth(), gText.getHeight());
				}

				/* Render Keep and Revert prompt */
				if (confirm) {

					// Keep button
					confirmButton[0].w = 125;
					confirmButton[0].h = 48;
					confirmButton[0].x = helper.screenWidth/2-confirmButton[0].w/2-138;
					confirmButton[0].y = 184;

					// Revert button
					confirmButton[1].w = 125;
					confirmButton[1].h = 48;
					confirmButton[1].x = helper.screenWidth/2-confirmButton[1].w/2+138;
					confirmButton[1].y = 184;

					/* Mouse on Keep or Revert button */
					for (int i=0; i<2; i++) {
						if (helper.checkCollision(mx, my, 1, 1, confirmButton[i].x, confirmButton[i].y, confirmButton[i].w, confirmButton[i].h)) {
							confirmMouse[i] = true;
						}
						else{
							confirmMouse[i] = false;
						}
					}

					// Start timer
					timer -= 1;

					/* Player response time out */
					if (timer <= 0) {
						timer = 2552;
						confirm = false;

						// Revert to old Bar Settings
						if (button=="a") {
							applyOldAudioCFG();
							bar[0].value	= MASTER_VOL;
							bar[1].value	= MUSIC_VOL;
							bar[2].value	= SFX_VOL;
						}

						// Revert to old Video Settings
						if (button=="v") {
							bar[3].value	= RESOLUTION;
							bar[4].value	= ANTI_ALIAS;
							bar[5].value	= VSYNC;
							bar[6].value	= FULLSCREEN;
							gWindow.applySettings(RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN);
						}
					}
					// Render confirmation text
					std::stringstream tempss;
					tempss << "Keep these display settings? Reverting to previous display settings in " << timer/60 << ".";
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26);
					gText.render(gRenderer, helper.screenWidth/2-gText.getWidth()/2,
												 150,
												 gText.getWidth(),
												 gText.getHeight());

					/* Keep & Revert button */
					for (int i=0; i<2; i++) {
						if (confirmMouse[i]) {
							SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 155);
							if (leftclick) {
								SDL_SetRenderDrawColor(gRenderer, 0, 125, 0, 155);
							}
						// No Hover
						}else if (!confirmMouse[i]) {
							SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 155);
						}
						SDL_RenderDrawRect(gRenderer, &confirmButton[i]);
						if (i==0) {
							gText.loadFromRenderedText(gRenderer, "Keep", {255,255,255}, gFont26);
						}else{
							gText.loadFromRenderedText(gRenderer, "Revert", {255,255,255}, gFont26);
						}
						gText.render(gRenderer, confirmButton[i].x+confirmButton[i].w/2-gText.getWidth()/2,
													 confirmButton[i].y+confirmButton[i].h/2-gText.getHeight()/2,
													 gText.getWidth(), gText.getHeight());
					}
				}
			}

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());

		// fps
		/*frame++;
		if (cap && fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
			SDL_Delay((1000/FRAMES_PER_SECOND ) - fps.get_ticks());
		}*/
	}
}

//Get's input from user and returns it
void Options::SaveLevel(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop,
						std::string SpawnCoordinatesData,
						std::string TileSaveData,
						std::string PirateSaveData){

	// Set to False so Player can see their mouse
	SDL_SetRelativeMouseMode(SDL_FALSE);

	// Show cursor
	SDL_ShowCursor(true);

	// Loop bool
	bool getInputSave = true;

	// Store user input in string
	std::string inputDirectory = "file name...";

	// What you are currently typing
	std::string typing = "nothing";
	int typingAnim = 0;
	std::string slash = "";

	//Start getting input from user
	SDL_StartTextInput();

	//Mouse properties
	int mx, my;

	//input text box
	SDL_Rect customRect = {0,0,0,0};
	SDL_Event e;

	while (getInputSave) {

		// Start FPS cap
		helper.fps.start();

		//Mouse Position
		SDL_GetMouseState(&mx, &my);

		// get new mouse coordinates based on render size, and actual screen size
		int renderW = 0;
		int renderHDummy = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
		int en = renderW * 0.4375;
		int renderH = renderW - en;
		mx = (helper.screenWidth*mx)/renderW;	// New mouse coordinates, no relation to camera
		my = (helper.screenHeight*my)/renderH;	// New mouse coordinates, no relation to camera

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				getInputSave = false;
				optionsResult	= Exit;
			}

			if (typing == "   "){
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE&& inputDirectory.length() > 0) {
						inputDirectory.pop_back();
					}
					else if (e.key.keysym.sym== SDLK_c&& SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(inputDirectory.c_str());
					}
					else if (e.key.keysym.sym== SDLK_v&& SDL_GetModState() & KMOD_CTRL) {
						inputDirectory = SDL_GetClipboardText();
					}
				}
				else if (e.type == SDL_TEXTINPUT) {
					if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')&& (e.text.text[0] == 'v' || e.text.text[0] == 'V')&& SDL_GetModState() & KMOD_CTRL)) {
						inputDirectory += e.text.text;
					}
				}
			}
			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			//If Key Pressed
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					getInputSave = false;			//quit current loop
					break;
				case SDLK_RETURN:
					std::stringstream defaultDir;
					defaultDir << "resource/data/maps/";
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//----------------------------- Save Spawn Point ----------------------------//
					//Save # of Blocks
					std::ofstream spawnPointFile;
					std::stringstream PathDir;
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "spawn.mp";
					// Open File
					spawnPointFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					spawnPointFile << SpawnCoordinatesData;
					// Close
					spawnPointFile.close();
					//----------------------------- Save Spawn Point ----------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//------------------------------ Save Tile Data -----------------------------//
					//Save # of Blocks
					std::ofstream newTileFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Tile.mp";
					// Open File
					newTileFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newTileFile << TileSaveData;
					// Close
					newTileFile.close();
					//------------------------------ Save Tile Data -----------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////

					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//------------------------------ Save Pirate Data ---------------------------//
					//Save # of Blocks
					std::ofstream newPirateFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Pirate.mp";
					// Open File
					newPirateFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newPirateFile << PirateSaveData;
					// Close
					newPirateFile.close();
					//------------------------------ Save Pirate Data ---------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					getInputSave = false;
				}
			}

			//Left mouse click
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					//
				}
				if ( helper.checkCollision(mx, my, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
					if (typing != "filename"){
						typingAnim = 30;
						typing = "filename";
					}
				} else{
					typingAnim = 0;
					typing = "nothing";
				}
			}
		}

		//Typing timer animation
		if (typing == "filename"){
			typingAnim += 1;
			if (typingAnim >= 0 && typingAnim < 60) {
				slash = "_";
			}
			if (typingAnim >= 60 && typingAnim <= 120) {
				slash = " ";
			}
			if (typingAnim > 120) {
				typingAnim = 0;
			}
		}
		//Add the slash animation only when the player is typing the file name
		std::stringstream tempSS;
		if (typing == "filename") {
			tempSS << inputDirectory.c_str() << slash;
		} else {
			tempSS << inputDirectory.c_str();
		}

		//Clear screen for render
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
		SDL_RenderClear(gRenderer);

			//Render text tip
			gText.loadFromRenderedText(gRenderer, "Enter level directory (ending with a '/'):", {255,255,255}, gFont26);
			int newWidth = gText.getWidth()/2;
			int newHeight = gText.getHeight()/2;
			gText.render(gRenderer, 5, 5, newWidth, newHeight);

			//Render text input
			gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {0,255,0}, gFont26);
			customRect.x = 10;
			customRect.y = 15;
			newWidth = gText.getWidth()/2;
			newHeight = gText.getHeight()/2;
			gText.render(gRenderer, customRect.x+4, customRect.y +newHeight - 2, newWidth, newHeight);
			customRect.w = newWidth+8;
			customRect.h = newHeight * 2;
			if ( helper.checkCollision(mx, my, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}else{
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}

		//Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());
	}

	// Bound mouse to window
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Hide mouse
	SDL_ShowCursor(false);
}

std::string Options::GetInput(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop, std::string hint)
{
	// Set to False so Player can see their mouse
	SDL_SetRelativeMouseMode(SDL_FALSE);

	// Show cursor
	SDL_ShowCursor(true);

	// Loop bool
	bool getInputSave = true;

	// Store hint hear to give the Editor an idea of what they are typing
	std::string textField = "128 128";
	//std::string textField = "<keysRequired> <LevelWidth> <LevelHeight>";

	// What you are currently typing
	std::string typing = "nothing";
	int typingAnim = 0;
	std::string slash = "";

	//Start getting input from user
	SDL_StartTextInput();

	//Mouse properties
	int mx, my;

	//input text box
	SDL_Rect customRect = {0,0,0,0};
	SDL_Event e;

	while (getInputSave) {

		// Start FPS cap
		helper.fps.start();

		//Mouse Position
		SDL_GetMouseState(&mx, &my);

		// get new mouse coordinates based on render size, and actual screen size
		int renderW = 0;
		int renderHDummy = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
		int en = renderW * 0.4375;
		int renderH = renderW - en;
		mx = (helper.screenWidth*mx)/renderW;	// New mouse coordinates, no relation to camera
		my = (helper.screenHeight*my)/renderH;	// New mouse coordinates, no relation to camera

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				getInputSave = false;
				optionsResult	= Exit;
			}

			if (typing == "inputBox"){
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE&& textField.length() > 0) {
						textField.pop_back();
					}
					else if (e.key.keysym.sym== SDLK_c&& SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(textField.c_str());
					}
					else if (e.key.keysym.sym== SDLK_v&& SDL_GetModState() & KMOD_CTRL) {
						textField = SDL_GetClipboardText();
					}
				}
				else if (e.type == SDL_TEXTINPUT) {
					if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')&& (e.text.text[0] == 'v' || e.text.text[0] == 'V')&& SDL_GetModState() & KMOD_CTRL)) {
						textField += e.text.text;
					}
				}
			}
			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			//If Key Pressed
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					getInputSave = false;			//quit current loop
					break;
				case SDLK_RETURN:
					getInputSave = false;
				}
			}

			//Left mouse click
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					//
				}
				if ( helper.checkCollision(mx, my, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
					if (typing != "inputBox"){
						typingAnim = 30;
						typing = "inputBox";
					}
				} else{
					typingAnim = 0;
					typing = "nothing";
				}
			}
		}

		//Typing timer animation
		if (typing == "inputBox"){
			typingAnim += 1;
			if (typingAnim >= 0 && typingAnim < 60) {
				slash = "_";
			}
			if (typingAnim >= 60 && typingAnim <= 120) {
				slash = " ";
			}
			if (typingAnim > 120) {
				typingAnim = 0;
			}
		}
		//Add the slash animation only when the player is typing the file name
		std::stringstream tempSS;
		if (typing == "inputBox") {
			tempSS << textField.c_str() << slash;
		} else {
			tempSS << textField.c_str();
		}

		//Clear screen for render
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
		SDL_RenderClear(gRenderer);

			//Render text tip
			gText.loadFromRenderedText(gRenderer, hint.c_str(), {255,255,255}, gFont13);
			gText.render(gRenderer, 5, 5,gText.getWidth(), gText.getHeight());

			//Render text input
			gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {0,255,0}, gFont13);
			customRect.x = 10;
			customRect.y = 20;
			gText.render(gRenderer, customRect.x+4, customRect.y,gText.getWidth(), gText.getHeight());
			customRect.w = gText.getWidth()+8;
			customRect.h = gText.getHeight();
			if ( helper.checkCollision(mx, my, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}else{
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}

		//Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());
	}
	return textField.c_str();
}

// Key Pressed
void Options::OnKeyDown(SDL_Keycode sym, Players &player) {
	switch (sym) {
	case SDLK_UP:
		if (index > 0) {
			index -= 1;
		}
		break;
	case SDLK_DOWN:
		int typeShown;
		if (type==0) {
			typeShown=4;
		}else if (type==2) {
			typeShown=2;
		}
		if (index < typeShown) {
			index += 1;
		}
		break;
	case SDLK_RETURN:
		// PauseMenu
		if (type==0)
		{
			if (index==0) {
				player.returned = true;
				pauseLoop 		= false;
			}
			// Enter Settings
			else if (index==2) {
				// Change type
				type = 2;

				// Change options
				title[0].name = "AUDIO";
				title[1].name = "VIDEO";
				title[2].name = "BACK";
				/*title[2].name = "CONTROLLER";
				title[3].name = "MOUSE AND KEYBOARD";
				title[4].name = "BACK";*/
				HIGHLIGHT_INDEX = -1;
			}
			// Exit To Main Menu
			else if (index==3) {
				optionsResult	= Back;
				pauseLoop 		= false;
				//gameLoop 		= false;
				//selection 	= 0;
			}
			// Exit To Desktop
			else if (index==4) {
				optionsResult	= Exit;
				pauseLoop 		= false;
				//gameLoop 	= false;
				//selection = -1;
			}
		}
		// SubMenu
		else if (type==2) {
			if (index==0) {
				HIGHLIGHT_INDEX = 0;
			}else if (index==1) {
				HIGHLIGHT_INDEX = 1;
			}else if (index==2) {
				// Change type
				type = 0;

				// Change options
				title[0].name = "RESUME";
				title[1].name = "HOW TO PLAY";
				title[2].name = "SETTINGS";
				title[3].name = "EXIT MAIN MENU";
				title[4].name = "EXIT TO DESKTOP";
				HIGHLIGHT_INDEX = -1;
			}
		}
		break;
	}
}

// Mouse Pressed
void Options::mousePressed(Players &player) {
	for (int i=0; i<5; i++) {
		if (helper.checkCollision(mx, my, 1, 1, title[i].x-3, title[i].y-3, title[i].w+3, title[i].h+3)) {
			// PauseMenu controls
			if (type==0) {
				// Resume Game
				if (index==0) {
					player.returned = true;
					pauseLoop 		= false;
				}
				// Go into 'Settings'
				else if (index==2) {
					// Change type
					type = 2;
					// Change options
					title[0].name = "AUDIO";
					title[1].name = "VIDEO";
					title[2].name = "BACK";
					/*title[2].name = "CONTROLLER";
					title[3].name = "MOUSE AND KEYBOARD";
					title[4].name = "BACK";*/
					HIGHLIGHT_INDEX = -1;
				}
				// Exit To Main Menu
				else if (index==3) {
					optionsResult	= Back;
					pauseLoop 		= false;
					//gameLoop 		= false;
					//selection 	= 0;
				}
				// Exit To Desktop
				else if (index==4) {
					optionsResult	= Exit;
					pauseLoop 		= false;
					//gameLoop 	= false;
					//selection = -1;
				}
			// Settings controls
			}else if (type==2) {
				if (index==0) {
					HIGHLIGHT_INDEX = 0;
				}else if (index==1) {
					HIGHLIGHT_INDEX = 1;
				}else if (index==2) {
					HIGHLIGHT_INDEX = 2;
					// Change type
					type = 0;
					// Change options
					title[0].name = "RESUME";
					title[1].name = "HOW TO PLAY";
					title[2].name = "SETTINGS";
					title[3].name = "EXIT MAIN MENU";
					title[4].name = "EXIT TO DESKTOP";
					HIGHLIGHT_INDEX = -1;
				}
			}
		}
	}
}

// Update buttons
void Options::mouseReleased(LWindow &gWindow) {

	/* Mouse on Keep or Revert button */
	for (int i=0; i<2; i++) {
		if (helper.checkCollision(mx, my, 1, 1, confirmButton[i].x, confirmButton[i].y, confirmButton[i].w, confirmButton[i].h)) {
			confirmMouse[i] = true;
		}
		else{
			confirmMouse[i] = false;
		}
	}

	/* Apply Audio Button */
	if (helper.checkCollision(mx, my, 1, 1, applyButton[0].x, applyButton[0].y, applyButton[0].w, applyButton[0].h))
	{
		// Apply new Bar Settings
		/*Mix_VolumeMusic();
		// Apply new SFX Bar Settings
		Mix_VolumeChunk(sRockBreak, 	bar[2].value*(bar[0].value*0.01));
		Mix_VolumeChunk(sLazer, 		bar[2].value*(bar[0].value*0.01));
		Mix_VolumeChunk(sAtariBoom, 	bar[2].value*(bar[0].value*0.01));
		Mix_VolumeChunk(sGrenade, 		bar[2].value*(bar[0].value*0.01));*/


		// Apply new Bar Settings and new SFX Bar Settings
		applyCustomAudioCFG(bar[1].value*(bar[0].value*0.01), bar[2].value*(bar[0].value*0.01));
		// player preview of new settings
		Mix_PlayChannel(-1, sLazer, 0);
		//applyOldAudioCFG();

		// Display confirm prompt
		confirm			= true;
		timer 			= 2552;
		button = "a";
	}

	/* Apply Video Button */
	if (helper.checkCollision(mx, my, 1, 1, applyButton[1].x, applyButton[1].y, applyButton[1].w, applyButton[1].h))
	{
		// Apply new Video Settings
		gWindow.applySettings(bar[3].value, bar[4].value, bar[5].value, bar[6].value);

		// Display confirm prompt
		confirm			= true;
		timer 			= 2552;
		button = "v";
	}

	/* Keep Button */
	if (helper.checkCollision(mx, my, 1, 1, confirmButton[0].x, confirmButton[0].y, confirmButton[0].w, confirmButton[0].h))
	{
		// file name .cfg
		std::string temps;

		// file data
		std::stringstream tempss;

		// Turn off confirm prompt
		confirm 		= false;
		timer 			= 2552;

		// Keep new Audio Settings
		if (button=="a") {
			MASTER_VOL		= bar[0].value;
			MUSIC_VOL		= bar[1].value;
			SFX_VOL			= bar[2].value;
			temps = "cfg/audio.cfg";
			tempss << MASTER_VOL << " "
				   << MUSIC_VOL  << " "
				   << SFX_VOL;
		}

		// Keep new Video Settings
		if (button=="v") {
			RESOLUTION		= bar[3].value;
			ANTI_ALIAS		= bar[4].value;
			VSYNC			= bar[5].value;
			FULLSCREEN		= bar[6].value;

			temps = "cfg/video.cfg";
			tempss << RESOLUTION << " "
				   << ANTI_ALIAS << " "
				   << VSYNC 	 << " "
				   << FULLSCREEN;
		}

		// save .cfg file
		std::ofstream fileSettings;
		fileSettings.open( temps.c_str() );
			fileSettings << tempss.str().c_str();
		fileSettings.close();
	}

	/* Revert Button */
	if (helper.checkCollision(mx, my, 1, 1, confirmButton[1].x, confirmButton[1].y, confirmButton[1].w, confirmButton[1].h)) {
		// Turn off confirm prompt
		confirm 		= false;
		timer 			= 2552;

		// Revert to old Bar Settings
		if (button=="a") {
			applyOldAudioCFG();
			// player preview of new settings
			Mix_PlayChannel(-1, sLazer, 0);
			bar[0].value	= MASTER_VOL;
			bar[1].value	= MUSIC_VOL;
			bar[2].value	= SFX_VOL;
		}

		// Revert to old Video Settings
		if (button=="v") {
			bar[3].value	= RESOLUTION;
			bar[4].value	= ANTI_ALIAS;
			bar[5].value	= VSYNC;
			bar[6].value	= FULLSCREEN;
			gWindow.applySettings(RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN);
		}
	}
}
