/*
 * PlayGame.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "LTexture.h"
#include "Particless.h"

#include "PlayGame.h"

// TODO [ ] - fix players class so that we can easily create more players
// TODO [ ] - research how servers and clients talk to each other
// TODO [ ] - make it multiplayer

// TODO [ ] - make it so that when a grenade explodes, an Enemy will get hurt by it if its in the blast radius

void PlayGame::Init() {
	// Upon entry
    debug 				= false;
    editor				= false;
	quit 				= false;
	leftClick 			= false;
	shift 				= false;
	camx 				= 0;
	camy 				= 0;
	frame 				= 0;
    cap 				= true;

	// Initialize
	pira.Init(pirate);
	part.init(particles);
	aste.init(asteroid);
	enem.init(enemy);
	spaw.init(spawner);
	player.reset(map.x+map.w/2-player.w/2, map.y+map.h/2-player.h/2, "Player1", false);
	player.loadScore();
	tex.init(text);
}
/*
void PlayGame::saveCFG(std::string fileName){
	std::ofstream fileSettings;
	fileSettings.open( fileName.c_str() );
		std::stringstream tempss;
		tempss << MASTER_VOL << " "
			   << MUSIC_VOL  << " "
			   << SFX_VOL 	 << " "
			   << RESOLUTION << " "
			   << FULLSCREEN << " "
			   << VSYNC;
		fileSettings << tempss.str().c_str();
	fileSettings.close();
}

void PlayGame::loadCFG(std::string fileName){
	std::fstream fileSettings( fileName.c_str() );
	if (fileSettings.is_open()) {
		fileSettings >> MASTER_VOL >> MUSIC_VOL >> SFX_VOL >> RESOLUTION >> FULLSCREEN >> VSYNC;
		std::cout << "Loaded config.cfg" << std::endl;
	}else{
		std::cout << "Error opening config.cfg." << std::endl;
		std::cout << "\tCreating default config.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/config.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created config.cfg." << std::endl;
			newConfigFile << "100 128 128 2 2 2";
		}else{
			std::cout << "\t\tError creating config.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default config settings." << std::endl;
			MASTER_VOL			= 100;
			MUSIC_VOL			= 128;
			SFX_VOL				= 128;
			RESOLUTION			= 2;
			FULLSCREEN			= 2;
			VSYNC				= 2;
		}
		newConfigFile.close();
	}
	fileSettings.close();

	// Set Audio Settings
	Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sLazer, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sAtariBoom, SFX_VOL*(MASTER_VOL*0.01));

	// Set Video Settings
	gWindow.applySettings(RESOLUTION, FULLSCREEN, VSYNC);
}*/

// Load
void PlayGame::Load(LWindow &gWindow, SDL_Renderer *gRenderer) {

	// Load Video settings from file
	loadVideoCFG();

	// Load Audio settings from file
	loadAudioCFG();

	// Load audio files
	LoadAudioFiles();

	// Apply audio configurations
	applyOldAudioCFG();

	// load textures
	gBG.loadFromFile(gRenderer, 		"resource/gfx/bg.png");
	gCircle.loadFromFile(gRenderer, 	"resource/gfx/circle.png");

	// load fonts
	gFont 	= TTF_OpenFont("fonts/Viga-Regular.ttf", 24);
	gFont13 = TTF_OpenFont("fonts/Viga-Regular.ttf", 21);
	gFont26 = TTF_OpenFont("fonts/Viga-Regular.ttf", 26);

	// load particle textures
	gParticles.loadFromFile(gRenderer, "img/particle/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);

	//Load texture target
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );

	// load media for other classes
	pira.Load(gRenderer);
	part.load(gRenderer);
	aste.loadResources(gRenderer);
	enem.load(gRenderer);
	spaw.load(gRenderer);
	player.loadResources(gRenderer);

	// Apply video configurations
	//applyVideoCFG(gWindow);
}

void PlayGame::Free() {
	// free textures
	gParticles.free();
	gText.free();
	gBG.free();
	gCircle.free();

	// free fonts
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont26);
	gFont = NULL;
	gFont13 = NULL;
	gFont26 = NULL;

	// Free audio files
	FreeAudioFiles();

	// free media from other classes
	pira.Free();
	player.freeResources();
	part.free();
	enem.free();
	spaw.free();
	aste.freeResources();
}

