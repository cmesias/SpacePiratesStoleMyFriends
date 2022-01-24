/*
 * Asteroids.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#ifndef LOCAL_ASTEROIDS_H_
#define LOCAL_ASTEROIDS_H_

#include "../Helper.h"
#include "../LTexture.h"
#include "../Particless.h"

class Asteroid : public Helper {
public:	// Media
	LTexture gAsteroid;
	SDL_Rect rAsteroid[3];
public:
	// Asteroid variables
	float x,y,w,h;
	float angle;
	int angleSpe, angleDir;
	float vX,vY;
	float speed;
	bool alive;
	bool collision;
	bool mouse;
	bool onScreen;
	std::string type;
	int health;
	bool damaged; 		// Asteroid is damaged
	int damagedTimer;	// Asteroid damaged animation timer
	int timer;			// Used for shooting

	// Local Variables (not used directly by Asteroid member)
	int ASTEROIDS 	= 0;
	bool asteroidsClear	= true;

	void loadResources(SDL_Renderer* gRenderer);

	void freeResources();

	void init(Asteroid asteroid[]);

	void clear(Asteroid asteroid[]);

	void spawnAsteroidAngle(Asteroid asteroid[],
			int x, int y,
			int w, int h,
			double angle, double speed);

	void spawnAsteroid(Asteroid asteroid[], int spawnx, int spawny, int w, int h, int targetx,int targety, float speed, std::string type);

	void configAsteroid(Asteroid asteroid[], int command, int mx, int my);

	void copyAsteroidOnMouse(Asteroid asteroid[], int &BLOCKX, int &BLOCKY, int &BLOCKW, int &BLOCKH);

	void updateAsteroid(Asteroid asteroid[],
						Particle particle[], Particle &p_dummy,
						bool alive, bool shield,
						unsigned int &score, int &mileScore,
						int &health, int mx, int my, int camx, int camy,
						int screenWidth, int screenHeight,
						int mapX, int mapY, int mapW, int mapH);

	void renderAsteroid(Asteroid asteroid[], int camx, int camy, SDL_Renderer* gRenderer);
};

#endif /* LOCAL_ASTEROIDS_H_ */
