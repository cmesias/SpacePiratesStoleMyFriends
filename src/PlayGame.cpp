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

#include "Engine/Particless.h"

#include "PlayGame.h"
#include "Engine/LTexture.h"

// TODO [ ] - fix players class so that we can easily create more players
// TODO [ ] - research how servers and clients talk to each other
// TODO [ ] - make it multiplayer

// TODO [ ] - make it so that when a grenade explodes, an Enemy will get hurt by it if its in the blast radius

// TODO 1-25-2022. (Players.cpp) [ ] - the players bullets wrap around the edges of the map, fix this by destroying bullets that go off the map

// TODO 1-25-2022. [ ] - Play SFX when an enemy is hit by a bullet from the player


// TODO 1-27-2022 [ ] - add "ctrl" variable
// TODO 1-27-2022 [ ] - fix when placing down a door or exit door, the placement is not lined up with the mouse, it is off center
// TODO 1-27-2022 [ ] - look at door states, such as when the player has enough keys then draw the exit door


// TODO 1-28-2022 [ ] - add tiles to init and update

void PlayGame::Init() {
	// Upon entry
    debug 				= true;
    editor				= true;
	camlock				= false;
	quit 				= false;
	leftClick 			= false;
	shift 				= false;
	frame 				= 0;
    cap 				= true;
	camx 				= 0;
	camy 				= 0;

	// Initialize

	// Tile class
	tl.Init(tile);

	// Tilebar class
	tb.Init(tilebar);
	tb.SpawnMultiple(tilebar);

	// Pirate class
	pira.Init(pirate);
	part.init(particles);

	// Asteroid class
	aste.init(asteroid);

	// Enemy class
	enem.init(enemy);

	// Spawner class
	spaw.init(spawner);

	// Player class
	player.reset(map.x+map.w/2-player.w/2, map.y+map.h/2-player.h/2, "Player1", false);

	// Player load score
	player.loadScore();

	// Text class
	tex.init(text);

	// Opened door, closed door, spawn tile
	rDoor[0] = {0, 0, 200, 200};
	rDoor[1] = {200, 0, 200, 200};
	rDoor[2] = {400, 0, 200, 200};
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
	gDoor.loadFromFile(gRenderer, 		"resource/gfx/cmesias/door.png");

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

	// Tile class load
	tl.Load(gRenderer);

	// Tilebar class load
	tb.Load(gRenderer);

	// Pirate class load
	pira.Load(gRenderer);

	// Particle class load
	part.load(gRenderer);

	// Asteroid class load
	aste.loadResources(gRenderer);

	// Enemy class load
	enem.load(gRenderer);

	// Spawner class load
	spaw.load(gRenderer);

	// Player class load
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
	tl.Free();
	//tb.Free();
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

	// Hides mouse, and traps mouse in game so it doesn't go out
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(false);

	// While loop
	while (!quit) {

		// Start FPS cap
		fps.start();

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

		// Editor mouse coordinates
		if (ctrl) {
			mouseX = mx;
			mouseY = my;
		}else{
			mouseX = newMx;
			mouseY = newMy;
		}

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
					// Local controls
					switch (event.key.keysym.sym) {
					case SDLK_LCTRL:
						ctrl = true;
						break;
					case SDLK_RCTRL:
						shift = true;
						break;
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
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

					///////////////////////////////////////////////////////////
					//------------------- Editor Controls -------------------//
					if (editor) {
						editorOnKeyDown(event.key.keysym.sym, part, particles);
					}

					///////////////////////////////////////////////////////////
					//------------------- Player Controls -------------------//
					else {
						// Player Keys Released, categorized
						player.OnKeyDown(player, event.key.keysym.sym);

					}
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{

					// Local controls
					switch (event.key.keysym.sym) {
					case SDLK_h:
						debug = (!debug);
						break;
					case SDLK_p:
						if (editor) {
							editor = false;
							camlock = true;
						} else {
							editor = true;
							camlock = false;
						}
						break;
					case SDLK_y:				// camera lock
						camlock = (!camlock);
						break;
					case SDLK_LCTRL:
						ctrl = true;
						break;
					case SDLK_RCTRL:
						shift = true;
						break;
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					case SDLK_m:
						if (editor) {
							std::stringstream mapSizeSS;
							mapSizeSS << GetInput(gWindow, gRenderer, quit, "Enter Keys required and Level Size (i.e.: 3 128 128): ");
							// Check if the Editor entered any numbers
							if (mapSizeSS.str().size() > 0) {
								mapSizeSS >> tl.requiredKeys >> tl.levelWidth >> tl.levelHeight;
							}
						}
						break;
					case SDLK_s:
						if (editor && !shift) {
							// Editor visual message
							std::stringstream tempss;
							tempss << "Saving level data...";
							tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());

							// Save Data for spawn coordinates
							std::stringstream SpawnCoordinatesData;
							SpawnCoordinatesData << spawnX << " " << spawnY << " " << doorX << " " << doorY;

							// Save Data for Tiles
							std::stringstream TileSaveData;
							TileSaveData << tl.SaveData(tile);

							// Save Data for Collision Tiles
							//std::stringstream CollisionTileSaveData;
							//CollisionTileSaveData << tc.saveTiles(tilec);

							// Go to saving interface
							SaveLevel(gWindow, gRenderer, quit,
									  SpawnCoordinatesData.str().c_str(),
									  TileSaveData.str().c_str());
							// Editor visual message
							tempss.str(std::string());
							tempss << "Finished saving.";
							tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());
						}
						break;
					}

					///////////////////////////////////////////////////////////
					//------------------- Editor Controls -------------------//
					if (editor) {
						editorOnKeyUp(event.key.keysym.sym);
					}
					///////////////////////////////////////////////////////////
					//------------------- Player Controls -------------------//
					else {
						// Player Keys Released, categorized
						player.OnKeyUp(player, event.key.keysym.sym);
					}
				}

				// Player mouse controls
				else{
					if (!editor) {
						// Player Mouse Click state
						player.mouseClickState(player, event);

						// Update Xbox 360 controls
						player.updateJoystick(player, event);
					}

				}

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

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				mouseReleased(event);

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

			// Render UI
			RenderUI(gRenderer);

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


	// Update Asteroids: Wave re-spawn
	if (!editor) {

		// Update tiles
		TilesUpdate();

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
		spaw.update(spawner, asteroid, aste, pirate, pira, player.x+player.w/2, player.y+player.h/2, mx, my, camx, camy);

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

		spawnAsteroidsNow2();

		// Damage text: for pirate
		tex.update(text);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	//---------------------------- Player's Camera & Editor's Camera ----------------------------//
	/*// center camera on target
	if (camlock) {
		// If Level is smaller than Render size, center camera on center of Level,
		// otherwise it follows the Player, and also is bounded on the Level Size
		if (tl.levelWidth <= 270 || tl.levelHeight <= 152) {
			camx = 0 + tl.levelWidth / 2 - screenWidth/2;
			camy = 0 + tl.levelHeight / 2 - screenHeight/2;
		}else{
			float angleTowardsMouse = atan2((my+camy) - player.y-player.h/2, (mx+camx) - player.x-player.w/2);
			angleTowardsMouse = angleTowardsMouse * (180/M_PI);
			if (angleTowardsMouse < 0) { angleTowardsMouse = 360 - (-angleTowardsMouse); }
			float radians   = (3.1415926536/180)*(angleTowardsMouse);
			float Cos 		= floor(cos(radians)*100+0.05)/100;
			float Sin 		= floor(sin(radians)*100+0.05)/100;

			// Camera target distance
			float bmx, bmy;
			bmx  = player.getCenterX()-screenWidth/2;
			bmy  = player.getCenterY()-screenHeight/2;
			float distance = sqrt((bmx - camx) * (bmx - camx)+
								  (bmy - camy) * (bmy - camy));

			// Camera target
			float vX = 0, vY = 0;
			if (distance > 0.5){
				vX 	= 2 * (10*distance/600) * (bmx - camx) / distance;
				vY 	= 2 * (10*distance/600) * (bmy - camy) / distance;
			}
			camx += vX;
			camy += vY;

			// Camera bounds
			if( camx < 0 ){
				camx = 0;
			}
			if( camy < 0 ){
				camy = 0;
			}
			if( camx+screenWidth > tl.levelWidth ){
				camx = tl.levelWidth-screenWidth ;
			}
			if( camy+screenHeight  > tl.levelHeight ){
				camy = tl.levelHeight-screenHeight ;
			}
		}
	}*/
	//---------------------------- Player's Camera & Editor's Camera ----------------------------//
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	// center camera on target
	if (camlock) {
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
		/*if( camx < 0 ){
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
		}*/
	}

	// Update camera
	if (camLeft) {
		camx -= 12;
	}
	if (camRight) {
		camx += 12;
	}
	if (camUp) {
		camy -= 12;
	}
	if (camDown) {
		camy += 12;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	//--------------------------------------- Editor Updates ------------------------------------//

	// update Monsters
	pira.EditorUpdate(pirate, mouseX+camx, mouseY+camy, mx+camx, my+camy, camx, camy);

	// Editor tile, item and pirate placement controls
	UpdateEditorTilePlacement();

	//--------------------------------------- Editor Updates ------------------------------------//
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
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

	// Render Tiles
	tl.Render(gRenderer, tile, 0, camx, camy);
	tl.Render(gRenderer, tile, 1, camx, camy);

	// Render spawn point and exit door
	{
		// Render Player spawn point
		gDoor.render(gRenderer, spawnX-camx, spawnY-camy, 200, 200, &rDoor[2]);
		/*SDL_Rect tempRect = {spawnX-camx, spawnY-camy, 16, 16};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 200, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);*/

		// Render Exit door
		if (player.collectedKeys >= tl.requiredKeys) {
			gDoor.render(gRenderer, doorX-camx, doorY-camy, 200, 200, &rDoor[1]);
		}else{
			gDoor.render(gRenderer, doorX-camx, doorY-camy, 200, 200, &rDoor[1]);
		}
	}

	// Render Asteroid
	aste.renderAsteroid(asteroid, camx, camy, gRenderer);

	// Render Colonies that spawn Asteroids
	spaw.render(spawner, camx, camy, gRenderer);

	// Render Enemies
	enem.render(enemy, camx, camy, gRenderer);

	// Render our player
	player.render(camx, camy, gWindow,
				gRenderer, part.count);

	// Render pirate's
	pira.render(gRenderer, pirate, camx, camy);

	// Render particles
	part.renderStarParticle(particles, camx, camy, 1, gRenderer);
	part.renderBulletParticle(particles, camx, camy, 1, gRenderer);

	// Render tile, appliances
	tl.Render(gRenderer, tile, 2, camx, camy);
	tl.Render(gRenderer, tile, 3, camx, camy);
	tl.Render(gRenderer, tile, 4, camx, camy);
	tl.Render(gRenderer, tile, 5, camx, camy);
	tl.Render(gRenderer, tile, 6, camx, camy);
}

void PlayGame::RenderUI(SDL_Renderer *gRenderer) {

	pira.RenderUI(gRenderer, pirate, camx, camy);
	player.RenderUI(gRenderer,
			{255,255,255},
			gText,
			gFont13, gFont26,
			mx, my,
			camx, camy,
			pira.count);
}

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
				tempRect = {pirate[i].xCenter-camx, pirate[i].yCenter-camy, pirate[i].w, pirate[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Render circle
				gCircle.setColor(255,255,255);
				gCircle.setAlpha(180);
				gCircle.render(gRenderer,
						pirate[i].getCenterX(pirate, i)-camx,
						pirate[i].getCenterY(pirate, i)-camy,
						pirate[i].w, pirate[i].h);

				// Render angle Text
				std::stringstream tempss;
				tempss << pirate[i].health;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				gText.render(gRenderer, pirate[i].x - camx, pirate[i].y-gText.getHeight() - camy, gText.getWidth(), gText.getHeight());

			}
		}

		// Render Map Box
		tempRect = {0-camx, 0-camy, tl.levelWidth, tl.levelHeight};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);
	}
}

