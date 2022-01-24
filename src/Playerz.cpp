/*
 * Playerz.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: Carl
 */

#include <iostream>

#include <SDL2/SDL_image.h>
#include "Playerz.h"

Playerz::Playerz() {
	x = 0;
	y = 0;
	w = 32;
	h = 32;
	velX = 0.0;
	velY = 0.0;
	drag = 0.15;
	moveL = false;
	moveR = false;
	moveU = false;
	moveD = false;
	alive = false;
	name  = "hi";
	playerID = -1;
	count = 0;
}

// Initialize Playerz(s)
void Playerz::init(Playerz player[]) {
	playerID = -1;
	count = 0;
	for (int i=0; i<maxCount; i++) {
		player[i].x = 0;
		player[i].y = 0;
		player[i].w = 32;
		player[i].h = 32;
		player[i].velX = 0.0;
		player[i].velY = 0.0;
		player[i].drag = 0.15;
		player[i].name = "JaneDoe";
		player[i].moveL = false;
		player[i].moveR = false;
		player[i].moveU = false;
		player[i].moveD = false;
		player[i].alive = false;
	}
}

// Create Playerz(s)
void Playerz::create(Playerz player[], float x, float y, int w, int h, std::string name) {
	for (int i=0; i<maxCount; i++) {
		if (!player[i].alive) {
			player[i].x = x;
			player[i].y = y;
			player[i].w = w;
			player[i].h = h;
			player[i].name = name;
			player[i].alive = true;
			count++;
			break;
		}
	}
}

// Update player
void Playerz::update(Playerz player[]) {
	for (int i=0; i<maxCount; i++) {
		if (player[i].alive) {
			// Velocity move
			if (player[i].moveL) {
				player[i].velX -= 1;
			}
			if (player[i].moveR) {
				player[i].velX += 1;
			}
			if (player[i].moveU) {
				player[i].velY -= 1;
			}
			if (player[i].moveD) {
				player[i].velY += 1;
			}

			// Gravity
			player[i].x += player[i].velX;
			player[i].y += player[i].velY;

			// Velocity max
			if (player[i].velX < -6) {
				player[i].velX = -6;
			}
			if (player[i].velX > 6) {
				player[i].velX = 6;
			}
			if (player[i].velY < -6) {
				player[i].velY = -6;
			}
			if (player[i].velY > 6) {
				player[i].velY = 6;
			}

			// Drag coefficient
			if (!player[i].moveL && !player[i].moveR) {
				player[i].velX = player[i].velX - player[i].velX * player[i].drag;
			}
			if (!player[i].moveU && !player[i].moveD) {
				player[i].velY = player[i].velY - player[i].velY * player[i].drag;
			}
		}
	}
	// Velocity move
	if (moveL) {
		velX -= 1;
	}
	if (moveR) {
		velX += 1;
	}
	if (moveU) {
		velY -= 1;
	}
	if (moveD) {
		velY += 1;
	}

	// Velocity max
	if (velX < -6) {
		velX = -6;
	}
	if (velX > 6) {
		velX = 6;
	}
	if (velY < -6) {
		velY = -6;
	}
	if (velY > 6) {
		velY = 6;
	}

	// Gravity
	x += velX;
	y += velY;

	// Drag coefficient
	if (!moveL && !moveR) {
		velX = velX - velX * drag;
	}
	if (!moveU && !moveD) {
		velY = velY - velY * drag;
	}
}

// Key Pressed
void Playerz::OnKeyDown( Playerz player[], SDL_Keycode sym )
{
	switch (sym) {
	case SDLK_a:
		player[playerID].moveL = true;
		break;
	case SDLK_d:
		player[playerID].moveR = true;
		break;
	case SDLK_w:
		player[playerID].moveU = true;
		break;
	case SDLK_s:
		player[playerID].moveD = true;
		break;
	}
}

// Key Released
void Playerz::OnKeyUp( Playerz player[], SDL_Keycode sym )
{
	switch (sym) {
	case SDLK_a:
		player[playerID].moveL = false;
		break;
	case SDLK_d:
		player[playerID].moveR = false;
		break;
	case SDLK_w:
		player[playerID].moveU = false;
		break;
	case SDLK_s:
		player[playerID].moveD = false;
		break;
	}
}

// Render Playerz(s)
void Playerz::render(SDL_Renderer *gRenderer, Playerz player[], int camx, int camy) {
	for (int i=0; i<maxCount; i++) {
		if (player[i].alive) {
			// Render all Players
			SDL_Rect tempR = { player[i].x-camx, player[i].y-camy, player[i].w, player[i].h };
			SDL_SetRenderDrawColor(gRenderer, 30, 50, 180, 255);
			SDL_RenderFillRect(gRenderer, &tempR);
		}
	}
	// Render Local Playerz
	SDL_Rect tempR = { player[playerID].x-camx, player[playerID].y-camy, player[playerID].w, player[playerID].h };
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(gRenderer, &tempR);

	// Render the Parent object
	tempR = { x-camx, y-camy, w, h };
	SDL_SetRenderDrawColor(gRenderer, 0, 148, 148, 255);
	SDL_RenderFillRect(gRenderer, &tempR);
}

// Get Playerz name
std::string Playerz::getName() {
	return name;
}

// Set Playerz ID
void Playerz::setID(int newID) {
	playerID = newID;
}