/* 5-31-2017
 * Change asteroid spawns to pirates
 */


void PlayGame::Show(LWindow &gWindow, SDL_Renderer *gRenderer, PlayGame::Result &result) {
	// Initialize
	Init();

	// Load resources
	Load(gWindow, gRenderer);

    // Play Music, looped
	//Mix_FadeInMusic(sAmbientMusic, -1, 0);

	// While loop
	while (!quit) {

		// Start FPS cap
		fps.start();

		// Hides mouse, and traps mouse in game so it doesn't go out
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_ShowCursor(false);

		// Get mouse position
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
		mx = (screenWidth*mx)/gWindow.getWidth();	// New mouse coordinates, no relation to camera
		my = (screenHeight*my)/gWindow.getHeight();	// New mouse coordinates, no relation to camera

		//int mex = (1280*mx)/gWindow.getWidth();
		//int mey = (720*my)/gWindow.getHeight();
		int oldMX = mx+camx;								// New mouse coordinates, relation to camera
		int oldMY = my+camy;								// New mouse coordinates, relation to camera
		int clampSize = 32;									// Magnet pixel size
		int remainderW = oldMX % clampSize;
		int remainderH = oldMY % clampSize;
		int newMx = mx - remainderW;						// New mouse coordinates, locked on x32 coordinates
		int newMy = my - remainderH;						// New mouse coordinates, locked on x32 coordinates

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Close program
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					// Player Keys Released, categorized
					player.OnKeyDown(player, event.key.keysym.sym);

					// Local controls
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					case SDLK_h:
						debug = (!debug);
						break;
					case SDLK_p:
						editor = (!editor);
						break;
					case SDLK_ESCAPE:
						start(gWindow, gRenderer, player);
						/*pau.pause(pauseResult, quit, gWindow, gRenderer,
											  particles, part, player, fps,
											  MASTER_VOL, MUSIC_VOL, SFX_VOL,
											  RESOLUTION, FULLSCREEN, VSYNC,
											  sRockBreak, sLazer, sAtariBoom);*/
						break;
					}
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{
					// Player Keys Released, categorized
					player.OnKeyUp(player, event.key.keysym.sym);

					// Local controls
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}
				}
				//////////////////////////////////////

				/////////////////////////////////////

				// Update Player click state
				if (!editor) {
					// Player Mouse Click state
					player.mouseClickState(player, event);

					// Update Xbox 360 controls
					player.updateJoystick(player, event);
				}

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				//result = mouseReleased(event);

				// Customize Character results
				switch (optionsResult)  {
				case Back:				// Exit to Main Menu
					quit = true;
					break;
				case Nothing:
					//
					break;
				case StartGame:
					//
					break;
				case ShowingMenu:
					//
					break;
				case Exit:				// Exit Desktop
					result = Exit;
					quit = true;
					break;
				}

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

		// update everything
		Update(gWindow, gRenderer);

		//Set our Texture as our render target
		gTargetTexture.setAsRenderTarget(gRenderer);

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render foreground
			RenderFG(gRenderer, gWindow);

			// Render objects
			Render(gRenderer, gWindow);

			// Render text
			RenderDebug(gRenderer);

			// Render text
			RenderText(gRenderer, gWindow);

		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );

		//Show rendered to texture
		gTargetTexture.render( gRenderer, 0, 0, screenWidth, screenHeight, NULL, 0.0);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}
	// Free everything
	Free();
}

