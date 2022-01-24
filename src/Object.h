/*
 * Object.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Carl
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL2/SDL.h>
#include "CEvent.h"

class Object: public CEvent  {
public:
	Object(std::string newName, float newX, float newY);
	//virtual ~Object();

	//void setVelX(float newVelX, bool relative);
	//void setVelY(float newVelY, bool relative);

public:
	std::string getName();

public:
	void update();
	void render(SDL_Renderer *gRenderer, int camx, int camy);

public: /* CEvent */
	void OnEvent(SDL_Event *Event);
	void OnKeyDown( SDL_Keycode sym, Uint16 mod, Uint16 scancode );
	void OnKeyUp( SDL_Keycode sym, Uint16 mod, Uint16 scancode );

private:
	float x;
	float y;
	int w;
	int h;
	float velX;
	float velY;
	float drag;
	bool moveL;
	bool moveR;
	bool moveU;
	bool moveD;
	bool alive;
	int id;
	std::string name;

private:
	int count;						// Player count
	const int maxCount = 10;		// Max count
};

#endif /* OBJECT_H_ */