// Render text
void PlayGame::RenderText(SDL_Renderer *gRenderer, LWindow &gWindow) {

	//  Render Lives left
	for (int i=0; i<3; i++) {
		SDL_Rect tempRect = {24 + i * 38, 20, 32, 48};
		SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<player.lives; i++) {
		SDL_Rect tempRect = {24 + i * 38, 20, 32, 48};
		SDL_SetRenderDrawColor(gRenderer, 200, 40, 40, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}

	// Render Player Health
	for (int i=0; i<player.maxHealth; i++) {
		SDL_Rect tempRect = {24 + i * 10, screenHeight - 20 - 64 - 48, 6, 32};
		SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}
	for (int i=0; i<player.health; i++) {
		SDL_Rect tempRect = {24 + i * 10, screenHeight - 20 - 64 - 48, 6, 32};
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
	}

	//  Render Player Ammo
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

	//  Render Player Grenades
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

	//  Render Player Ammo
	std::stringstream tempss;
	tempss << player.ammo << "/" << player.ammoClip;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.ammoClip * 10, screenHeight - 20 - 64, gText.getWidth(), gText.getHeight());

	// Render Player Ammo
	tempss.str(std::string());
	tempss << player.grenades << "/" << player.grenadesMax;
	gText.setAlpha(255);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
	gText.render(gRenderer, 28 + player.grenadesMax * 36, screenHeight - 20 - 24 - 5, gText.getWidth(), gText.getHeight());

	// Render map
	// map.render(gRenderer, camx, camy);

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
	//tempss.str(std::string());
	//tempss << "x: " << player.x << " y: " << player.y;
	//gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont26);
	//gText.setAlpha(255);
	//gText.render(gRenderer, 0,0, gText.getWidth(), gText.getHeight());



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//----------------------------------------------------- Render Editor's Hand -------------------------------------------//
	// Editor debug menu
	if (debug) {

		// Render collision tile debug
		/*for (int i = 0; i < tc.max; i++) {
			if (tilec[i].alive){
				if (tilec[i].mouse) {
					SDL_Rect tempr = {tilec[i].x+tilec[i].w/2-4-camx, tilec[i].y+tilec[i].h/2-4-camy, 8, 8};
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
					std::stringstream tempss;
					tempss << tilec[i].layer;
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
					gText.setAlpha(255);
					gText.render(gRenderer, tilec[i].x - camx, tilec[i].y - camy, gText.getWidth()/4, gText.getHeight()/4);
				}
				if (tilec[i].mouseBox) {
					SDL_Rect tempr = {tilec[i].x+4 - camx, tilec[i].y+4 - camy, tilec[i].w-8, tilec[i].h-8};
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}
		}*/

		// Render hand debug info
		std::stringstream tempss;
		tempss << "place_type: "  << place_type   	<< ", id: " 		 << tl.id
			   << ", layer: "     << tl.layer 		<< ", editor: " 	 << editor
			   << ", tl.multiW: " << tl.multiW    	<< ", tl.multiH: " << tl.multiH
			   << ", tc.multiH: " << tl.multiH
			   << ", tl.tilew: "  << tl.tilew     	<< ", tl.tileh: "  << tl.tileh
			   << ", tl.Count: "  << tl.tileCount
			   << ", camlock: " 	<< camlock 		<< ", part.count: " << part.count
			   << ", pira.type: " 	<< pira.type;
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont, 200);
		gText.setAlpha(255);
		gText.render(gRenderer, 0+screenWidth-gText.getWidth(), 100, gText.getWidth(), gText.getHeight());

		// Render tile debug
		if (debug){
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//------------------------------------- Render what's in Editors hand -------------------------------------//
			if (place_type == 0) {
				// Render Tile debug
				tl.RenderDebug(gRenderer, tile, mouseX, mouseY, mx, my, camx, camy, &rTiles[0], tb.tilesWidth);
			}else if (place_type == 1) {

				// Render tile in hand
				/*SDL_Color color;
				int tcWidth;
				int tcHeight;
				if (tc.type == 0) {
					color = {0, 0, 255};
					tcWidth = 16;
					tcHeight = 13;
				}else if (tc.type == 1) {
					color = {255, 0, 255};
					tcWidth = 16;
					tcHeight = 16;
				}else if (tc.type == 2) {
					color = {255, 255, 0};
					tcWidth = 16;
					tcHeight = 16;
				}
				for (int j = 0; j < tc.multiW; j++) {
					for (int h = 0; h < tc.multiH; h++) {
						//SDL_Rect tempr = {newMx, newMy, 32*multiW, 32*multiH};
						SDL_Rect tempr = {mouseX+j*tc.tilew, mouseY+h*tc.tileh, tcWidth, tcHeight};
						SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}
				}*/
			}else if (place_type == 2) {
				// Render Item in Hand
				// Render mouse coordinates snapped to grid
				///////////////obj.gItem.setAlpha(110);
				//////////////////obj.gItem.render(gRenderer, mouseX, mouseY, 16, 16, &obj.rClips[obj.id]);
				SDL_Rect tempRect = {mouseX, mouseY, 200, 200};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}else if (place_type == 3) {
				for (int j = 0; j < pira.multiW; j++) {
					for (int h = 0; h < pira.multiH; h++) {
						if (pira.type == 0) {
							pira.gTexture.setAlpha(110);
							pira.gTexture.render(gRenderer, mouseX+j*200, mouseY+h*200, 200, 200);
						} else if (pira.type == 1) {
							pira.gTexture.setAlpha(110);
							pira.gTexture.render(gRenderer, mouseX+j*200, mouseY+h*200, 200, 200);
						}
					}
				}

			}
			//------------------------------------- Render what's in Editors hand -------------------------------------//
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		// Render Tile Bar
		tb.Render(gRenderer, tilebar, tl.id);
	}
	//----------------------------------------------------- Render Editor's Hand -------------------------------------------//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// Render Editor UI
void PlayGame::RenderHand(SDL_Renderer *gRenderer) {
	if (debug){
		if (place_type == 0) {
			// Render Tile debug
			tl.RenderDebug(gRenderer, tile, mouseX, mouseY, mx, my, camx, camy, &rTiles[0], tb.tilesWidth);
		}
		// Render " " class objects in hand
		/*else if (place_type == 1) {

		}
		// Render item class objects in Hand
		else if (place_type == 2) {
			// Render Item in Hand
			// Render mouse coordinates snapped to grid
			obj.gItem.setAlpha(110);
			obj.gItem.render(gRenderer, mouseX, mouseY, 16, 16, &obj.rClips[obj.id]);
			SDL_Rect tempRect = {mouseX, mouseY, 16, 16};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}*/
	}
}

// Mouse Released
//PlayGame::Result PlayGame::mouseReleased(SDL_Event event){
//	PlayGame::Result result = Nothing;
////	if (event.type == SDL_MOUSEBUTTONUP) {
//		if (event.button.button == SDL_BUTTON_LEFT) {
//			leftClick = false;
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
//		}
//		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
//		}
//	}
//	return result;
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// Global Functions ////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void PlayGame::TilesUpdate() {

	// If tile layer goes below 0, loop around to 6
	if (tl.layer < 0) {
		tl.layer = 6;
	}

	// If tile layer goes above 6, loop around to 0
	if (tl.layer > 6) {
		tl.layer = 0;
	}

	// If tile id goes below 0, loop around to <uniqueTiles>
	if (tl.id < 0) {
		tl.id = tb.uniqueTiles;
	}

	// If tile id goes above  <uniqueTiles>, loop around to 0
	if (tl.id > tb.uniqueTiles) {
		tl.id = 0;
	}

	// Pirates
	if (pira.type < 0) {
		pira.type = 1;
	}
	if (pira.type > 1) {
		pira.type = 0;
	}

	// Loop around back to 3
	if (place_type > 3) {
		place_type = 0;
	}
}


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
							float bmx = pirate[j].xCenter + pirate[j].w/2;
							float bmy = pirate[j].yCenter + pirate[j].h/2;
							float bmx2 = particles[i].x + particles[i].w/2;
							float bmy2 = particles[i].y + particles[i].h/2;
							float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
												  (bmy - bmy2) * (bmy - bmy2));
							// collision occurred
							if (distance < pirate[j].radius + particles[i].w/2) {

								// Play impact sound effect
								Mix_PlayChannel(-1, sPirateHurt, 0);

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
								/*std::stringstream tempss;
								tempss << particles[i].damage;
								int randw = rand() % int(pirate[j].w);
								tex.spawn(text, pirate[j].x+randw, pirate[j].y+pirate[j].h/2,
										  0.0, -3.7, 255, tempss.str().c_str(), {255,255, 0});*/
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

							// Hurt player
							player.health -= particles[i].damage;
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
	if (spaw.count <= 0 && pira.count <= 0){


		// Next wave
		player.wave++;

		for (int i=0; i<1 + player.increment; i++){
			int randx 		= rand() % map.w;
			int randy 		= rand() % map.h;
			spaw.spawn(spawner, 0+randx, 0+randy, 128, 128);
			/*pira.spawn(pirate, map.x+randx, map.y+randy,
					  80, 80, 128, 128,
					  0.0, randDouble(3.6, 4.1),
					  11, 57, 17);

			pira.spawn(pirate, map.x+randx, map.y+randy,
					  80, 80, 128, 128,
					  0.0, randDouble(3.6, 4.1),
					  11, 57, 17);*/


			/*pira.spawn(pirate, map.x+randx, map.y+randy,
					  80, 80, 160, 160,
					  0.0, randDouble(3.6, 4.4),
					  40, 57, 17);*/
		}
		// Increment number of pirates to be spawned
		player.increment += rand() % 3 + 2;

		// Max increment, after reaching 50, stop increasing the number of spawns
		if (player.increment > 50) {
			player.increment = 50;
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------ Save Functions ------------------------------------------------------//


void PlayGame::ClearLevel(Particle &part, Particle particles[]) {
	tl.RemoveAll(tile);
	////////obj.RemoveAll(item);
	/////////part.RemoveAll(particles);
	pira.RemoveAll(pirate);
}


//------------------------------------------------------ Save Functions ------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------- Editor Controls --------------------------------------------//


// Mouse Pressed
PlayGame::Result PlayGame::mousePressed(SDL_Event event){

	// If holding a mouse button down
	if (event.type == SDL_MOUSEBUTTONDOWN) {

		// If holding down left mouse button
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
		}
		// If holding down right mouse button
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = true;
		}

		// If holding down middle mouse button
		if (event.button.button == SDL_BUTTON_MIDDLE) {

		}
	}
	return Nothing;
}

// Mouse Released
PlayGame::Result PlayGame::mouseReleased(SDL_Event event){
	PlayGame::Result result = Nothing;

	// If releasing a mouse button
	if (event.type == SDL_MOUSEBUTTONUP) {

		// If holding down left mouse button
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
		}

		// If holding down right mouse button
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = false;
		}

		// If holding down right middle button
		if (event.button.button == SDL_BUTTON_MIDDLE) {

		}
	}
	return result;
}