// Update everything
void PlayGame::Update(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Update Variables
	bool test;
	test = true;

	// Generate Stars
	if (test){
	//	part.genStars(particle, gWindow.getWidth(), gWindow.getHeight());
	}

	// Update Particles
	part.updateStarParticles(particles, map.x, map.y, map.w, map.h);
	part.updateBulletParticles(particles, map.x, map.y, map.w, map.h);

	// Update Asteroids
	aste.updateAsteroid(asteroid, particles, part,
								  player.alive, player.shield, player.score, enem.mileScore, player.health,
								  mx, my, camx, camy, gWindow.getWidth(), gWindow.getHeight(),
								  map.x, map.y, map.w, map.h);

	// Update Spawner: spawns an Asteroid
	//spaw.update(spawner, asteroid, aste, player.x+player.w/2, player.y+player.h/2, mx, my, camx, camy);

	// Update Player
	player.update(map,
				  asteroid, aste,
				  enemy, enem,
				  particles, part,
				  mx, my, camx, camy,
				  map.x+map.w/2-player.w/2, map.y+map.h/2-player.h/2,
				  gWindow, gRenderer,
				  gText, gFont26, {255,255,255},
				  sAtariBoom, sLazer, sGrenade,
				  sGrenadePickup, sPistolReload);

	// If Player is alive
	if (player.alive) {
		// update pirates
		pira.update(pirate, particles, part, map, player, sLazer, sPistolReload, camx, camy);

		// Update Enemy
		enem.update(enemy,
				particles, part,
					player.alive,
					player.x+player.w/2, player.y+player.h/2, player.w/2,
					player.score, map.x, map.y, map.w, map.h,
					sLazer, sAtariBoom);
	}

	// Update Player & Asteroid collision check
	//checkCollisionPlayerAsteroid(player, asteroid);

	// Update Player Particle & Asteroid collision check
	//checkCollisionParticleAsteroid(particles, part, asteroid, player);

	// Update Player Particle & Enemy collision check
	//checkCollisionParticleEnemy(particles, part, enemy, player);

	// Collision, particle & pirate
	checkCollisionParticlePirate();

	// Update Enemy Particle & Player collision check
	checkCollisionParticlePlayer(particles, part, player);

	// Collision: Grenade Particle & Pirate
	checkCollisionGrenadePlayer();

	// Update Asteroids: Wave re-spawn
	//spawnAsteroidsNow2();

	// Damage text: for pirate
	tex.update(text);

	// center camera on target
	{
		//camx = player.x + player.w / 2 - gWindow.getWidth()/2;
		//camy = player.y + player.h / 2 - gWindow.getHeight()/2;
		float bmx, bmy;
		bmx  = player.x+player.w/2-screenWidth/2;
		bmy  = player.y+player.h/2-screenHeight/2;
		float distance = sqrt((bmx - camx) * (bmx - camx)+
							  (bmy - camy) * (bmy - camy));

		// Camera target
		float vX, vY;
		if (distance >= 1){
			vX 	= 2 * (10*distance/256) * (bmx - camx) / distance;
			vY 	= 2 * (10*distance/256) * (bmy - camy) / distance;
			camx += vX;
			camy += vY;
		}

		// Camera bounds
		if( camx < 0 ){
			camx = 0;
		}
		if( camy < 0 ){
			camy = 0;
		}
		if( camx+screenWidth > map.w ){
			camx = map.w-screenWidth;
		}
		if( camy+screenHeight > map.h ){
			camy = map.h-screenHeight ;
		}
	}
}

// Render foreground
void PlayGame::RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow) {
	// render map
	for (int j=0; j<6; j++) {
		for (int i=0; i<6; i++) {
			gBG.render(gRenderer, map.x+i*837-camx, map.y+j*837-camy, 837, 837);
		}
	}
}

// Render everything
void PlayGame::Render(SDL_Renderer *gRenderer, LWindow &gWindow) {

	// Render Asteroid
	aste.renderAsteroid(asteroid, camx, camy, gRenderer);

	// Render Colonies that spawn Asteroids
	spaw.render(spawner, camx, camy, gRenderer);

	// Render Enemies
	enem.render(enemy, camx, camy, gRenderer);

	// Render our player
	player.render(mx, my, camx, camy, gWindow,
				gRenderer,
				gFont13, gFont26,
				{255,255,255}, part.count, gText);

	// Render pirate's
	pira.render(gRenderer, pirate, camx, camy);

	// Render particles
	part.renderStarParticle(particles, camx, camy, 1, gRenderer);
	part.renderBulletParticle(particles, camx, camy, 1, gRenderer);
}

