/*
 * Playerz.h
 *
 *  Created on: Apr 19, 2017
 *      Author: Carl
 */

#ifndef PLAYERZ_H_
#define PLAYERZ_H_

/* Putting this here will prevent errors
 * if some other class includes this class */
#include <iostream>

#include <SDL2/SDL.h>
//#include "LTexture.h"

class Playerz  {
public:
	Playerz();
	//virtual ~Playerz();

public:
	void OnKeyDown(  Playerz player[], SDL_Keycode sym);
	void OnKeyUp( Playerz player[], SDL_Keycode sym );

public:
	std::string getName();
	void setID(int newID);

public:
	void init(Playerz player[]);
	void create(Playerz player[], float x, float y, int w, int h, std::string name);
	void update(Playerz player[]);
	void render(SDL_Renderer *gRenderer, Playerz player[], int camx, int camy);

public:
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
	std::string name;

private:
	int playerID;
	int count;
	const int maxCount = 10;
};

#endif /* PLAYERZ_H_ */
