/*
 * Zombie.h
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#ifndef GAME_ZOMBIE_H_
#define GAME_ZOMBIE_H_

#include "../Helper.h"
#include "Players.h"
#include "Maps.h"

class Zombie : public Helper {
public:	// Media
	LTexture gTexture;
	LTexture gBoss;
	int count;
	const int max = 100;
	bool asteroidsClear;
public:
	float x2, y2;
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


	double timer;			// Used for shooting
	bool attacking;			// zombie attacking
	double attackLength;	// Length of attack

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
	void Init(Zombie zombie[]);
	void Load(SDL_Renderer* gRenderer);
	void Free();

	void clear(Zombie zombie[]);
	void spawn(Zombie zombie[], float x, float y,
			   float w, float h, int imageW, int imageH,
			   float angle, float speed,
			   int type, double health,
			   int distanceHeadIsFromCenterOfImage, int bulletW, int bulletH);
	void update(Zombie zombie[], Particle particle[], Particle &p_dummy,
				Map &map, Players &player, Mix_Chunk* sLazer,
				int camx, int camy);
	void render(SDL_Renderer* gRenderer, Zombie zombie[], int camx, int camy);
};

#endif /* GAME_ZOMBIE_H_ */