// Render debug information
void PlayGame::RenderDebug(SDL_Renderer *gRenderer)
{
	// If debugging, show debug info
	if (debug) {
		/* Render Player debug */
		// Render circle
		gCircle.setColor(255,255,255);
		gCircle.setAlpha(180);
		gCircle.render(gRenderer, player.x2-camx,
								  player.y2-camy,
								  player.radius*2, player.radius*2);

		// Original box, untouched
		SDL_Rect tempRect = {player.x-camx, player.y-camy, player.w, player.h};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);
		// Box, centered
		tempRect = {player.x2-camx, player.y2-camy, player.w, player.h};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

		/* Render Particle debug */
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				// Original box, untouched
				SDL_Rect tempRect = {particles[i].x-camx, particles[i].y-camy, particles[i].w, particles[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
				// Box, centered
				tempRect = {particles[i].x2-camx, particles[i].y2-camy, particles[i].w, particles[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Render circle
				gCircle.setColor(255,255,255);
				gCircle.setAlpha(180);
				gCircle.render(gRenderer, particles[i].x-camx,
						particles[i].y-camy,
						particles[i].w, particles[i].h);

				// Render angle Text
				std::stringstream tempss;
				tempss << particles[i].deathTimer;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				//gText.render(gRenderer, particles[i].x - camx, particles[i].y-gText.getHeight() - camy, gText.getWidth(), gText.getHeight());
			}
		}

		/* Render Pirate debug */
		for (int i = 0; i < pira.max; i++) {
			if (pirate[i].alive) {
				// Original box, untouched
				SDL_Rect tempRect = {pirate[i].x-camx, pirate[i].y-camy, pirate[i].w, pirate[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
				// Box, centered
				tempRect = {pirate[i].x2-camx, pirate[i].y2-camy, pirate[i].w, pirate[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Render circle
				gCircle.setColor(255,255,255);
				gCircle.setAlpha(180);
				gCircle.render(gRenderer, pirate[i].x2-camx,
						pirate[i].y2-camy,
						pirate[i].w, pirate[i].h);

				// Render angle Text
				std::stringstream tempss;
				tempss << pirate[i].health;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				gText.render(gRenderer, pirate[i].x - camx, pirate[i].y-gText.getHeight() - camy, gText.getWidth(), gText.getHeight());

			}
		}
	}
}

// Render text
void PlayGame::RenderText(SDL_Renderer *gRenderer, LWindow &gWindow) {

	/* Render Player Health */
	for (int i=0; i<10; i++) {
		SDL_Rect tempRect = {24 + i * 10, screenHeight - 20 - 64 - 48, 6, 32};
		SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<10; i++) {
		SDL_Rect tempRect = {24 + i * 10, screenHeight - 20 - 64 - 48, 6, 32};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}

	/* Render Player Ammo */
	for (int i=0; i<player.ammoClip; i++) {
		SDL_Rect tempRect = {24 + i * 10, screenHeight - 20 - 64, 6, 32};
		SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<player.ammo; i++) {
		SDL_Rect tempRect = {24 + i * 10, screenHeight - 20 - 64, 6, 32};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}

	/* Render Player Grenades */
	for (int i=0; i<player.grenadesMax; i++) {
		SDL_Rect tempRect = {24 + i * 38, screenHeight - 20 - 24, 32, 24};
		SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<player.grenades; i++) {
		SDL_Rect tempRect = {24 + i * 38, screenHeight - 20 - 24, 32, 24};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<player.grenadesMax; i++) {
		if (player.grenades == i) {
			SDL_Rect tempRect = {24 + i * 38, screenHeight - 20 - 24, int((32*player.grenadesCD)/300), 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);
		}
	}

	/* Render Player Ammo */
	std::stringstream tempss;
	tempss << player.ammo << "/" << player.ammoClip;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.ammoClip * 10, screenHeight - 20 - 64, gText.getWidth(), gText.getHeight());

	/* Render Player Ammo */
	tempss.str(std::string());
	tempss << player.grenades << "/" << player.grenadesMax;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.grenadesMax * 36, screenHeight - 20 - 24 - 5, gText.getWidth(), gText.getHeight());

	// Render map
	//map.render(gRenderer, camx, camy);

	// Render any text spawned
	for (int i = 0; i < 100; i++) {
		if (text[i].alive) {
			std::stringstream tempss;
			tempss << text[i].textfield.c_str();
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont26);
			gText.setAlpha(text[i].alpha);
			gText.setColor(text[i].color.r, text[i].color.g, text[i].color.b);
			gText.render(gRenderer, text[i].x-gText.getWidth()/2 - camx, text[i].y-gText.getHeight()/2 - camy, gText.getWidth(), gText.getHeight());

			/*SDL_Rect tempRect = {text[i].x - camx,text[i].y - camy,
							   text[i].w,  text[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
			SDL_RenderFillRect(gRenderer, &tempRect);*/
		}
	}

	// pirate count
	tempss.str(std::string());
	tempss << "x: " << player.x << " y: " << player.y;
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont26);
	gText.setAlpha(255);
	gText.render(gRenderer, 0,0, gText.getWidth(), gText.getHeight());

}

// Mouse Pressed
PlayGame::Result PlayGame::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			//leftClick = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			// normal enemy
			pira.spawn(pirate, mx+camx-80/2, my+camy-80/2,
					  80, 80, 160, 160,
					  0.0, randDouble(3.6, 4.4), 0, 250,
					  40, 57, 17);
			// boss enemy
			/*pira.spawn(pirate, mx+camx-250/2, my+camy-250/2,
					  250, 250, 512, 512,
					  0.0, randDouble(3.6, 4.4), 1, 1000,
					  119, 256, -49);*/
		}
	}
	return Nothing;
}

