/*
 * TileBar.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#ifndef LOCAL_TILEBAR_H_
#define LOCAL_TILEBAR_H_

#include "../Engine/LTexture.h"
#include "../Engine/LWindow.h"

//Tile Class
class TileBar {
public:

	//// 37 w x 8 h =
	const int amountOfTilesHoriztontally = 8;
	const int amountOfTilesVertically = 37;
	const int tileW = 128;
	const int tileH = 128;

	//const int uniqueTiles = 80;
	const int uniqueTiles = 296;
	int tileCount;

public:

	SDL_Rect clip;			//Tile clips
	int x;					//Tile x position
	int y;					//Tile y position
	int w;					//Tile width
	int h;					//Tile height
	int angle;				//Tile angle
	int alpha;				//Tile alpha
	int id;					//Tile ID number
	std::stringstream name;	//Tile name
	bool MouseOnBlock;		//Tile has mouse above its
	bool screen;			//Tile in screen
	bool selected;			//Tile selected (multiples)
	bool alive;				//Tile is alive

	// Resources
	LTexture gTileBar;
	LTexture gRect;

	// Indirect
	//const int TILES_UNIQUE = 288;	// Total number of Unique Tiles

	// 57 tiles width
	// 31 tiles width
	//const int TILES_UNIQUE = 900;	// Total number of Unique Tiles

	int MARGINX = 0;
	int MARGINY = 0;
	const int TILES_UNIQUE = 296;	// Total number of Unique Tiles
	SDL_Rect rRect[6];				// Colors 64x64, [0: Green], [1: Orange], [2: White], [3: Blue], [4: Purple], [5: Dark Green fill, White border]
	bool touching;					// Player is hovering over Tile Bar

public:	// Core functions
	void Load(SDL_Renderer *gRenderer);

	void Init(TileBar tilebar[]);

	void Free();

public:	// Editor functions

	void Spawn(TileBar tilebar[], int x, int y, int w, int h,
								  int startx, int starty, int endW, int endH,
								  std::string collision);

	void SpawnMultiple(TileBar tilebar[]);

	// Select a tile from the tilebar
	void Select(TileBar tilebar[], int &tile_selection);

	// Move the tilebar
	void Move(TileBar tilebar[], std::string direction);

public:	// Game-play functions

	void Update(TileBar tilebar[], LWindow gWindow, int mx, int my, float camx, float camy);

	void Render(SDL_Renderer *gRenderer, TileBar tilebar[], int tile_selection);

public:	// Other functions

	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

};

#endif /* LOCAL_TILEBAR_H_ */
