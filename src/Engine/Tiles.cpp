/*
 * Tile.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#include "../Engine/Tiles.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>


void Tile::Load(SDL_Renderer *gRenderer) {
	gTiles.loadFromFile(gRenderer, "resource/gfx/cmesias/tile00.png");
	gTileBreak.loadFromFile(gRenderer, "resource/gfx/tile-break.png");
	int j = 0;
	for (int w = 0; w < 10; w++) {
		rTileBreak[j].x = 0 + w * 16;
		rTileBreak[j].y = 0;
		rTileBreak[j].w = 16;
		rTileBreak[j].h = 16;
		j++;
	}
	gFont12 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 12);
}

void Tile::Free() {
	gText.free();
	gTiles.free();
	gTileBreak.free();
	TTF_CloseFont(gFont12);
	gFont12 = NULL;
}


void Tile::Init(Tile tile[]) {
	levelWidth = 64;
	levelHeight = 64;
	layer 		= 0;
	id 			= 0;
	tileCount 	= 0;
	multiW 		= 1;
	multiH 		= 1;
	hideOtherLayers 	= false;
	for (int i = 0; i < max; i++) {
		tile[i].hits = -1;
		tile[i].id = 1;
		tile[i].alpha = 255;
		tile[i].layer = -1;
		tile[i].animTimer = 0;
		tile[i].animFrame = 0;
		tile[i].mouse = false;
		tile[i].mouseBox = false;
		tile[i].screen = false;
		tile[i].player = false;
		tile[i].side = "right";
		tile[i].collide = false;
		tile[i].destructible = false;
		tile[i].alive = false;
	}
}

void Tile::Spawn(Tile tile[], float x, float y, int w, int h, int id, int layer, SDL_Rect clip) {
	for (int i = 0; i < max; i++) {
		if (!tile[i].alive){
			tile[i].hits 	= -1;
			tile[i].x 		= x;
			tile[i].y 		= y;
			tile[i].w 		= w;
			tile[i].h 		= h;
			tile[i].id 		= id;
			tile[i].alpha 	= 255;
			tile[i].clip 	= clip;
			tile[i].layer 	= layer;
			tile[i].animTimer = 0;
			tile[i].animFrame = 0;
			tile[i].mouse 	= false;
			tile[i].mouseBox = false;
			tile[i].screen 	= false;
			tile[i].player = false;
			tile[i].side = "right";
			tile[i].collide = collide;
			tile[i].destructible = destructible;
			tile[i].alive 	= true;
			tileCount++;
			break;
		}
	}
}
void Tile::Copy(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					id = tile[i].id;
				}
			}
		}
	}
}

void Tile::ChangeCollision(Tile tile[], int click) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			// If Tile placement size is above Tile change collision
			if (click == 0) {
				if (tile[i].mouseBox){
					if (tile[i].layer == layer) {
						tile[i].collide = (!tile[i].collide);
					}
				}
			// Mouse change collision
			}else{
				if (tile[i].mouse){
					if (tile[i].layer == layer) {
						tile[i].collide = (!tile[i].collide);
					}
				}
			}
		}
	}
}

void Tile::ChangeDestructable(Tile tile[], int click) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			// If Tile placement size is above Tile change collision
			if (click == 0) {
				if (tile[i].mouseBox){
					if (tile[i].layer == layer) {
						tile[i].destructible = (!tile[i].destructible);
					}
				}
			// Mouse change collision
			}else{
				if (tile[i].mouse){
					if (tile[i].layer == layer) {
						tile[i].destructible = (!tile[i].destructible);
					}
				}
			}
		}
	}
}

void Tile::ChangeLayer(Tile tile[], int value) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					tile[i].layer += value;
				}
			}
		}
	}
}

void Tile::Remove(Tile tile[], int click) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			// Left click
			if (click == 0) {
				if (tile[i].mouseBox){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			// Right click
			}else{
				if (tile[i].mouse){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			}
		}
	}
}

void Tile::RemoveAll(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			tile[i].alive = false;
		}
	}
	tileCount = 0;
}

void Tile::SpawnMultiple(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]) {
	Remove(tile, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j * tilew + camx);
			int y = int(newMy + h * tileh + camy);
			Spawn(tile, x, y, tilew, tileh, id, layer, rTiles[id]);
		}
	}
}

void Tile::Update(Tile tile[], LWindow &gWindow, int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]) {
	//std::cout << "newMx: " << newMx << std::endl;
	//std::cout << "newMy: " << newMy << std::endl;
	int tileW = tilew*multiW;
	int tileH = tileh*multiH;
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			//If the tile is in the screen
			if (tile[i].x + tile[i].w > camx && tile[i].x < camx + screenWidth
			 && tile[i].y + tile[i].h > camy && tile[i].y < camy + screenHeight) {
				tile[i].screen = true;
			} else {
				tile[i].screen = false;
			}
			//If the mouse+size is on the tile
			if (newMx+tileW-2 > tile[i].x && newMx+1 < tile[i].x + tile[i].w &&
					newMy+tileH-2 > tile[i].y && newMy+1 < tile[i].y + tile[i].h) {
				tile[i].mouseBox = true;
			} else {
				tile[i].mouseBox = false;
			}
			//If the mouse is on the tile
			if (mex > tile[i].x && mex < tile[i].x + tile[i].w &&
				mey > tile[i].y && mey < tile[i].y + tile[i].h) {
				tile[i].mouse = true;
			} else {
				tile[i].mouse = false;
			}
			// Reset layer if goes higher
			if (tile[i].layer > 6) {
				tile[i].layer = 0;
			}
			// Death by breaking
			if (tile[i].destructible) {
				if (tile[i].hits > 9) {
					tile[i].alive = false;
					tileCount--;
				}
			}

			// Animations
			/*if (tile[i].id == 143) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 30) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 142;
					tile[i].clip = rTiles[142];
				}
			}
			if (tile[i].id == 142) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 30) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 143;
					tile[i].clip = rTiles[143];
				}
			}
			if (tile[i].id == 166) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 9) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 167;
					tile[i].clip = rTiles[167];
				}
			}
			if (tile[i].id == 167) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 9) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 166;
					tile[i].clip = rTiles[166];
				}
			}*/
		}
	}
}