// Mouse Released
PlayGame::Result PlayGame::mouseReleased(SDL_Event event){
	PlayGame::Result result = Nothing;
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// Global Functions ////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

// Check collision: Player and Asteroid
void PlayGame::checkCollisionPlayerAsteroid(Players &player, Asteroid asteroid[]) {
	/*// Player
	if (player.alive) {
		// Asteroid
		for (int i = 0; i < 128; i++) {
			if (asteroid[i].alive){
				// Asteroid collision target
				float bmx = player.x+player.w/2;
				float bmy = player.y+player.h/2;

				// Calculate distance
				float distance = sqrt((bmx - asteroid[i].x - asteroid[i].w / 2)* (bmx - asteroid[i].x- asteroid[i].w / 2) +
								      (bmy - asteroid[i].y - asteroid[i].h / 2)* (bmy - asteroid[i].y- asteroid[i].h / 2));

				// Collision occurred
				if (distance < player.w/2 + asteroid[i].w/2 && !player.shield){
					asteroid[i].health -= asteroid[i].health;
					player.health -= player.health;

					// play sound effect
					Mix_PlayChannel(-1, sAtariBoom, 0);
				}
			}
		}	// end Asteroid
	}	// end Player*/
}

// Check collision: Player Particle and Asteroid
void PlayGame::checkCollisionParticleAsteroid(Particle particle[], Particle &part, Asteroid asteroid[], Players &player) {
	/*// Particle
	for (int i = 0; i < part.max; i++) {
		if (particles[i].alive) {
			if (particles[i].type == 0){

				// Asteroid
				for (int j = 0; j < 128; j++) {
					if (asteroid[j].alive){
						if (particles[i].x + particles[i].w > asteroid[j].x && particles[i].x < asteroid[j].x + asteroid[j].w
								&& particles[i].y + particles[i].h > asteroid[j].y
								&& particles[i].y < asteroid[j].y + asteroid[j].h) {
							particles[i].collide = true;
						} else {
							particles[i].collide = false;
						}

						// Collision occurred
						if (particles[i].collide){
							//particles[i].time = 0;
							//particles[i].alive = false;
							//part.count--;

							asteroid[j].damaged = true;
							asteroid[j].health -= particles[i].damage;

							player.hits++;

							// play sound effect
							Mix_PlayChannel(-1, sRockBreak, 0);
						}
					}
				} // end Asteroid
			}
		}
	} // end Particle*/
}


// Check collision: Player Particle and Enemy
void PlayGame::checkCollisionParticleEnemy(Particle particle[], Particle &part, Enemy enemy[], Players &player) {
	/*//Enemy
	for (int j = 0; j < 8; j++) {
		if (enemy[j].alive){
			// Particle
			for (int i = 0; i < part.max; i++) {
				if (particles[i].alive) {
					if (particles[i].type == 0){
						if (particles[i].x + particles[i].w > enemy[j].x && particles[i].x < enemy[j].x + enemy[j].w
								&& particles[i].y + particles[i].h > enemy[j].y
								&& particles[i].y < enemy[j].y + enemy[j].h) {
							// remove particle
							//particles[i].time = 0;
							//particles[i].alive = false;
							//part.count--;

							// reduce enemy health
							enemy[j].damaged = true;
							enemy[j].health -= particles[i].damage;

							// add player hits
							player.hits++;
						}
					}
				}
			}	// end Particle
		}
	}	// end Enemy */
}

// Check collision: Player Particle and Enemy
void PlayGame::checkCollisionParticlePirate() {
	// Pirate
	for (int j = 0; j < 100; j++) {
		if (pirate[j].alive){
			// Particle
			for (int i = 0; i < part.max; i++) {
				if (particles[i].alive) {

					// Check only if Particle is NOT a Star Particle or a Grenade Particle
					if (particles[i].type != 2 && particles[i].type != 3){

						// Check only if Particle is NOT the Enemy's Particle
						if (particles[i].tag != pirate[j].tag){

							// particle collision with pirate using a CIRCLE
							float bmx = pirate[j].x2 + pirate[j].w/2;
							float bmy = pirate[j].y2 + pirate[j].h/2;
							float bmx2 = particles[i].x + particles[i].w/2;
							float bmy2 = particles[i].y + particles[i].h/2;
							float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
												  (bmy - bmy2) * (bmy - bmy2));
							// collision occurred
							if (distance < pirate[j].radius + particles[i].w/2) {

								// reduce pirate health
								pirate[j].health -= particles[i].damage;

								// spawn blood particle effect
								for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
									int rands = rand() % 9 + 2;
									float newX = pirate[j].x+pirate[j].w/2;
									float newY = pirate[j].y+pirate[j].h/2;
									part.spawnParticleAngle(particles, "none", 2,
														newX-rands/2,
														newY-rands/2,
													   rands, rands,
													   h, randDouble(2.1, 7.1),
													   0.0,
													   {255, 0, 0, 255}, 1,
													   1, 1,
													   rand() % 100 + 150, rand() % 2 + 7,
													   rand() % 50 + 90, 0,
													   true, 0.11,
													   true, randDouble(0.07, 0.69));
								}

								// remove particle
								particles[i].alive = false;
								part.count--;

								// spawn damage text
								std::stringstream tempss;
								tempss << particles[i].damage;
								int randw = rand() % int(pirate[j].w);
								tex.spawn(text, pirate[j].x+randw, pirate[j].y+pirate[j].h/2,
										  0.0, -3.7, 255, tempss.str().c_str(), {255,255, 0});
							}

							/*if (particles[i].x + particles[i].w > pirate[j].x && particles[i].x < pirate[j].x + pirate[j].w
									&& particles[i].y + particles[i].h > pirate[j].y
									&& particles[i].y < pirate[j].y + pirate[j].h) {
								particles[i].collide = true;
							} else {
								particles[i].collide = false;
							}

							// particle collide
							if (particles[i].collide){

								// remove particle
								particles[i].time = 0;
								particles[i].alive = false;
								part.count--;

								// reduce pirate health
								pirate[j].health -= particles[i].damage;

								// spawn particle effect
								for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
									int rands = rand() % 3 + 8;
									float newX = pirate[j].x+pirate[j].w/2;
									float newY = pirate[j].y+pirate[j].h/2;
									part.spawnParticleAngle(particles, 2,
														newX-rands/2,
														newY-rands/2,
													   rands, rands,
													   h, randDouble(0.1, 1.5),
													   0.0,
													   {255, 0, 0, 255}, 1,
													   1, 1,
													   rand() % 100 + 150, rand() % 2 + 5,
													   rand() % 50 + 90, 0,
													   false, 0,
													   rand() % 35 + 5);
								}

								// spawn damage text
								std::stringstream tempss;
								tempss << particles[i].damage;
								tex.spawn(text, pirate[j].x+pirate[j].w/2, pirate[j].y+pirate[j].h/2, 0.0, -2.7, 255, tempss.str().c_str());
							}*/
						}
					}
				}
			}	// end Particle
		}
	}			// end Pirate
}

