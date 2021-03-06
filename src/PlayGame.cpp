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


// TODO 1-28-2022 [x] - add tiles to init and update

// DONE 1-31-2022 [x] - added ability to save and load tiles, pirates, spawn point for player and spawn point for door
// DONE 1-31-2022 [x] - added ability to save and load highest level achieved

// TODO 1-31-2022 [ ] - Find out why tiles are still not appearing??? Check each function in the Tile class and see if all functions are being used properly
// TODO 1-31-2022 [ ] - Implement a way for the user to choose a level stage


// TODO [ ] 4-3-2022 - Get main menu from smol dungeon, and integrate it into here next time because mega is akig at me

/*
 * EDITOR CONTROLS (2-1-2022)
 * H 					= Debug mode
 * P					= Editor mode/Player mode
 * I 					= Ascend tile type in hand
 * SHIFT + I 			= Descend tile type in hand
 * L					= Acend layer number in hand
 * SHIFT + L			= Descend layer number in hand
 *
 * Left Mouse Click 	= Place tile in world
 * Right Mouse Click 	= Delete tile in world
 * Y					= Camera lock
 *
 * 0					= Save number of keys to aquire, and set map size, also save level data such as tiles and other objects in the world
 *
 *
 *
 *
 *TODO [ ] (4-2-2022) - Add health drops for enemies somtimes, get code and sprite from Smol Dungeon
 */