bool Tile::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void Tile::checkCollision(Tile tile[], float x, float y, int w, int h, float &coordinateXY, float &velocity) {
	SDL_Rect rectA;
	rectA.x = x;
	rectA.y = y;
	rectA.w = w;
	rectA.h = h;

	bool moveBack;
	moveBack = false;

	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].collide) {
				SDL_Rect rectB;
				rectB.x = tile[i].x;
				rectB.y = tile[i].y;
				rectB.w = tile[i].w;
				rectB.h = tile[i].h;
				if  ( checkCollisionRect( rectA, rectB )) {
					moveBack = true;
				}
			}
		}
	}
	if (moveBack){
		coordinateXY -= velocity;
	}

	/*for (int i = 0; i < max; i++) {
		if (tile[i].alive) {
			if (tile[i].id == 9) {
				if (tile[i].collide) {
					if (y + h <= tile[i].y) {
						if (x + w < tile[i].x || x > tile[i].x + tile[i].w) {
							jumpstate = "falling";
						}
					}
				}

				//Collision Check - Spiked on top - touching body
				if (x + w > tile[i].x && x < tile[i].x + tile[i].w &&
					y + h > tile[i].y && y < tile[i].y + tile[i].h) {
					tile[i].collide = true;
				} else {
					tile[i].collide = false;
				}

				if (tile[i].collide) {
					if (tile[i].side == "left") {
						x = tile[i].x - w;
						if (vX > 0) {
							vX = 0;
						}
					}
					if (tile[i].side == "right") {
						x = tile[i].x + tile[i].w;
						if (vX < 0) {
							vX = 0;
						}
					}
					if (tile[i].side == "top") {
						vY = 0.0;
						y = tile[i].y - h;
				        jumpstate = "ground";
						jumps = 1;
					}
					if (tile[i].side == "down") {
						y = tile[i].y + tile[i].h;
						vY = 1;
						jumpstate = "falling";
					}
				}
				if (x + w <= tile[i].x) {
					tile[i].side = "left";
				}
				if (y + h <= tile[i].y) {
					tile[i].side = "top";
				}
				if (x >= tile[i].x + tile[i].w) {
					tile[i].side = "right";
				}
				if (y >= tile[i].y + tile[i].h) {
					tile[i].side = "down";
				}
			} // end checking tile-9
		}
	}*/
}

