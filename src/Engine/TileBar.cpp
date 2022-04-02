/*
 * TileBar.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#include "../Engine/TileBar.h"

#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

// TODO 2-2-2022 [ ] - fix the tilbar and tiles, but mostly tilbar, fix the sizes when spawned in world

void TileBar::Load(SDL_Renderer *gRenderer) {
	gTileBar.loadFromFile(gRenderer, "resource/gfx/cmesias/tile00.png");
	gRect.loadFromFile(gRenderer, "resource/gfx/cmesias/rect.png");
	rRect[0] = { 0, 0, 64, 64 };
	rRect[1] = { 64, 0, 64, 64 };
	rRect[2] = { 128, 0, 64, 64 };
	rRect[3] = { 192, 0, 64, 64 };
	rRect[4] = { 256, 0, 64, 64 };
	rRect[5] = { 320, 0, 64, 64 };
}

void TileBar::Init(TileBar tilebar[]) {
	tileCount = 0;
	touching = false;
	rRect[0] = { 0, 0, 64, 64 };
	rRect[1] = { 64, 0, 64, 64 };
	rRect[2] = { 128, 0, 64, 64 };
	rRect[3] = { 192, 0, 64, 64 };
	rRect[4] = { 256, 0, 64, 64 };
	rRect[5] = { 320, 0, 64, 64 };
	for (int i = 0; i < TILES_UNIQUE; i++) {
		tilebar[i].clip = {0,0,0,0};
		tilebar[i].x = 0;
		tilebar[i].y = 0;
		tilebar[i].w = 0;
		tilebar[i].h = 0;
		tilebar[i].angle = 0;
		tilebar[i].alpha = 0;
		tilebar[i].id = -1;
		tilebar[i].MouseOnBlock = false;
		tilebar[i].screen = false;
		tilebar[i].alive = false;
	}
}

void TileBar::Free(){
	gTileBar.free();
	gRect.free();
}

void TileBar::Spawn(TileBar tilebar[], int x, int y, int w, int h,
								int startx, int starty, int endW, int endH,
								std::string collision) {
	for (int i = 0; i < TILES_UNIQUE; i++) {
		if (!tilebar[i].alive){
			tilebar[i].x = x;
			tilebar[i].y = y;
			tilebar[i].w = w;
			tilebar[i].h = h;
			tilebar[i].clip.x = startx+MARGINX;
			tilebar[i].clip.y = starty+MARGINY;
			tilebar[i].clip.w = endW;
			tilebar[i].clip.h = endH;
			tilebar[i].id = 1+i;
			tilebar[i].alpha = 255;
			tilebar[i].name.str(std::string());
			tilebar[i].name << "tile-" << i;
			tilebar[i].MouseOnBlock = false;
			tilebar[i].screen = false;
			tilebar[i].selected = false;
			tilebar[i].alive = true;

			// Increase margin, going right
			//MARGINX++;
			break;
		}
	}
}

// Function to spawn the tilebar, but spawn them one by one
void TileBar::SpawnMultiple(TileBar tilebar[]) {

	// Loop the texture clips going vertically
	for (int h = 0; h < amountOfTilesVertically; h++) {

		// Then, loop the texture clips going horizontally
		for (int w = 0; w < amountOfTilesHoriztontally; w++) {

			int x = 0;
			int y = 0;
			int width = 32;
			int height = 32;
			int spacingWidth = 32;
			int spacingHeight = 32;
			int startClipX = 0;
			int startClipY = 0;

			// Spawn the tilebar, one by one
			Spawn(tilebar, x + w * spacingWidth, y + (h * spacingHeight),
						   width, height,
						   startClipX + w * tileW, startClipY + (h * tileH),
						   tileW, tileH, "off");
		}

		// Reset x margin
		MARGINX = 0;

		// Add y margin
		//MARGINY++;
	}
}

void TileBar::Select(TileBar tilebar[], int &tile_selection) {
	for (int i = 0; i < TILES_UNIQUE; i++) {
		if (tilebar[i].MouseOnBlock) {
			tile_selection = i;
		}
	}
}

//Move tile bar
void TileBar::Move(TileBar tilebar[], std::string direction){
	for (int i = 0; i < TILES_UNIQUE; i++) {
		if (direction == "left"){
			tilebar[i].x -= 50;
		}
		if (direction == "right"){
			tilebar[i].x += 50;
		}
		if (direction == "up"){
			tilebar[i].y -= 50;
		}
		if (direction == "down"){
			tilebar[i].y += 50;
		}
	}
}

void TileBar::Update(TileBar tilebar[], LWindow gWindow, int mx, int my, float camx, float camy){
	touching = false;
	for (int i = 0; i < TILES_UNIQUE; i++) {
		if (tilebar[i].alive) {
			if ( checkCollision(mx, my, 1, 1, tilebar[i].x,tilebar[i].y, tilebar[i].w, tilebar[i].h) ){
				tilebar[i].MouseOnBlock = true;
				touching = true;
			} else {
				tilebar[i].MouseOnBlock = false;
			}

			/*if (tilebar[i].x + tilebar[i].w+camx > camx && tilebar[i].x+camx < camx + gWindow.getWidth()
			 && tilebar[i].y + tilebar[i].h+camy > camy && tilebar[i].y+camy < camy + gWindow.getHeight()) {
				tilebar[i].screen = true;
			} else {
				tilebar[i].screen = false;
			}*/
		}
	}
}

void TileBar::Render(SDL_Renderer *gRenderer, TileBar tilebar[], int tile_selection){
	for (int i = 0; i < TILES_UNIQUE; i++) {
		if (tilebar[i].alive){
			//if (tilebar[i].screen){

				gRect.setAlpha(45);
				gRect.render(gRenderer, tilebar[i].x, tilebar[i].y, tilebar[i].w, tilebar[i].h, &rRect[5], 0);

				gTileBar.setAlpha(90);
				gTileBar.render(gRenderer, tilebar[i].x, tilebar[i].y, tilebar[i].w, tilebar[i].h, &tilebar[i].clip);
			//}

				// Selected Tile on tile-bar
				if ( i == tile_selection) {
					//gRect.setAlpha(255);
					//gRect.render(tilebar[i].x, tilebar[i].y,tilebar[i].w, tilebar[i].h, &rRect[1], 0);
					SDL_Rect tempr = {tilebar[i].x, tilebar[i].y,tilebar[i].w, tilebar[i].h};
					SDL_SetRenderDrawColor(gRenderer, 244, 144, 25, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
				// Hovered Tile on tile-bar
				if (tilebar[i].MouseOnBlock) {
					//gRect.setAlpha(255);
					//gRect.render(tilebar[i].x+4, tilebar[i].y+4,tilebar[i].w-8, tilebar[i].h-8, &rRect[0], 0);
					SDL_Rect tempr = {tilebar[i].x+1, tilebar[i].y+1,tilebar[i].w-2, tilebar[i].h-2};
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				//Render tile bar border
				}else{
				}
				// Tile selected
				if (tilebar[i].selected) {
					SDL_Rect tempr = {tilebar[i].x+2, tilebar[i].y+2,tilebar[i].w-4, tilebar[i].h-4};
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
		}
	}
}

bool TileBar::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2){
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

