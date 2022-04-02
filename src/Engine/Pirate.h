/*
 * Pirate.h
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#ifndef ENGINE_PIRATE_H_
#define ENGINE_PIRATE_H_

#include "Maps.h"
#include "Players.h"
#include "Helper.h"

class Pirate : public Helper {
public:	// Media
	/*
	 * 12 monsters in Total
	 * 108 clips
	 * 9 x 12 ( 9 clips going to the right, 12  going down)
	 */
	SDL_Rect clip[108];
	LTexture gTexture;
	LTexture gBoss;
	bool asteroidsClear;

public:	// Min variables
	float xCenter, yCenter;
	int radius;
	float x,y;
	float w,h;			// Enemy collision size
	int realw, realh;	// Enemy size in pixels being drawn
	float angle;
	float vX,vY;
	float speed;
	float health;
	float healthDecay;
	float maxHealth;
	float damage;
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

public:	// Main functions
	void Init(Pirate pirate[]);
	void Load(SDL_Renderer* gRenderer);
	void Free();

	void clear(Pirate pirate[]);
	void spawn(Pirate pirate[], float x, float y,
			   float w, float h, int imageW, int imageH,
			   float angle, float speed,
			   int distanceHeadIsFromCenterOfImage, int bulletW, int bulletH);
	void update(Pirate pirate[], Particle particle[], Particle &p_dummy,
				Map &map, Players &player,
				Mix_Chunk* sLazer, Mix_Chunk* sPistolReload,
				int camx, int camy);
	void render(SDL_Renderer* gRenderer, Pirate pirate[], int camx, int camy);

	void RenderUI(SDL_Renderer* gRenderer, Pirate pirate[], int camx, int camy);

	float getCenterX(Pirate pirate[], int i);

	float getCenterY(Pirate pirate[], int i);
	// This function will determine a Monster's spells (usually called after loading Monsters)
	void SetStatsBasedOnType(Pirate pirate[], int i);


public: // Editor Variables

	/* Types of Enemies
	 * 0: Normal enemy
	 * 1: Boss enemy
	 */
	int type;

	int count;
	const int max = 100;
	const int maxTypes = 13;	// max number of unique Monsters
	int multiW = 1;
	int multiH = 1;

	// Mouse
	bool mouse;				// mouse is on top of Monster
	bool mouseBox;			// if tile placement size of on top of monster

public:	// Editor methods

	void Remove(Pirate pirate[], int click);

	void RemoveAll(Pirate pirate[]);

	void EditorUpdate(Pirate pirate[], int newMx, int newMy, int mex, int mey, int camx, int camy);

public: // Save functions

	// Load data
	void LoadData(Pirate pirate[], int level);

	// Save data
	std::string SaveData(Pirate pirate[]);
};

#endif /* ENGINE_PIRATE_H_ */