void Tile::Render(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive && tile[i].screen){
			// Tile trasparency on Player collision
			if (tile[i].layer != 0 && tile[i].player) {
				//tile[i].alpha = 90;
			}else{
				//tile[i].alpha = 255;
			}
			// Render layer that Editor has selected
			if (hideOtherLayers) {
				if (layer == tile[i].layer) {
				}
			// Render all tiles
			}else{
				if (layer_dummy == tile[i].layer) {
					// Center Door Tile
					if (tile[i].id > 63 && tile[i].id < 80) {
						gTiles.setAlpha(tile[i].alpha);
						gTiles.render(gRenderer, tile[i].x-7 - camx, tile[i].y-8 - camy, 32, 32, &tile[i].clip);
					}
					// Render every other tile normally
					else{
						gTiles.setAlpha(tile[i].alpha);
						gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
						// If destructible, render destructible Texture on Tiles
						if (tile[i].destructible && tile[i].hits >= 0) {
							gTileBreak.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rTileBreak[tile[i].hits]);
						}
					}
					/*SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h};
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);*/
					//if (player.y+player.h >= tile[i].y+tile[i].h) {
						//gTiles.setAlpha(tile[i].alpha);
						//gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
						//player.render(gRenderer, camx, camy);
					//}else{
						//player.render(gRenderer, camx, camy);
						//gTiles.setAlpha(tile[i].alpha);
						//gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
					//}
				}
			}
		}
	}
}

void Tile::RenderDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[], int tileSize){
	// Render Tile info
	for (int i = 0; i < max; i++) {
		if (tile[i].alive && tile[i].screen){
			if (hideOtherLayers) {
				if (layer == tile[i].layer) {
					// Identify a Collision Tile
					if (tile[i].collide) {
						SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 2, 2};
						SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
						SDL_RenderFillRect(gRenderer, &tempr);
					}else{
						SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 2, 2};
						SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
						SDL_RenderFillRect(gRenderer, &tempr);
					}
					// If Tile is destructible
					if (tile[i].destructible) {
						SDL_Rect tempr = {tile[i].x - camx, tile[i].y+2 - camy, 2, 2};
						SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}else{
						SDL_Rect tempr = {tile[i].x - camx, tile[i].y+2 - camy, 2, 2};
						SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}
					// Identify if mouse is on top of a Tile
					if (tile[i].mouse && tile[i].layer == layer) {
						SDL_Rect tempr = {tile[i].x+2 - camx, tile[i].y - camy, 2, 2};
						SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
						std::stringstream tempss;
						tempss << tile[i].id;
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont12);
						gText.setAlpha(255);
						gText.render(gRenderer, tile[i].x - camx, tile[i].y - camy, gText.getWidth(), gText.getHeight());
					}
					// Identify if the size of the Tiles we are placing are over mouse
					if (tile[i].mouseBox) {
						SDL_Rect tempr = {tile[i].x+4 - camx, tile[i].y - camy, 2, 2};
						SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}

					// Render Tile Size
					/*SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h};
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);*/

					/*if (tile[i].mouse) {
						stringstream tempss;
						tempss << "x: " << tile[i].x;
						gText.loadText(tempss.str().c_str(), white, gFont12);
						gText.setAlpha(255);
						gText.render(tile[i].x - cam.x, tile[i].y - cam.y, gText.getWidth(), gText.getHeight());
						stringstream tempss1;
						tempss1 << "y: " << tile[i].y;
						gText.loadText(tempss1.str().c_str(), white, gFont12);
						gText.setAlpha(255);
						gText.render(tile[i].x - cam.x, tile[i].y+12 - cam.y, gText.getWidth(), gText.getHeight());
					}*/
				}
			}else{
				// Identify a Collision Tile
				if (tile[i].collide) {
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 2, 2};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				}else{
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 2, 2};
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				}
				// If Tile is destructible
				if (tile[i].destructible) {
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y+2 - camy, 2, 2};
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}else{
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y+2 - camy, 2, 2};
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
				// Identify if mouse is on top of a Tile
				if (tile[i].mouse && tile[i].layer == layer) {
					SDL_Rect tempr = {tile[i].x+2 - camx, tile[i].y - camy, 2, 2};
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
					std::stringstream tempss;
					tempss << tile[i].id;
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont12);
					gText.setAlpha(255);
					gText.render(gRenderer, tile[i].x - camx, tile[i].y - camy, gText.getWidth(), gText.getHeight());
				}
				// Identify if the size of the Tiles we are placing are over mouse
				if (tile[i].mouseBox) {
					SDL_Rect tempr = {tile[i].x+4 - camx, tile[i].y - camy, 2, 2};
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}

				// Render Tile Size
				/*SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);*/

				/*if (tile[i].mouse) {
					stringstream tempss;
					tempss << "x: " << tile[i].x;
					gText.loadText(tempss.str().c_str(), white, gFont12);
					gText.setAlpha(255);
					gText.render(tile[i].x - cam.x, tile[i].y - cam.y, gText.getWidth(), gText.getHeight());
					stringstream tempss1;
					tempss1 << "y: " << tile[i].y;
					gText.loadText(tempss1.str().c_str(), white, gFont12);
					gText.setAlpha(255);
					gText.render(tile[i].x - cam.x, tile[i].y+12 - cam.y, gText.getWidth(), gText.getHeight());
				}*/
			}
		}
	}
	// Render tile in hand
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			// these are door Tiles, center these
			if (id > 63 && id < 80) {
				gTiles.setAlpha(110);
				gTiles.render(gRenderer, newMx+j*tilew-7, newMy+h*tileh-8, 32, 32, &rTiles[id]);
			}
			// Render normally
			else{
				gTiles.setAlpha(110);
				gTiles.render(gRenderer, newMx+j*tilew, newMy+h*tileh, tilew, tileh, &rTiles[id]);
			}

			// Render mouse coordinates snapped to grid
			/*SDL_Rect tempr = {newMx, newMy, rTiles[id].w*multiW, rTiles[id].h*multiH};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);*/

			// Render mouse + size * multiplied
			// Render mouse coordinates unsnapped
			/*tempr = {mex, mey, tilew*multiW, tileh*multiH};
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);*/
		}
	}
}