// Editor controls - Placing Tiles
void PlayGame::editorOnKeyDown( SDL_Keycode sym, Particle &part, Particle particles[] )
{
	switch (sym) {
	case SDLK_UP:				// camera up
		camUp = true;
		break;
	case SDLK_DOWN:				// camera down
		camDown = true;
		break;
	case SDLK_LEFT:				// camera left
		camLeft = true;
		break;
	case SDLK_RIGHT:			// camera right
		camRight = true;
		break;
	case SDLK_b:				// Change Tile collision properties
		if (shift) {
			tl.ChangeCollision(tile, 0);
		}else{
			tl.ChangeCollision(tile, 1);
		}
		break;
	case SDLK_v:				// Change Tile destructible properties
		if (shift) {
			tl.ChangeDestructable(tile, 0);
		}else{
			tl.ChangeDestructable(tile, 1);
		}
		break;
	case SDLK_x:				// Save spawn point
		spawnX = mouseX+camx;
		spawnY = mouseY+camy;
		break;
	case SDLK_u:				// Spawn Tile as a collision or not
		tl.collide = (!tl.collide);
		break;
	case SDLK_j:				// Spawn Tile as a destructible or not
		tl.destructible = (!tl.destructible);
		break;
	case SDLK_z:				// Save door spawn point
		doorX = mouseX+camx;
		doorY = mouseY+camy;
		break;
	case SDLK_w:
		if (shift) {
			tb.Move(tilebar, "up");
		}
		break;
	case SDLK_s:
		if (shift) {
			tb.Move(tilebar, "down");
		}
		break;
	case SDLK_a:
		if (shift) {
			tb.Move(tilebar, "left");
		}
		break;
	case SDLK_d:
		if (shift) {
			tb.Move(tilebar, "right");
		}
		break;
	case SDLK_q:								// Change place type (i.e. Tiles or Collision Tiles)
		place_type++;
		break;
	case SDLK_TAB:								// Toggle hide other layers
		tl.hideOtherLayers = (!tl.hideOtherLayers);
		break;
	case SDLK_k:								// Change clamp size
		if (shift){
			if (clampSize > 2) {
				clampSize -=2;
			}
		}else{
			clampSize+=2;
		}
		break;
	case SDLK_l:								// Change place_type Layer
		if (shift) {
			if (place_type==0) {
				tl.layer--;
			}else if (place_type==1) {
				//tc.layer--;
			}
		}else{
			if (place_type==0) {
				tl.layer++;
			}else if (place_type==1) {
				//tc.layer++;
			}
		}
		break;
	case SDLK_i:								// Change place_type Type
		if (shift) {
			if (place_type==0) {
				tl.id--;
			}else if (place_type==1) {
				//tc.type--;
			}else if (place_type==2) {
			//////////////	obj.id--;
			}else if (place_type==3) {
				pira.type--;
			}
		}else{
			if (place_type==0) {
				tl.id++;
			}else if (place_type==1) {
			//	tc.type++;
			}else if (place_type==2) {
			///////////////	obj.id++;
			}else if (place_type==3) {
				pira.type++;
			}
		}
		break;
	case SDLK_PERIOD:							// Tile, change Layer
		if (shift)
			tl.ChangeLayer(tile, -1);
		else
			tl.ChangeLayer(tile, 1);
		break;
	case SDLK_c:								// Tile, copy Tile
		if (editor) {
			if (place_type == 0 ) {
				tl.Copy(tile);
			}else if (place_type == 1) {
			//	tc.copy(tilec);
			}
		}
		break;
	case SDLK_SPACE:							// Tile, remove all Tiles (and Collision Tiles)
		if (shift && editor) {
			ClearLevel(part, particles);

			/*if (place_type == 0 ) {
			//	tl.removeAllTiles(tile);
			}else if (place_type == 1) {
			//	tc.removeAll(tilec);
			}*/
		}
		break;
	case SDLK_LEFTBRACKET:						// Tile, subtract block in x-axis
		if (place_type == 0) {
			// If shift, then change tile size
			if (shift) {
				if (tl.tilew > 1) {
					tl.tilew -= 1;
				}
			}
			else{
				// If not shift, then change tile amount placed
				if (tl.multiW > 1) {
					tl.multiW -= 1;
				}
			}
		}else if (place_type == 1) {
			if (shift) {
				//if (tc.tilew > 1) {
				//	tc.tilew -= 1;
				//}
			}
			else{
				//if (tc.multiW > 1) {
				//	tc.multiW -= 1;
				//}
			}
		}else if (place_type == 3) {
			if (pira.multiW > 1) {
				pira.multiW -= 1;
			}
		}
		break;
	case SDLK_RIGHTBRACKET:						// Tile, add block in x-axis or change tile size
		if (place_type == 0) {
			if (shift) {
				tl.tilew += 1;
			}
			else{
				tl.multiW += 1;
			}
		}else if (place_type == 1) {
			if (shift) {
				//tc.tilew += 1;
			}
			else{
				//tc.multiW += 1;
			}
		}else if (place_type == 3) {
			pira.multiW += 1;
		}
		break;
	case SDLK_MINUS:							// Tile, subtract block in y-axis or change tile size
		if (place_type == 0) {
			if (shift) {
				if (tl.tileh > 1) {
					tl.tileh -= 1;
				}
			}
			else{
				if (tl.multiH > 1) {
					tl.multiH -= 1;
				}
			}
		}else if (place_type == 1) {
			if (shift) {
				//if (tc.tileh > 1) {
				//	tc.tileh -= 1;
				//}
			}
			else{
				//if (tc.multiH > 1) {
				//	tc.multiH -= 1;
				//}
			}
		}else if (place_type == 3) {
			if (pira.multiH > 1) {
				pira.multiH -= 1;
			}
		}
		break;
	case SDLK_EQUALS:							// Tile, add block in y-axis
		if (place_type == 0) {
			if (shift) {
				tl.tileh += 1;
			}
			else{
				tl.multiH += 1;
			}
		}else if (place_type == 1) {
			if (shift) {
				//tc.tileh += 1;
			}
			else{
				//tc.multiH += 1;
			}
		}else if (place_type == 3) {
			pira.multiH += 1;
		}
		break;
	case SDLK_9: {								// Load Room
			/*tl.loadTiles(tile);
			tc.loadTiles(tilec);
			//loadcTiles(ctile);

			std::stringstream tempss;
			tempss << "Loading Tiles";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());*/

			break;
		}
	case SDLK_0: {								// Save Room
			/*tl.saveTiles(tile);
			tc.saveTiles(tilec);
			//savecTiles(ctile);

			std::stringstream tempss;
			tempss << "Saving Tiles";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());*/
			break;
		}
	}
}
void PlayGame::editorOnKeyUp( SDL_Keycode sym ) {
	switch (sym) {
	case SDLK_UP:
		camUp = false;
		break;
	case SDLK_DOWN:
		camDown = false;
		break;
	case SDLK_LEFT:
		camLeft = false;
		break;
	case SDLK_RIGHT:
		camRight = false;
		break;
	}
}
// tile and tilebar not showing up

