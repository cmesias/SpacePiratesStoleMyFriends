/*
 * Pirate.h
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#ifndef GAME_PIRATE_H_
#define GAME_PIRATE_H_

#include "../Helper.h"
#include "Players.h"
#include "Maps.h"

class Pirate : public Helper {
public:	// Media
	LTexture gTexture;
	LTexture gBoss;
	int count;
	const int max = 100;
	bool asteroidsClear;
public:
	float xCenter, yCenter;
	int radius;
	float x,y;
	float w,h;			// Enemy collision size
	int realw, realh;	// Enemy size in pixels being drawn
	float angle;
	float vX,vY;
	float speed;
	/* Types of Enemies
	 * 0: Normal enemy
	 * 1: Boss enemy
	 */
	int type;
	double health;
	double damage;
	float distance;
	std::string tag;
	bool alive;
	bool collision;
	bool onScreen;


	// these may be deprecated soon
	double timer;			// Used for shooting
	bool attacking;			// pirate attacking
	double attackLength;	// Length of attack

	float atkRange;			// Pirate attack range

private:	// Shooting and Reloading animations

	// For shooting animation
	bool shooting;			// f the pirate is shooting
	float atkSpeed;			// Attack speed
	float atkSpeedTimer;	// Attack speed timer

	// For reloading animation
	int ammo;				// starting with 0 ammo
	int magSize;			// max clip size
	bool reloading;			// If the pirate is reloading
	float reloadSpeed;		// Reload speed
	float reloadTimer;		// Reload timer

public:	// variables used for calculating shooting barrel coordinates

	/*
	 * Clone: 		11
	 * Big Boss: 	119
	 */
	int distanceHeadIsFromCenterOfImage;

	/* how many pixels away the Enemy's barrel is from the center of it's image
	 *	Player's Image: 92, 21
	 *	Clone: 57, 17
	 *	Big Boss: 256, -49
	 */
	double bulletW, bulletH;

	/* Location of Enemy's Barrel for shooting */
	double barrelX, barrelY;

public:
	void Init(Pirate pirate[]);
	void Load(SDL_Renderer* gRenderer);
	void Free();

	void clear(Pirate pirate[]);
	void spawn(Pirate pirate[], float x, float y,
			   float w, float h, int imageW, int imageH,
			   float angle, float speed,
			   int type, double health,
			   int distanceHeadIsFromCenterOfImage, int bulletW, int bulletH);
	void update(Pirate pirate[], Particle particle[], Particle &p_dummy,
				Map &map, Players &player,
				Mix_Chunk* sLazer, Mix_Chunk* sPistolReload,
				int camx, int camy);
	void render(SDL_Renderer* gRenderer, Pirate pirate[], int camx, int camy);
};

#endif /* GAME_PIRATE_H_ */