void Tile::LoadData(Tile tile[], int level){
	//Load Tile
	tileCount = 0;
	levelWidth = 64;
	levelHeight = 64;
	Init(tile);

	// Open Level File
	std::stringstream fileName;
	fileName << "resource/data/maps/";
	fileName << "level" << level;
	fileName << "/Tile.mp";
	std::fstream fileTileDataL(fileName.str().c_str());
	// Read first line for number of Keys required to finish level and Level Size and tile count
	fileTileDataL >> requiredKeys >> levelWidth >> levelHeight >> tileCount;
	// Read the rest of the lines for Tile data
	while( fileTileDataL.good() ){
		for (int i = 0; i < max; i++) {
			if (!tile[i].alive) {
				fileTileDataL >>  tile[i].x 		>>
								  tile[i].y 		>>
								  tile[i].w 		>>
								  tile[i].h 		>>
								  tile[i].id 		>>
								  tile[i].layer 	>>
								  tile[i].alpha 	>>
								  tile[i].clip.x 	>>
								  tile[i].clip.y 	>>
								  tile[i].clip.w 	>>
								  tile[i].clip.h 	>>
								  tile[i].mouse 	>>
								  tile[i].screen 	>>
								  tile[i].collide 	>>
								  tile[i].destructible 	>>
								  tile[i].alive;
				break;
			}
		}
	}
	fileTileDataL.close();
}

std::string Tile::SaveData(Tile tile[]){
	// Create new file to store Tile data
	std::ofstream tileDataFile;
	// Create stringstream to store Tile Data
	std::stringstream tempData;
	// Write Level Size on first line
	tempData << requiredKeys << " " << levelWidth << " " << levelHeight << " " << tileCount << "\n";
	// Write the rest of the Tile Data after the first line
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {
			tempData << tile[i].x 			<< " "
					 << tile[i].y 			<< " "
					 << tile[i].w  			<< " "
					 << tile[i].h  			<< " "
					 << tile[i].id  		<< " "
					 << tile[i].layer 		<< " "
					 << tile[i].alpha  		<< " "
					 << tile[i].clip.x 		<< " "
					 << tile[i].clip.y 		<< " "
					 << tile[i].clip.w 		<< " "
					 << tile[i].clip.h 		<< " "
					 << tile[i].mouse 		<< " "
					 << tile[i].screen 		<< " "
					 << tile[i].collide 	<< " "
					 << tile[i].destructible 	<< " "
			   	   	 << tile[i].alive 		<< "\n";
		}
	}
	return tempData.str().c_str();
}
