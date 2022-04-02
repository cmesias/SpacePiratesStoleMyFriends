/*
 * PlayGame.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef PLAYGAME_H_
#define PLAYGAME_H_

#include "Engine/Asteroids.h"
#include "Engine/Enemies.h"
#include "Engine/Maps.h"
#include "Engine/Pirate.h"
#include "Engine/Players.h"
#include "Engine/Spawners.h"
#include "Engine/TileBar.h"
#include "Engine/Tiles.h"
#include "Engine/Options.h"
#include "Engine/Helper.h"

// New from "DungeonCaster"

class PlayGame : public Helper, public Options {
public:	// specific game rules

public:	// globals

public: // System variables
	enum Result { Back, Nothing, StartGame, ShowingMenu, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, PlayGame::Result &result);
	PlayGame::Result mousePressed(SDL_Event event);
	PlayGame::Result mouseReleased(SDL_Event event);

public:	// Scene textures
	LTexture gBG;
	LTexture gTargetTexture;
	LTexture gCircle;

	class Text {
	public:
		int count;
		float x;
		float y;
		int w;
		int h;
		int alpha;
		int timer;
		bool alive;
		float vX;
		float vY;
		std::string textfield;
		SDL_Color color;
		void init(Text text[]) {
			count =0;
			for (int i=0; i<100; i++) {
				text[i].alive = false;
				text[i].color = {255,255,255};
			}
		}
		void spawn(Text text[], float x, float y, float vX, float vY, int alpha, std::string textfield, SDL_Color color = {255,255,255}) {
			for (int i=0; i<100; i++) {
				if (!text[i].alive) {
					text[i].x = x;
					text[i].y = y;
					text[i].w = 139;
					text[i].h = 96;
					text[i].vX = vX;
					text[i].vY = vY;
					text[i].timer = timer;
					text[i].color = color;
					text[i].alpha = alpha;
					text[i].textfield = textfield;
					text[i].alive = true;
					count++;
					break;
				}
			}
		}
		void update(Text text[]) {
			for (int i=0; i<100; i++) {
				if (text[i].alive) {
					text[i].x += text[i].vX;
					text[i].y += text[i].vY;

					text[i].alpha -= 16;
					if (text[i].alpha <= 0) {
						text[i].alive = false;
						count--;
					}
				}
			}
		}
	};

	Text tex;
	Text text[100];

public:	// Other classes
	Pirate pira;
	Map map;
	Particle part;
	Asteroid aste;
	Spawner spaw;
	Enemy enem;
	//Pause pau;
	//Pause::Result pauseResult;
	Players player;

	// Class instance, multiple
	Pirate pirate[100];
	Particle particles[512];
	Asteroid asteroid[128];
	Enemy enemy[8];
	Spawner spawner[200];
	//Mixed mix;
	//Controls cont;
	//static Items items[10];

public:	// Tile and Tilebar variables

	// Tiles
	Tile tl;
	Tile tile[3000];

	// Collision Tiles
	//TileC tc;
	//TileC tilec[2096];
	SDL_Rect rClips[64]; 		// doorWidth * doorHeight = 64

	// Tilebar
	TileBar tb;
	TileBar tilebar[296];
	SDL_Rect rTiles[296];		// how many unique tiles are, on the tile-bar

	/*
	 * 0: Closed
	 * 1: Open
	 * 2: Spawn tile
	 */
	SDL_Rect rDoor[3];

	// Player door and spawn point textures
	LTexture gDoor;				//

public:			// Camera variables

    float camx;
    float camy;
    bool camlock;

    // Camera Shake

	/* This will bring the player
	 * to life. At least, that's what
	 * it'll look like. This will rustle
	 * the player's armor as they move
	 */
	float rustleW;
	float rustleSpe;
	float rustleDirX;
    bool camshake;

	void ShakeCamera();

	bool playerShakeCamera;

public:	// Variables

	// Initialize
	void Init();

	// Load resources
	void Load(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Free resources
	void Free();

	// Update everything
	void Update(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Render foreground
	void RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render objects
	void Render(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render UI
	void RenderUI(SDL_Renderer *gRenderer);

	// Render debug information
	void RenderDebug(SDL_Renderer *gRenderer);

	// Render text
	void RenderText(SDL_Renderer *gRenderer, LWindow &gWindow);

public: // Functions for Tiles

	void TilesUpdate();

public:	// Functions mixed with other classes

	// Check collision between Particle & Asteroid
	void checkCollisionPlayerAsteroid(Players &player, Asteroid asteroid[]);

	// Check collision between Particle & Asteroid
	void checkCollisionParticleAsteroid(Particle particle[], Particle &part, Asteroid asteroid[], Players &player);

	// Check collision between Particle & Enemy
	void checkCollisionParticleEnemy(Particle particle[], Particle &part, Enemy enemy[], Players &player);

	// Check collision between Particle & Pirate
	void checkCollisionParticlePirate();

	// Check collision between Particle & Player
	void checkCollisionParticlePlayer(Particle particle[], Particle &part, Players &player);

	// Check collision between certain Tiles & Player
	void checkCollisionTilePlayer();

	// Check collision between Grenade Particle & Enemies
	void checkCollisionGrenadePlayer();

	// Spawn Asteroids upon destroying all Asteroids
	void spawnAsteroidsNow2();

public:	// Boundary functions

	// Make sure all objects on the map do not overextend over the map
	void ClampObjectsToLevelSize(Particle &part, Particle particles[]);

public:	// Functions that serve multiple purposes

	// Knock-back effect: take the position of the explosion, and the object your are knocking back
	void knockbackEffect(float targetX, float targetY, int targetW, int targetH,
						 float objectX, float objectY, int objectW, int objectH,
						 float &objectVX,float &objectVY, float force);

public: // Saved data - variables

	int currentLevelStage;			// Current stage loaded
	int playerStageLevel;			// Highest stage ever achieved
	float spawnX = 0, spawnY = 0;	// This is where the player will spawn on the map
	float doorX = 100, doorY = 100;	// The player needs to reach this door or tile to advance to the next level

public:

	// Save current level stage
	void SaveFarthestLevelAchieved();

	// Load farthest level achieved
	void LoadFarthestLevelAchieved();

	// Load spawn point
	void loadSpawnPoint();

public: // Render Editor UI

	// Render tile in hand
	void RenderHand(SDL_Renderer *gRenderer);

private:	// used for some debugging
	bool debug;
	bool editor;
	int mx, my;
	bool quit;
	bool ctrl;
	bool shift;
	SDL_Event event;

	// Buttons for editor
	bool rightClick = false;
	bool leftClick;

public: // Editor variables

	/* Placement types
	 * 0: Tiles
	 * 2: Collision Tiles (depreciated)
	 * 3: pirates
	 *
	 */
	int place_type;
	int clampSize;					// Magnet pixel size
	float mouseX;
	float mouseY;

	// Editors camera
	bool camUp = false;
	bool camDown = false;
	bool camLeft = false;
	bool camRight = false;

	// Editor controls
	void editorOnKeyDown(SDL_Keycode sym, Particle &part, Particle particles[]);

	void editorOnKeyUp(SDL_Keycode sym);

	void UpdateEditorTilePlacement();

	// Clear current working level
	void ClearLevel(Particle &part, Particle particles[]);

	// Load a level
	void LoadLevel(Particle &part, Particle particles[]);

	// Reset Level
	void ResetLevel(Particle &part, Particle particles[]);

};

#endif /* PLAYGAME_H_ */