void PlayGame::UpdateEditorTilePlacement() {

	// Editor
	if (editor) {
		if (leftClick) {
			/* If not on Tile-bar, place other tiles */
			if (!tb.touching) {
				//if (editor) {

					// Spawn a tile
					if (place_type == 0) {
						tl.SpawnMultiple(tile, mouseX, mouseY, camx, camy, &rTiles[0]);
					}

					// Spawn collision tiles (redacted)
					else if (place_type == 1) {
						// tc.spawn(tilec, mouseX, mouseY, camx, camy);
					}

					// Spawn items
					else if (place_type == 2) {
						// obj.Spawn(item, mouseX+camx, mouseY+camy, 16, 16);
					}

					// Spawn pirates
					else if (place_type == 3) {
						// normal enemy
						pira.spawn(pirate, mx+camx-80/2, my+camy-80/2,
								  80, 80, 160, 160,
								  0.0, randDouble(3.6, 4.4),
								  40, 57, 17);
						// boss enemy
						/*pira.spawn(pirate, mx+camx-250/2, my+camy-250/2,
								  250, 250, 512, 512,
								  0.0, randDouble(3.6, 4.4), 1, 1000,
								  119, 256, -49);*/
					}
				//}
			}else{
				tb.Select(tilebar, tl.id);
			}
		}
		if (rightClick) {
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {
				if (editor) {

					// Remove  tile
					if (place_type == 0) {
						tl.Remove(tile, 1);
					}

					// Remove collision tile
					else if (place_type == 1) {
						//	tc.remove(tilec, 1);
					}

					// Remove items
					else if (place_type == 2) {
						//	obj.Remove(item, 1);
					}

					// Remove pirate
					else if (place_type == 3) {
						pira.Remove(pirate, 1);
					}
				}
			}else{
				tb.Select(tilebar, tl.id);
			}
		}
	}
}


//-------------------------------------------- Editor Controls --------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



