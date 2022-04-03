/*
 * Particless.h
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */



#ifndef LOCAL_PARTICLESS_H_
#define LOCAL_PARTICLESS_H_

#include "LTexture.h"
#include "Helper.h"
#include <SDL2/SDL_mixer.h>

class Particle: public Helper {
public:	// other variables
	int count;
	const int max = 512;
	LTexture spr_bullet;					// Bullet Textures
	LTexture spr_bullet_blue;				// Bullet Texture blue
	LTexture gParticles;					// Particle Textures
	LTexture spr_grenade;
	SDL_Rect cParticles[ 6 ];				// [0: Blue], [1: Green], [2: Orange], [3: Red], [4: White], [5: Yellow] Particle


public:	// variables
	float x2, y2;							// particle center
	int radius;								// particle radius
	float x, y;
	float w, h;
	int alpha;
	int alphaspeed;
	int time;
	double deathTimer;
	double deathTimerSpeed;
	float angle;
	double vX, vY;
	double speed;
	double angleSpe, angleDir;
	bool onScreen;
	bool alive;
	bool collide;
	std::string side;
	/* 0: damages enemies
	 * 1: damages player
	 * 2: stars, no damage
	 * 3: grenade, does damage
	 */
	int type;
	std::string tag;		// who the owner of this particle is
	int damage;
	SDL_Color color;
	int layer;

	bool sizeDeath;
	float deathSpe;

	int ptimer = 0;

public:
	bool decay;				// decay particle speed?
	float decaySpeed;		// how much decay are we doing?
	int timeri;				// timer that may be used for anything SPECIFICALLY for something (i.e.: this particle spawns out more particles such as a smoke)

public:	// basic functions
	void init(Particle particle[]);
	void Remove(Particle particle[], int i);
	void RemoveAll(Particle particle[]);
	void load(SDL_Renderer* gRenderer);
	void free();

public:	// functions

	void spawnParticleAngle(Particle particle[], std::string tag, int type,
			float spawnX, float spawnY,
			int spawnW, int spawnH,
			double angle, double speed,
			double damage,
			SDL_Color color, int layer,
			int angleSpe, int angleDir,
			int alpha, int alphaspeed,
			int deathTimer, int deathTimerSpeed,
			bool sizeDeath, float deathSpe,
			bool decay = false, float decaySpeed = 0.0);

	void spawnBulletParticleAngle(Particle particle[], std::string tag, int type,
			float spawnX, float spawnY,
			int spawnW, int spawnH,
			double angle, double speed,
			double damage,
			SDL_Color color, int layer,
			int angleSpe, int angleDir,
			int alpha, int alphaspeed,
			int deathTimer, int deathTimerSpeed,
			bool sizeDeath, float deathSpe,
			bool decay = false, float decaySpeed = 0.0);

	/*void fireParticle(Particle particle[], int type, int damage,
										   int spawnx, int spawny, int w, int h,
										   double speed, float vX, float vY,
										   int alphaspeed, int alpha,
										   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
										   SDL_Color color, int layer);
	void fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
											int alphaspeed, int alpha,
											int deathTimer, int deathTimerSpeed,
											double speed,
											SDL_Color color, int layer);*/
	void updateBulletParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);
	void renderBulletParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer);

public:	// Star
	void genStars(Particle particle[], int screnWidth, int screenHeight);
	void updateStarParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);
	void renderStarParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer);
};

#endif /* LOCAL_PARTICLESS_H_ */
