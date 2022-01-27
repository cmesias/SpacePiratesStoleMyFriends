/*
 * Tile.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#ifndef LOCAL_TILES_H_
#define LOCAL_TILES_H_

#include "LTexture.h"
#include "LWindow.h"
#include "Helper.h"

class Tile: public Helper {
public:	// editor variables
	/* Tile Variables */
	const int max = 3000;
	int multiW = 1;
	int multiH = 1;
	int tilew = 16;
	int tileh = 16;
	bool hideOtherLayers = false;

	LTexture gText;
	LTexture gTiles;
	LTexture gTileBreak;
	SDL_Rect rTileBreak[10];
	TTF_Font *gFont12 = NULL;

public:	// Level Loading Variables
	int tileCount = 0;
	int levelWidth = 64;
	int levelHeight = 64;
	int requiredKeys = 99999;	// number of keys required to move on to the next level


public:	// instance variables
	float x;
	float y;
	int w;
	int h;
	int id;
	int alpha;
	SDL_Rect clip;
	int layer;
	int animTimer;
	int animFrame;
	int hits;			// hits before it breaks
	bool mouse;			// Mouse point
	bool mouseBox;		// Mouse box
	bool screen;
	bool player;
	bool alive;
	std::string side;	// collision with player
	bool destructible;	// able to be destroyed
	bool collide;		// collision with player - this just checks if this tile is a collision tile, if it is a collision tile then it will interact with the player.

public:	// Test variables

	int distanceP;		// Distance from Player
public:	// Initial
	void Init(Tile tile[]);

	void Load(SDL_Renderer *gRenderer);

	void Free();

public:	// Editor functions
	void Spawn(Tile tile[], float x, float y, int w, int h, int id, int layer, SDL_Rect clip);

	void Copy(Tile tile[]);

	void ChangeCollision(Tile tile[], int click);

	void ChangeDestructable(Tile tile[], int click);

	void ChangeLayer(Tile tile[], int click);

	// This will remove underlying Tile(s) before Tile placement
	void Remove(Tile tile[], int click);

	void RemoveAll(Tile tile[]);

	void SpawnMultiple(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]);

public:	// core game functions

	void Update(Tile tile[], LWindow &gWindow, int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	void checkCollision(Tile tile[], float x, float y, int w, int h, float &coordinateXY, float &velocity);

	void Render(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy);

	void RenderDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey,
						 int camx, int camy, SDL_Rect rTiles[], int tileSize);

public: // tile saving functions

	// Load data
	void LoadData(Tile tile[], int level);

	// Save data
	std::string SaveData(Tile tile[]);
};

#endif /* LOCAL_TILES_H_ */
