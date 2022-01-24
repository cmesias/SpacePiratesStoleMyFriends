/*
 * Object.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Carl
 */

#include <iostream>
#include <SDL2/SDL_image.h>
#include "Object.h"

Object::Object(std::string newName, float newX, float newY) {
	x = newX;
	y = newY;
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
	id	  = -1;
	name  = newName;
	count = 0;
}

/*Object::~Object() {
	// TODO Auto-generated destructor stub
}*/

/*// Set X Velocity
void Object::setVelX(float newVelX, bool relative = false) {
	if (relative)
		velX += newVelX;
	else
		velX = newVelX;
}

// Set Y Velocity
void Object::setVelY(float newVelY, bool relative = false) {
	if (relative)
		velY += newVelY;
	else
		velY = newVelY;
}*/


// Update object
void Object::update() {

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

void Object::OnEvent(SDL_Event* Event)
{
	CEvent::OnEvent(Event);
}

// Key Pressed
void Object::OnKeyDown( SDL_Keycode sym, Uint16 mod, Uint16 scancode )
{
	switch ( sym ) {
	case SDLK_a:
		moveL = true;
		break;
	case SDLK_d:
		moveR = true;
		break;
	case SDLK_w:
		moveU = true;
		break;
	case SDLK_s:
		moveD = true;
		break;
	}
}

// Key Released
void Object::OnKeyUp( SDL_Keycode sym, Uint16 mod, Uint16 scancode )
{
	switch ( sym ) {
	case SDLK_a:
		moveL = false;
		break;
	case SDLK_d:
		moveR = false;
		break;
	case SDLK_w:
		moveU = false;
		break;
	case SDLK_s:
		moveD = false;
		break;
	}
}

// Render Object(s)
void Object::render(SDL_Renderer *gRenderer, int camx, int camy) {
	SDL_Rect tempR = { x-camx, y-camy, w, h };
	SDL_SetRenderDrawColor(gRenderer, 144, 20, 20, 255);
	SDL_RenderFillRect(gRenderer, &tempR);
}

// Get Object name
std::string Object::getName() {
	return name;
}