void PlayGame::ShakeCamera()
{
	playerShakeCamera = false;

	camshake = true;
	rustleW = 5;
	rustleSpe = 1;

	rustleDirX = rand() % 3 + 1;
}

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

    // Camera
	camx 				= 0;
	camy 				= 0;
	rustleW				= 0.0;
	rustleSpe			= 1;
	rustleDirX			= 1;
	camshake			= false;
	playerShakeCamera	= false;

	// Initialize
	currentLevelStage = 1;
	playerStageLevel = -1;

	// Tile class
	tl.initTile(tile);

	// Tilebar class
	tb.init(tilebar);
	tb.placeTileBar(tilebar);

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
	//player.reset(0+tl.levelWidth/2-player.w/2, 0+tl.levelHeight/2-player.h/2, "Player1", false);

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
	gRect.loadFromFile(gRenderer, 		"resource/gfx/rect.png");

	// Tile Jars
	gJar.loadFromFile(gRenderer, "resource/gfx/jar_spritesheet.png");
	rJar[0] = {64*0,0,64,64};
	rJar[1] = {64*1,0,64,64};
	rJar[2] = {64*2,0,64,64};
	rJar[3] = {64*3,0,64,64};
	rJar[4] = {64*4,0,64,64};
	rJar[5] = {64*5,0,64,64};

	// load fonts
	gFont 	= TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 24);
	gFont13 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 21);
	gFont26 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 26);

	// load particle textures
	gParticles.loadFromFile(gRenderer, "resource/gfx/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);

	//Load texture target
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );

	// load media for other classes

	tl.load(gRenderer, &gJar, &flowerTiles, &flowerTiles2);
	tb.load(gRenderer);

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
	tl.free();
	tb.free();
	aste.freeResources();
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

	// Load resources
	//LoadLevel(part, particles);

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
		mex = (screenWidth*mx)/gWindow.getWidth();				// New mouse coordinates, no relation to camera
		mey = (screenHeight*my)/gWindow.getHeight();				// New mouse coordinates, no relation to camera

		//int mex = (1280*mx)/gWindow.getWidth();
		//int mey = (720*my)/gWindow.getHeight();
		int oldMX = mx+camx;								// New mouse coordinates, relation to camera
		int oldMY = my+camy;								// New mouse coordinates, relation to camera
		int clampSize = 32;									// Magnet pixel size
		int remainderW = oldMX % clampSize;
		int remainderH = oldMY % clampSize;
		newMx = mx - remainderW;						// New mouse coordinates, locked on x32 coordinates
		newMy = my - remainderH;						// New mouse coordinates, locked on x32 coordinates

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
					case SDLK_0:
						if (editor) {
							std::stringstream mapSizeSS;
							mapSizeSS << GetInput(gWindow, gRenderer, quit, "Enter Keys required and Level Size (i.e.: 3 128 128): ");
							// Check if the Editor entered any numbers
							if (mapSizeSS.str().size() > 0) {
								mapSizeSS >> tl.requiredKeys >> tl.levelWidth >> tl.levelHeight;
							}
						}
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

							// Save Data for Items
							std::stringstream PirateSaveData;
							PirateSaveData << pira.SaveData(pirate);

							// Go to saving interface
							SaveLevel(gWindow, gRenderer, quit,
									  SpawnCoordinatesData.str().c_str(),
									  TileSaveData.str().c_str(),
									  PirateSaveData.str().c_str());

							// Editor visual message
							tempss.str(std::string());
							tempss << "Finished saving.";
							tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());
						}
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
						ctrl = false;
						break;
					case SDLK_RCTRL:
						shift = false;
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
							/*// Editor visual message
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
							//SaveLevel(gWindow, gRenderer, quit,
							//		  SpawnCoordinatesData.str().c_str(),
							//		  TileSaveData.str().c_str());

							// Editor visual message
							//tempss.str(std::string());
							///tempss << "Finished saving.";
							//tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());*/
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

	// place_type
	if (place_type > 4) {
		place_type = 0;
	}

	// Update Asteroids: Wave re-spawn
	if (!editor) {

		// Update tiles
		tl.updateTile(tile, gWindow, player.getX(), player.getY(), player.w, player.h,
								   newMx+camx, newMy+camy,
								   mex+camx, mey+camy,
								   camx, camy,
								   &rTiles[0]);

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
									  player.alive, player.shield, enem.mileScore, player.health,
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
					  sGrenadePickup, sPistolReload, sPistolEmpty);

		// Shake camera if player wants to, but right not not used
		if (player.getShakeCameraStatus()) {
			player.StopShakeCameraPing();
			ShakeCamera();
		}

		// If Player is alive
		if (player.alive) {
			// update pirates
			pira.update(pirate, particles, part, map,
					player.x2, player.y2, player.score,
					sLazer, sPistolReload, camx, camy);

			// Update Enemy
			enem.update(enemy,
					particles, part,
						player.alive,
						player.x+player.w/2, player.y+player.h/2, player.w/2,
						player.score, map.x, map.y, map.w, map.h,
						sLazer, sAtariBoom);
		}

		// Collision check: Player & Health packs
		checkCollisionPlayerAsteroid(player, asteroid);

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

		// Spawn spawners that spawn Pirates
		spawnAsteroidsNow2();

		// Check if Pirate's health is 0
		checkPirateDied();

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
		if (!editor) {
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

		// Handle camera shake
		if (camshake) {

			// Shake going right
			if (rustleDirX == 1) {

				// Shake camera
				camx += rustleW;

				// Next direction to shake
				rustleDirX = 2;

				// Lower rustle of camera every frame
				if (rustleW > 0) {
					rustleW -= rustleSpe;
				} else {
					// Stop shaking
					camshake = false;
				}
			}

			// Shake going down
			else if (rustleDirX == 2) {

				// Shake camera
				camy += rustleW;

				// Next direction to shake
				rustleDirX = 3;

				// Lower rustle of camera every frame
				if (rustleW > 0) {
					rustleW -= rustleSpe;
				} else {
					// Stop shaking
					camshake = false;
				}
			}

			// Shake going left
			else if (rustleDirX == 3) {

				// Shake camera
				camx -= rustleW;

				// Next direction to shake
				rustleDirX = 4;

				// Lower rustle of camera every frame
				if (rustleW > 0) {
					rustleW -= rustleSpe;
				} else {
					// Stop shaking
					camshake = false;
				}
			}

			// Shake going up
			else if (rustleDirX == 4) {

				// Shake camera
				camy -= rustleW;

				// Next direction to shake
				rustleDirX = 1;

				// Lower rustle of camera every frame
				if (rustleW > 0) {
					rustleW -= rustleSpe;
				} else {
					// Stop shaking
					camshake = false;
				}
			}
		}
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
			gBG.render(gRenderer, 0+i*837-camx, 0+j*837-camy, 837, 837);
		}
	}
}