// Check collision: Enemy Particle and Player
void PlayGame::checkCollisionParticlePlayer(Particle particle[], Particle &part, Players &player) {
	// Player
	if (player.alive && !player.shield){
		// Particle
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {

				// Check only if Particle is NOT a Star Particle
				if (particles[i].type != 2){

					// Check only if Particle is NOT the Enemy's Particle
					if (particles[i].tag != player.tag){

						// particle collision with player using a CIRCLE
						float bmx = player.x2 + player.w/2;
						float bmy = player.y2 + player.h/2;
						float bmx2 = particles[i].x + particles[i].w/2;
						float bmy2 = particles[i].y + particles[i].h/2;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						// collision occurred
						if (distance < player.radius + particles[i].w/2) {
							// reduce player health
							//player.health -= particles[i].damage;
							// spawn blood particle effect
							for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
								int rands = rand() % 11 + 3;
								float newX = player.x2+player.w/2;
								float newY = player.y2+player.h/2;
								part.spawnParticleAngle(particle, "none", 2,
													newX-rands/2,
													newY-rands/2,
												   rands, rands,
												   j, randDouble(2.1, 7.1),
												   0.0,
												   {255, 0, 144, 255}, 1,
												   1, 1,
												   rand() % 100 + 150, rand() % 2 + 7,
												   rand() % 50 + 90, 0,
												   true, 0.11,
												   true, randDouble(0.02, 0.69));
							}
							// remove particle
							particles[i].alive = false;
							part.count--;
						}

						// particle collision with player using a BOX
						/*if (particles[i].x + particles[i].w > player.x && particles[i].x < player.x + player.w
								&& particles[i].y + particles[i].h > player.y
								&& particles[i].y < player.y + player.h) {
							particles[i].time = 0;
							particles[i].alive = false;
							part.count--;

							player.health -= particles[i].damage;
						}*/
					}
				}
			}
		}	// end Particle
	}	// end Player
}

