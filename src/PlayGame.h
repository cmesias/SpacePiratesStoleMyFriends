/*
 * PlayGame.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef PLAYGAME_H_
#define PLAYGAME_H_

#include "Game/Asteroids.h"
#include "Game/Enemies.h"
#include "Game/Maps.h"
#include "Game/Pirate.h"
//#include "Game/Pause.h"
#include "Game/Players.h"
#include "Game/Spawners.h"
#include "Options.h"
#include "Helper.h"

class PlayGame : public Helper, public Options {
public:	// specific game rules

public:	// globals

public: // System variables
	enum Result { Back, Nothing, StartGame, ShowingMenu, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, PlayGame::Result &result);
	PlayGame::Result mousePressed(SDL_Event event);
	PlayGame::Result mouseReleased(SDL_Event event);

public:	// Scene textures
	LTexture gBG;
	LTexture gTargetTexture;
	LTexture gCircle;

	class Text {
	public:
		int count;
		float x;
		float y;
		int w;
		int h;
		int alpha;
		int timer;
		bool alive;
		float vX;
		float vY;
		std::string textfield;
		SDL_Color color;
		void init(Text text[]) {
			count =0;
			for (int i=0; i<100; i++) {
				text[i].alive = false;
				text[i].color = {255,255,255};
			}
		}
		void spawn(Text text[], float x, float y, float vX, float vY, int alpha, std::string textfield, SDL_Color color = {255,255,255}) {
			for (int i=0; i<100; i++) {
				if (!text[i].alive) {
					text[i].x = x;
					text[i].y = y;
					text[i].w = 139;
					text[i].h = 96;
					text[i].vX = vX;
					text[i].vY = vY;
					text[i].timer = timer;
					text[i].color = color;
					text[i].alpha = alpha;
					text[i].textfield = textfield;
					text[i].alive = true;
					count++;
					break;
				}
			}
		}
		void update(Text text[]) {
			for (int i=0; i<100; i++) {
				if (text[i].alive) {
					text[i].x += text[i].vX;
					text[i].y += text[i].vY;

					text[i].alpha -= 16;
					if (text[i].alpha <= 0) {
						text[i].alive = false;
						count--;
					}
				}
			}
		}
	};

	Text tex;
	Text text[100];

public:	// Other classes
	Pirate pira;
	Map map;
	Particle part;
	Asteroid aste;
	Spawner spaw;
	Enemy enem;
	//Pause pau;
	//Pause::Result pauseResult;
	Players player;

	// Class instance, multiple
	Pirate pirate[100];
	Particle particles[512];
	Asteroid asteroid[128];
	Enemy enemy[8];
	Spawner spawner[200];
	//Mixed mix;
	//Controls cont;
	//static Items items[10];
public:
    // camera
    int camx;
    int camy;

public:	// Variables

	// Initialize
	void Init();

	// Load resources
	void Load(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Free resources
	void Free();

	// Update everything
	void Update(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Render foreground
	void RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render objects
	void Render(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render debug information
	void RenderDebug(SDL_Renderer *gRenderer);

	// Render text
	void RenderText(SDL_Renderer *gRenderer, LWindow &gWindow);

public:	// Functions mixed with other classes
	// Check collision between Particle & Asteroid
	void checkCollisionPlayerAsteroid(Players &player, Asteroid asteroid[]);

	// Check collision between Particle & Asteroid
	void checkCollisionParticleAsteroid(Particle particle[], Particle &part, Asteroid asteroid[], Players &player);

	// Check collision between Particle & Enemy
	void checkCollisionParticleEnemy(Particle particle[], Particle &part, Enemy enemy[], Players &player);

	// Check collision between Particle & Pirate
	void checkCollisionParticlePirate();

	// Check collision between Particle & Player
	void checkCollisionParticlePlayer(Particle particle[], Particle &part, Players &player);

	// Check collision between Grenade Particle & Enemies
	void checkCollisionGrenadePlayer();

	// Spawn Asteroids upon destroying all Asteroids
	void spawnAsteroidsNow2();

public:	// Functions that serve multiple purposes

	// Knock-back effect: take the position of the explosion, and the object your are knocking back
	void knockbackEffect(float targetX, float targetY, int targetW, int targetH,
						 float objectX, float objectY, int objectW, int objectH,
						 float &objectVX,float &objectVY, float force);

private:	// used for some debugging
	bool debug;
	bool editor;
	int mx, my;
	bool leftClick;
	bool quit;
	bool shift;
	SDL_Event event;
};

#endif /* PLAYGAME_H_ */