// Render everything
void PlayGame::Render(SDL_Renderer *gRenderer, LWindow &gWindow) {

	// Render Tiles
	// Render tile, ground below ground
	tl.renderTile(gRenderer, tile, -1, camx, camy, &rJar[0]);

	// Render tile, ground
	tl.renderTile(gRenderer, tile, 0, camx, camy, &rJar[0]);

	// Render Tile in behind player sprite
	tl.RenderBehindPlayer(gRenderer, tile, 1, camx, camy, &rTiles[0], &rJar[0]);

	// Render Tile in behind player sprite
	tl.RenderBehindPlayer(gRenderer, tile, 2, camx, camy, &rTiles[0], &rJar[0]);

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
			gDoor.render(gRenderer, doorX-camx, doorY-camy, 200, 200, &rDoor[0]);
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

	// Render Tile on top of player
	tl.RenderOnTopOfPlayer(gRenderer, tile, 1, camx, camy, &rTiles[0], &rJar[0]);

	// Render Tile on top of player
	tl.RenderOnTopOfPlayer(gRenderer, tile, 2, camx, camy, &rTiles[0], &rJar[0]);

	// Render pirate's
	pira.render(gRenderer, pirate, camx, camy);

	// Render particles
	part.renderStarParticle(particles, camx, camy, 1, gRenderer);
	part.renderBulletParticle(particles, camx, camy, 1, gRenderer);

	// Render Tile, ceiling
	tl.renderTile(gRenderer, tile, 3, camx, camy, &rJar[0]);
	tl.renderTile(gRenderer, tile, 4, camx, camy, &rJar[0]);
	tl.renderTile(gRenderer, tile, 5, camx, camy, &rJar[0]);
	tl.renderTile(gRenderer, tile, 6, camx, camy, &rJar[0]);
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

	// Render "E" prompt on doors to new areas
	tl.RenderUI(gRenderer, tile, camx, camy);
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

		// Render Particle debug
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				// Original box, untouched
				/*SDL_Rect tempRect = {particles[i].x-camx, particles[i].y-camy, particles[i].w, particles[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
				// Box, centered
				tempRect = {particles[i].x2-camx, particles[i].y2-camy, particles[i].w, particles[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);*/

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

		// Render Pirate debug
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
						pirate[i].xCenter-camx,
						pirate[i].yCenter-camy,
						pirate[i].w, pirate[i].h);

				// Render angle Text
				std::stringstream tempss;
				tempss << pirate[i].health;
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				gText.render(gRenderer, pirate[i].x - camx, pirate[i].y-gText.getHeight() - camy, gText.getWidth(), gText.getHeight());

			}
		}

		// Render Tile debug
		tl.renderTileDebug(gRenderer, tile, newMx, newMy, mex, mey, camx, camy, &rTiles[0]);

		// Render Map Box
		tempRect = {0-camx, 0-camy, tl.levelWidth, tl.levelHeight};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

		// Original box, untouched
		SDL_Rect tempRecte = {mx-32, my-32, 64, 64};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRecte);
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
			   << ", p_camshake: " 	<< playerShakeCamera
			   << ", camshake: " 	<< camshake
			   << ", rustleW: " 	<< rustleW
			   << ", rustleSpe: " 	<< rustleSpe
			   << ", rustleDirX: " 	<< rustleDirX;

		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont, 200);
		gText.setAlpha(255);
		gText.render(gRenderer, 0+screenWidth-gText.getWidth()*3, 100, gText.getWidth(), gText.getHeight());

		// Render tile debug
		if (debug){
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//------------------------------------- Render what's in Editors hand -------------------------------------//
			if (place_type == 0) {
				tl.RenderHand(gRenderer, tile, newMx, newMy, mex, mey, camx, camy, &rTiles[0]);
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
		tb.render(gRenderer, tilebar, tl.id);

		// Render confirmation prompt
		if (promptConfirm) {

			// Render box around text

			// Render: "Are you sure you want to save?"
			std::stringstream text;
			text << "Are you sure you want to ";
			if (promptType == 0)
				text << "Load level " << "?";
			else
				text << "Save level " << "?";

			// Prompt position
			int promptX;
			int promptY;
			gRect.setAlpha(200);
			gRect.setColor(240, 159, 80);

			// Prompt
			{
				// Get text
				gText.loadFromRenderedText(gRenderer, text.str().c_str(), {255,255,255}, gFont13);
				promptX = screenWidth * 0.5 - gText.getWidth()/2;
				promptY = screenHeight * 0.90 - gText.getHeight();

				// Render BG texture
				gRect.render(gRenderer, promptX-25, promptY-20, gText.getWidth()+50, gText.getHeight()+40);

				// Render text
				gText.render(gRenderer, promptX, promptY, gText.getWidth(), gText.getHeight());
			}

			// Render "YES"
			{
				// Get text
				gText.loadFromRenderedText(gRenderer, "YES", {0,144,0}, gFont13);
				promptX = screenWidth * 0.25 - gText.getWidth()/2;
				promptY = screenHeight * 0.50 - gText.getHeight()/2;

				// Render BG texture
				gRect.render(gRenderer, promptX-screenWidth/4, promptY-(screenWidth/3)/2, screenWidth/2, screenWidth/3);

				// Render text
				gText.render(gRenderer, promptX, promptY, gText.getWidth(), gText.getHeight());
			}

			// Render "NO"
			{
				// Get text
				promptX = screenWidth * 0.75 - gText.getWidth()/2;
				promptY = screenHeight * 0.50 - gText.getHeight()/2;
				gText.loadFromRenderedText(gRenderer, "NO", {180,0,0}, gFont13);

				// Render BG texture
				gRect.render(gRenderer, promptX-screenWidth/4, promptY-(screenWidth/3)/2, screenWidth/2, screenWidth/3);

				// Render text
				gText.render(gRenderer, promptX, promptY, gText.getWidth(), gText.getHeight());
			}
		}
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
			tl.renderTileDebug(gRenderer, tile, newMx, newMy, mex, mey, camx, camy, &rTiles[0]);

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

	// Tiles: layers
	if (tl.layer < -1) {
		tl.layer = 6;
	}
	if (tl.layer > 6) {
		tl.layer = -1;
	}

	// Tiles: id
	if (tl.id < 0) {
		tl.id = tb.TILES_UNIQUE;
	}
	if (tl.id > tb.TILES_UNIQUE) {
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
	// Player
	if (player.alive) {
		// Asteroid
		for (int i = 0; i < aste.max; i++) {
			if (asteroid[i].alive){
				// Asteroid collision target
				float bmx = player.x+player.w/2;
				float bmy = player.y+player.h/2;
				float bmx2 = asteroid[i].x+asteroid[i].w/2;
				float bmy2 = asteroid[i].y+asteroid[i].h/2;

				// Calculate distance
				float distance = sqrt((bmx - bmx2)* (bmx - bmx2) +
								      (bmy - bmy2)* (bmy - bmy2));

				// Collision occurred
				if (distance < player.w/2 + asteroid[i].w/2)
				{
					// Remove health pack
					asteroid[i].alive = false;
					aste.ASTEROIDS--;

					// Increase player health
					player.health += 10;


					// Player health max
					if (player.health > player.maxHealth) {
						player.health = player.maxHealth;
					}

					// play sound effect
					Mix_PlayChannel(-1, sAtariBoom, 0);
				}
			}
		}	// end Asteroid
	}	// end Player
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
						if (distance < player.radius + particles[i].w/2)
						{
							// Shake camera
							ShakeCamera();

							// Play impact sound effect
							Mix_PlayChannel(-1, sPirateHurt, 0);

							// Hurt player
							player.health -= particles[i].damage;

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


void PlayGame::checkCollisionTilePlayer() {
/*
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//-------------------------------- Collision with Tiles --------------------------------//
	// Update collision with Tiles
	//tl.checkCollision(tile, x, y, w, h, y, vY);

	// Update collision with Tiles
	// Player Velocity X Axis
	player.AccelerateXWithvX();

	// Create a new collision boundary around the player,
	// we will use this instead of the player's original boundary
	SDL_Rect rectA;
	rectA.x = player.x+4;
	rectA.y = player.y+8;
	rectA.w = player.w-8;
	rectA.h = player.h/2;

	// Move player back if coming into contact with a tile
	bool moveBack;
	moveBack = false;

	// Loop through all tiles if the player is touching them
	for (int i = 0; i < tl.max; i++) {
		if (tile[i].alive){
			if (tile[i].collide) {
				SDL_Rect rectB;
				rectB.x = tile[i].x;
				rectB.y = tile[i].y;
				rectB.w = tile[i].w;
				rectB.h = tile[i].h;
				if  ( checkCollisionRect( rectA, rectB )) {
					// If Player has more than 0 keys, then unlock door, otherwise continue collision check
					if (player.collectedKeys > 0 && player.useKey) {
						player.useKey = false;
						if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
							tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
							player.collectedKeys--;
							Mix_PlayChannel(-1, sRockBreak, 0);
							tile[i].alive = false;
							tl.tileCount--;
						}
					}
					// Continue handling collision
					moveBack = true;
				}
			}
		}
	}
	if (moveBack){
		player.DeccelerateXWithvX();
	}

	// Update collision with Tiles
	// Player Velocity Y Axis
	player.AccelerateYWithvY();

	// Create a new collision boundary around the player,
	// we will use this instead of the player's original boundary
	rectA.x = player.x+4;
	rectA.y = player.y+8;
	rectA.w = player.w-8;
	rectA.h = player.h/2;

	// Move player back if coming into contact with a tile
	moveBack = false;

	// Loop through all tiles if the player is touching them
	for (int i = 0; i < tl.max; i++) {
		if (tile[i].alive){
			if (tile[i].collide) {
				SDL_Rect rectB;
				rectB.x = tile[i].x;
				rectB.y = tile[i].y;
				rectB.w = tile[i].w;
				rectB.h = tile[i].h;
				if  ( checkCollisionRect( rectA, rectB )) {
					// If Player has more than 0 keys, then unlock door, otherwise continue collision check
					if (player.collectedKeys > 0 && player.useKey) {
						player.useKey = false;
						if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
							tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
							player.collectedKeys--;
							Mix_PlayChannel(-1, sRockBreak, 0);
							tile[i].alive = false;
							tl.tileCount--;
						}
					}
					// Continue handling collision
					moveBack = true;
				}
			}
		}
	}
	if (moveBack){
		player.DeccelerateYWithvY();
	}

	//-------------------------------- Collision with Tiles --------------------------------//
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	// Update player stuff that need to be updated after
	//player.UpdateAfter();*/
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
						// remove particle
						particles[i].alive = false;
						part.count--;

						// Spawn VFX
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
							//int r = rand() % 255;
							//int g = rand() % 255;
							//int b = rand() % 255;
							//SDL_Color color = {r, g, b};
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
					}
				}	// end check part type == 3
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

void PlayGame::checkPirateDied()
{
	for (int i = 0; i < pira.max; i++)
	{
		if (pirate[i].alive)
		{
			// Pirate dead
			if (pirate[i].health <= 0)
			{

				// If boss Pirate
				if (pirate[i].type == 0)
				{
					// Increase Player score
					player.score += 20;
				}

				// If boss Pirate
				if (pirate[i].type == 1)
				{
					// Spawn health pack
					float width = 32*2;
					float height = 27*2;
					float spawnX = pirate[i].x+pirate[i].w/2-width/2;
					float spawnY = pirate[i].y+pirate[i].h/2-height/2;
					aste.spawnAsteroidAngle(asteroid, spawnX, spawnY,
							width, height,
							0.0, 0.0);

					// Increase Player score
					player.score += 40;
				}

				// spawn blood particle effect
				/*for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
					int rands = rand() % 9 + 2;
					float newX = pirate[i].x+pirate[i].w/2;
					float newY = pirate[i].y+pirate[i].h/2;
					p_dummy.spawnParticleAngle(particle, 2,
										newX-rands/2,
										newY-rands/2,
									   rands, rands,
									   h, randDouble(2.1, 7.1),
									   0.0,
									   {255, 255, 0, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 7,
									   rand() % 50 + 90, 0,
									   true, 0.11,
									   true, randDouble(0.07, 0.69));
				}*/

				// Remove pirate
				pirate[i].alive = false;
				pira.count--;
			}
		}
	}
}

// Make sure all objects on the map do not overextend over the map
void PlayGame::ClampObjectsToLevelSize(Particle &part, Particle particles[]) {

	// Collision with door
	if (player.collectedKeys >= tl.requiredKeys && player.useKey) {
		if (checkCollision(player.x, player.y, player.w, player.h, doorX, doorY, 16, 16)) {
			// Increase Stage level
			playerStageLevel++;
			// Change to next Stage Level
			LoadLevel(part, particles);
		}
	}
	// Player
	if (player.x+player.w < 0) {
		player.x = 0;
	}
	if (player.x > 0+tl.levelWidth) {
		player.x =0+tl.levelWidth-player.w;
	}
	if (player.y+player.h < 0) {
		player.y = 0;
	}
	if (player.y > 0+tl.levelHeight) {
		player.y = 0+tl.levelHeight-player.h;
	}
	// Monster
	for (int i = 0; i < pira.max; i++) {
		if (pirate[i].alive) {
			if( pirate[i].x < 0 ){
				pirate[i].x = 0;
			}
			if( pirate[i].y < 0 ){
				pirate[i].y = 0;
			}
			if( pirate[i].x+pirate[i].w > tl.levelWidth ){
				pirate[i].x = tl.levelWidth-pirate[i].w;
			}
			if( pirate[i].y+pirate[i].h > tl.levelHeight ){
				pirate[i].y = tl.levelHeight-pirate[i].h ;
			}
		}
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
	case SDLK_COMMA:				// Change Tile collision properties
		if (place_type == 0 ) {
			tl.changeTileCollision(tile);
		}
		break;
	case SDLK_v:				// Change Tile destructible properties
		if (shift) {
			//tl.ChangeDestructable(tile, 0);
		}else{
			//tl.ChangeDestructable(tile, 1);
		}
		break;
	case SDLK_x:				// Save spawn point
		spawnX = mouseX+camx;
		spawnY = mouseY+camy;
		break;
	case SDLK_k:{								// Change collisionTile, if we should place a Tile with Collision

		tl.collisionTile = (!tl.collisionTile);
		break;
	}
	case SDLK_j:				// Spawn Tile as a destructible or not
		tl.destructible = (!tl.destructible);
		break;
	case SDLK_z:				// Save door spawn point
		doorX = mouseX+camx;
		doorY = mouseY+camy;
		break;
	case SDLK_w:
		if (shift) {
			tb.moveBarSelection(tilebar, "up");
		}else{
			camUp = true;
		}
		break;
	case SDLK_s:
		if (shift) {
			tb.moveBarSelection(tilebar, "down");
		}else{
			camDown = true;
		}
		break;
	case SDLK_a:
		if (shift) {
			tb.moveBarSelection(tilebar, "left");
		}else{
			camLeft = true;
		}
		break;
	case SDLK_d:
		if (shift) {
			tb.moveBarSelection(tilebar, "right");
		}else{
			camRight = true;
		}
		break;
	case SDLK_q:								// Change place type (i.e. Tiles or Collision Tiles)
		if (shift) {
			place_type--;
		}else{
			place_type++;
		}
		break;
	case SDLK_TAB:								// Toggle hide other layers
		tl.hideOtherLayers = (!tl.hideOtherLayers);
		break;
	/*case SDLK_k:								// Change clamp size
		if (shift){
			if (clampSize > 2) {
				clampSize -=2;
			}
		}else{
			clampSize+=2;
		}
		break;*/
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
		if (place_type == 0 ) {
			if (shift)
				tl.changeTileLayer(tile, -1);
			else
				tl.changeTileLayer(tile, 1);
		}else if (place_type == 1 ) {

		}
		break;
	case SDLK_c:								// Tile, copy Tile
		if (editor) {
			if (place_type == 0 ) {
				tl.copyTile(tile);
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
	case SDLK_8: {								// Load Room
			/*tl.loadTiles(tile);
			tc.loadTiles(tilec);
			//loadcTiles(ctile);

			std::stringstream tempss;
			tempss << "Loading Tiles";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());*/

			break;
		}
	case SDLK_9: {								// Save Room
			/*tl.saveTiles(tile);
			tc.saveTiles(tilec);
			//savecTiles(ctile);

			std::stringstream tempss;
			tempss << "Saving Tiles";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str());*/

			// Save current farthest level reached
			SaveFarthestLevelAchieved();
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
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {

				// Remove Tiles
				if (place_type == 0) {

					// Remove Tiles if left clicking, "0" in second argument
					// Also removes them by how many Tiles we are going to place horiz. and verti.
					tl.removeTile(tile, 0);
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
			}else{
				if (shift) {
					tb.selectBlockMultiple(tilebar, tl.id, mex, mey);
				// Pen Tool, select a Tile from the TileBar
				}else{
					tb.selectBlock(tilebar, tl.id);
				}
			}
		}
		if (rightClick) {
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {
				if (editor) {

					// Remove  tile
					if (place_type == 0) {
						tl.spawnTile(tile, newMx, newMy, camx, camy, &rTiles[0]);
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
				// Pen Tool, select a Tile from the TileBar
				tb.selectBlock(tilebar, tl.id);
			}
		}
	}
}


//-------------------------------------------- Editor Controls --------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------- Save/Load Functions ----------------------------------------------------//


// Save current level stage
void PlayGame::SaveFarthestLevelAchieved(){

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	//------------------------------- Save Level Data ---------------------------//

	std::stringstream PathDir;

	std::stringstream defaultDir;
	defaultDir << "resource/data/";

	//Save # of Blocks
	std::ofstream newPirateFile;
	PathDir.str(std::string());

	// Set directory to save
	PathDir << defaultDir.str().c_str();

	// Set file name and extension
	PathDir << "farthestAchievedLevel.mp";

	// Open File
	newPirateFile.open(PathDir.str().c_str());

	// Store data given from Editor to store in File
	newPirateFile << playerStageLevel;

	// Close
	newPirateFile.close();
	//------------------------------- Save Level Data ---------------------------//
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
}

// Load farthest level achieved
void PlayGame::LoadFarthestLevelAchieved(){

	// Create string-stream to store data from file
	std::stringstream filename;

	// Set file location and name of file
	filename << "resource/data/farthestAchievedLevel.mp";

	// Open file based on string above
	std::fstream farthestLevelFile( filename.str().c_str() );

	// If file is open
	if (farthestLevelFile.is_open()) {

		// Set game level to farthest saved level achieved
		farthestLevelFile >>  playerStageLevel;
	}

	// Error opening saved file, create a new one starting at level 1
	else{

		// Create string-stream that will hold our data to be saved
		std::stringstream filename;

		// Set file location and name of file
		filename << "resource/data/farthestAchievedLevel.mp";

		// Create new file based on string above
		std::ofstream newFarthestLevelFile( filename.str().c_str() );

		// If successfully created farthestAchievedLevel.mp file
		if (newFarthestLevelFile.is_open()) {
			newFarthestLevelFile << 1;
			playerStageLevel = 1;
		}
		newFarthestLevelFile.close();
	}
	farthestLevelFile.close();
}

void PlayGame::loadSpawnPoint(){
	// open config file
	std::stringstream filename;
	filename << "resource/data/maps/level" << playerStageLevel;
	filename << "/spawn.mp";
	std::fstream fileSpawn( filename.str().c_str() );
	if (fileSpawn.is_open()) {
		fileSpawn >>  spawnX >> spawnY >>  doorX >> doorY;
		player.x		= spawnX;
		player.y		= spawnY-6;
	}
	// Error opening spawn.mp file
	else{
		// Creating default spawn.mp
		std::stringstream filename;
		filename << "data/maps/level" << playerStageLevel;
		filename << "/spawn.mp";
		std::ofstream newSpawnFile( filename.str().c_str() );
		// If successfully created spawn.mp file
		if (newSpawnFile.is_open()) {
			newSpawnFile << "32 32 48 32";
			spawnX			= 32;
			spawnY			= 32;
			doorX			= 48;
			doorY			= 32;
			player.x		= spawnX;
			player.y		= spawnY-6;
		}
		newSpawnFile.close();
	}
	fileSpawn.close();
}

void PlayGame::ClearLevel(Particle &part, Particle particles[]) {
	tl.RemoveAll(tile);
	////////obj.RemoveAll(item);
	/////////part.RemoveAll(particles);
	pira.RemoveAll(pirate);
}

void PlayGame::LoadLevel(Particle &part, Particle particles[]) {

	// Remove everything
	{
		// Remove particles tiles
		part.RemoveAll(particles);

		// Remove textured tiles
		tl.RemoveAll(tile);
	}

	// Reset Player movements and direction
	player.collectedKeys = 0;
	player.health = 200;
	player.useKey = false;
	player.moveleft = false;
	player.moveright = false;
	player.moveup = false;
	player.movedown = false;

	// Load level farthest achieved
	LoadFarthestLevelAchieved();

	// Remove Particles
	part.RemoveAll(particles);

	// Load Monsters
	pira.LoadData(pirate, playerStageLevel);

	// Set file path and name
	std::stringstream tempss;
	tempss << "data/maps/";
	tempss << "Level";
	tempss << 1;
	tempss << ".mp";
	std::fstream fileTileDataL(tempss.str().c_str());

	// While file is open
	while( fileTileDataL.good() )
	{
		// Load Tile data
		tl.LoadData(tile, fileTileDataL);

		// Load Player spawn point
		fileTileDataL >>  this->spawnX >> this->spawnY;

		// Load map size
		fileTileDataL >>  map.w >> map.h;

		// Break out of file
		break;
	}
	fileTileDataL.close();




	// Load level spawn point
	//loadSpawnPoint();

	// Load Collision Tiles
	//tc.loadTiles(tilec, level);

	// Load Items (Items) Tiles
	//////////obj.LoadData(item, level);
}

void PlayGame::ResetLevel(Particle &part, Particle particles[]) {
	player.collectedKeys = 0;
	player.health = 200;
	player.useKey = false;
	player.moveleft = false;
	player.moveright = false;
	player.moveup = false;
	player.movedown = false;
	LoadLevel(part, particles);
}


//------------------------------------------------------ Save Functions ------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