void PlayGame::checkCollisionGrenadePlayer() {
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				// Grenade particle
				if (particles[i].type == 3) {
					// get particle radius
					particles[i].radius = particles[i].w;

					// Particle movement
					particles[i].x += particles[i].vX * particles[i].speed;
					particles[i].y += particles[i].vY * particles[i].speed;

					// Speed decay of grenade
					if (particles[i].decay) {
						particles[i].speed = particles[i].speed - particles[i].speed * particles[i].decaySpeed;
					}

					// Particle spin
					particles[i].angle += particles[i].angleSpe * particles[i].angleDir;

					// Particle death
					particles[i].time += particles[i].deathTimerSpeed;

					// Particle map collision
					if (particles[i].x+particles[i].w < map.x) {
						particles[i].x = map.x+map.w-particles[i].w;
					}
					if (particles[i].x > map.x+map.w) {
						particles[i].x = map.x-particles[i].w;
					}
					if (particles[i].y+particles[i].h < map.y) {
						particles[i].y = map.y+map.h-particles[i].h;
					}
					if (particles[i].y > map.y+map.h) {
						particles[i].y = map.y-particles[i].h;
					}

					// Grenade has a smoke Particle effect coming out of it
					particles[i].timeri++;
					/* Making this '30' frames will enable the Player to
					 * watch 3 visual 'ticks' (particle sparks coming from the grenade
					 * that will explode momentarily after the 3 tick)
					 */
					if (particles[i].timeri > 30) {
						particles[i].timeri = 0;
						for (double h=0.0; h< 360.0; h+=30){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							part.spawnParticleAngle(particles, "none", 2,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   h, randDouble(2.1, 9.1),
											   0.0,
											   {255, 144, 10, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.11,
											   true, randDouble(0.01, 0.6));
						}
					}

					// Particle death
					if (particles[i].time > particles[i].deathTimer) {
						for (int j=0; j<pira.max; j++) {
							if (pirate[j].alive) {

								// pirate target
								float bmx = particles[i].x+particles[i].w/2;
								float bmy = particles[i].y+particles[i].h/2;
								float bmx2 = pirate[j].x+pirate[j].w/2;
								float bmy2 = pirate[j].y+pirate[j].h/2;

								// pirate distance from target
								float distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
								if (distance <= 0.5) {
									distance = 0.5;
								}
								// If within distance of explosion
								if (distance < 512){
								//if (distance < 104){
									// apply knock-back
									knockbackEffect(particles[i].x, particles[i].y, particles[i].w, particles[i].h,
													pirate[j].x, pirate[j].y, pirate[j].w, pirate[j].h, pirate[j].vX, pirate[j].vY, 15);
									// reduce health
									pirate[j].health -= particles[i].damage;

									// spawn damage text
									std::stringstream tempss;
									tempss << particles[i].damage;
									int randw = rand() % int(pirate[j].w);
									tex.spawn(text, pirate[j].x+randw, pirate[j].y+pirate[j].h/2,
											  0.0, -3.7, 255, tempss.str().c_str(), {255,255, 0});
								}
							}
						}

						// play sound effect
						Mix_PlayChannel(-1, sGrenadeExplode, 0);

						// spawn explosion particle effect
						for (double h=0.0; h< 360.0; h+=rand() % 10 + 2){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							/*int r = rand() % 255;
							int g = rand() % 255;
							int b = rand() % 255;
							SDL_Color color = {r, g, b};*/
							part.spawnParticleAngle(particles, "none", 2,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   h, randDouble(2.1, 9.1),
											   0.0,
											   {255, 144, 10, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.11,
											   true, randDouble(0.005, 0.6));
						}
						for (double h=0.0; h< 360.0; h+=rand() % 10 + 2){
							int rands = rand() % 9 + 2;
							float newX = particles[i].x+particles[i].w/2;
							float newY = particles[i].y+particles[i].h/2;
							part.spawnParticleAngle(particles, "none", 2,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   h, randDouble(2.1, 9.1),
											   0.0,
											   {255, 255, 0, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.11,
											   true, randDouble(0.0001, 0.001));
						}
						// remove particle
						particles[i].alive = false;
						part.count--;
					}
				}
			}
		}
}

// Spawn an Asteroid using a Spawn Point
void PlayGame::spawnAsteroidsNow2()
{
	// Increase number of Enemies spawned after "1000 * player.increment/2"
	/*if (e_dummy.mileScore > 1000 * (player.increment/2)){
		e_dummy.mileScore = 0;
		e_dummy.SPAWN_ENEMY = true;
		player.lives++;
	}*/

	// Spawn new wave of Enemies
	/*if (e_dummy.SPAWN_ENEMY){
		int randi, randw, randh;
			randi = rand() % 4 + 1;
			switch (randi){
				case 1:{
					//TODO - make Enemy X coordinate spawn more random
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
					}
					break;
				}
				case 2:{
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
					}
					break;
				}
				case 3:{
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
					}
					break;
				}
				case 4:{
					for (int j=0; j<e_dummy.ENEMY_SPAWN; j++){
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX+mapW + (64*randw), mapY+randh, 64, 32, -0.5, 0.0, 5);
						randw = rand() % 5 + 1;
						randh = rand() % 200;
						e_dummy.spawn(enemy, mapX-64 - (64*randw), mapY+randh, 64, 32, 0.5, 0.0, 5);
					}
					break;
				}
			}

		// Increase enemy count
		if (e_dummy.ENEMY_SPAWN < 5){
			e_dummy.ENEMY_SPAWN++;
		}

		// Stop spawning Enemies
		e_dummy.SPAWN_ENEMY 	= false;
	}*/

	// Check if there is 0 Asteroids
	if (pira.count <= 0){
		if (player.increment < 50) {
			player.increment += rand() % 3 + 3;
		}
		player.wave++;

		for (int i=0; i<40+player.increment; i++){
			int randx 		= rand() % map.w;
			int randy 		= rand() % map.h;
			//s_spawn.spawn(spawner, 0+randx, 0+randy, randw, randh);
			pira.spawn(pirate, map.x+randx, map.y+randy,
					  80, 80, 128, 128,
					  0.0, randDouble(3.6, 4.1), 0, 100,
					  11, 57, 17);
		}

		// Give Player a 180 ms shield
		player.shield			= true;
		player.shieldT			= 180;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////



void PlayGame::knockbackEffect(float targetX, float targetY, int targetW, int targetH,
							   float objectX, float objectY, int objectW, int objectH,
							   float &objectVX,float &objectVY, float force) {
	// pirate target
	float bmx2 = targetX+targetW/2;
	float bmy2 = targetY+targetH/2;
	float bmx  = objectX+objectW/2;
	float bmy  = objectY+objectH/2;

	// pirate distance from target
	float distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
	if (distance <= 0.5) {
		distance = 0.5;
	}
	// positive means going towards bmx, and bmy
	//float constVX = (bmx - bmx2) / distance;
	//float constVY = (bmy - bmy2) / distance;

	float angle = atan2(bmy - bmy2,bmx - bmx2);
	angle = angle * (180 / 3.1416);

	if (angle < 0) {
		angle = 360 - (-angle);
	}

	// positive means going towards bmx, and bmy
	objectVX += force * (cos( (3.14159265/180)*(angle) ));
	objectVY += force * (sin( (3.14159265/180)*(angle) ));
}
