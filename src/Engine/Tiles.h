/*
 * Tile.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#ifndef LOCAL_TILES_H_
#define LOCAL_TILES_H_

#include "LWindow.h"
#include "LTexture.h"

#include "Helper.h"
#include "TextNFont.h"
#include <vector>

class Tile: public Helper, public TextNFont {

public:	// Level Loading Variables
	int tileCount = 0;
	int levelWidth = 64;
	int levelHeight = 64;
	int requiredKeys = 99999;	// number of keys required to move on to the next level

public:
	/* Tilec Variables */
	const int max = 5000;
	int multiW = 1;
	int multiH = 1;
	int tilew = 32;
	int tileh = 32;
	bool hideOtherLayers = false;
	LTexture gTiles;
	LTexture *gJar;
	bool startJarBreaking;
	int jarIndex;

	// Check collision between Player & Tile
	/* Tile references
	 *
	 * 187, 194
	 * 201, 194
	 * 215, 222
	 * 229, 235
	 * 243, 250
	 */
	std::vector<int> *flowerTiles;

	/* Second frame before looping
	 *
	 * 194
	 * 194
	 * 222
	 * 235
	 * 250
	 */
	std::vector<int> *flowerTiles2;

public:	// animation for light Tile

	// 296 - 303:	top of light tile
	// 328 - 335:	bottom of light tile
	float animTimer = 0;
	const float animTimerSpe = 3;

public:
	float x;
	float y;
	int w;
	int h;

	// Get rect of each specific Tile
	SDL_Rect rectB;

	/*
	 * 0: Used to collide with the player to move into a new area/level
	 * 1: Jar
	 */
	int id;
	int alpha;
	SDL_Rect clip;

	/* Layers
	 *
	 * 0: Floor
	 * 1: Appliance			- Only rendered on top of Player when player.y+player.h < tile.x+tile.w
	 * 2: Top of Appliance	- Always rendered on top of Player
	 * 3: Roof 				- Always rendered on top of Player
	 */
	int layer;
	bool mouse;				// Mouse point
	bool mouseBox;			// Mouse box
	bool screen;
	bool player;
	bool promptSelf;
	float hoverAmount;
	float hoverDir;
	bool alive;
	std::string side;		// collision with player
	bool collisionTile;		// If true, this Tile is considered a collision tile

	// Health of Tile, and if destructible
	float health;
	bool destructible;

public:	// Initial
	void initTile(Tile tile[]);

	void load(SDL_Renderer *gRenderer, LTexture *gJar, std::vector<int> *flowerTiles, std::vector<int> *flowerTiles2);

	void free();

public:	// editor functions
	void placeTile(Tile tile[], float x, float y,
					int id, int layer,
					bool collisionTile,
					SDL_Rect clip);

	void copyTile(Tile tile[]);

	void changeTileCollision(Tile tile[]);

	void changeTileLayer(Tile tile[], int value);

	void removeTile(Tile tile[], int click);

	void RemoveAll(Tile tile[]);

	/* Place Tile */
	void spawnTile(Tile tile[], float newMx, float newMy, float camx, float camy, SDL_Rect rTiles[]);

public:	// core game functions
	bool updateTile(Tile tile[], LWindow &gWindow,
			  float targetX, float targetY, float targetW, float targetH,
			  int newMx, int newMy,
			  int mex, int mey, int camx, int camy,
			  SDL_Rect rTiles[]);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Tile collision check, player x position
	void checkCollisionXY(Tile tile[], float &x, float &y, int w, int h, float &vX, float &vY, bool dashing);

	// Tile collision check, player x position
	void checkCollisionX(Tile tile[], float &x, float y, int w, int h, std::string &jumpstate, float &vX, int &jumps);

	// Tile collision check, player y position
	void checkCollisionY(Tile tile[], float x, float &y, int w, int h, std::string &jumpstate, float &vY, int &jumps, bool &jump);

	void renderTile(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy, SDL_Rect rJar[]);

	bool PlayerBehindTile;

	void RenderBehindPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[], SDL_Rect rJar[]);

	void RenderOnTopOfPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[], SDL_Rect rJar[]);

	void RenderUI(SDL_Renderer *gRenderer, Tile tile[], int camx, int camy);

	// Render Tile Debug info
	void renderTileDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

	// Render Tile Debug in Hand
	void RenderHand(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

	void setStatsBasedOnType(Tile tile[], int i);

public:	// Get functions
	// Get size of Tile in pixels
	int getPixelW(SDL_Rect rTiles[]);

	// Get size of Tile in pixels
	int getPixelH(SDL_Rect rTiles[]);

public:	// Tile manipulation

	void MoveTiles(Tile tile[], std::string direction);

public: // tile saving functions

	// Load Tile data
	void LoadData(Tile tile[], std::fstream &fileToReturn);

	// Save Tile data
	std::string SaveData(Tile tile[]);
};

#endif /* LOCAL_TILES_H_ */
