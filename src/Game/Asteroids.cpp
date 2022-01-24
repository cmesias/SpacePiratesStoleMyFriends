/*
 * Asteroids.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "Asteroids.h"

// Load asteroid resources
void Asteroid::loadResources(SDL_Renderer* gRenderer){
	gAsteroid.loadFromFile(gRenderer, "resource/gfx/asteroid.png");
	setClips(rAsteroid[0], 0, 0, 48, 48);
	setClips(rAsteroid[1], 48, 0, 96, 96);
	setClips(rAsteroid[2], 144, 0, 128, 128);

	//gAsteroid.setBlendMode(SDL_BLENDMODE_ADD);
}

// Free asteroid resources
void Asteroid::freeResources(){
	gAsteroid.free();
}


// Initialize
void Asteroid::init(Asteroid asteroid[]) {
	ASTEROIDS 	= 0;
	asteroidsClear	= true;
	for (int i = 0; i < 128; i++) {
		asteroid[i].x 				= 0;
		asteroid[i].y 				= 0;
		asteroid[i].w 				= 0;
		asteroid[i].h 				= 0;
		asteroid[i].vX 				= 0.0;
		asteroid[i].vY 				= 0.0;
		asteroid[i].speed 			= 0.0;
		asteroid[i].angle 			= 0.0;
		asteroid[i].angleSpe 		= 0;
		asteroid[i].angleDir 		= 0;
		asteroid[i].damagedTimer 	= 0;
		asteroid[i].health 			= 100;
		asteroid[i].alive 			= false;
		asteroid[i].collision 		= false;
		asteroid[i].mouse 			= false;
		asteroid[i].damaged 		= false;
		asteroid[i].onScreen 		= false;
		asteroid[i].type 			= "";
	}
}

// Clear all asteroids
void Asteroid::clear(Asteroid asteroid[]){
	for (int i = 0; i < 128; i++) {
		asteroid[i].alive = false;
	}
	this->ASTEROIDS = 0;
}

// Create an Asteroid
void Asteroid::spawnAsteroidAngle(Asteroid asteroid[],
		int x, int y,
		int w, int h,
		double angle, double speed) {
	for (int i = 0; i < 128; i++)
	{
		if (!asteroid[i].alive)
		{
			asteroid[i].w 				= w;
			asteroid[i].h 				= h;
			if (w == 48){
				asteroid[i].type 		= "small";
			}else if (w==96){
				asteroid[i].type 		= "medium";
			}else if (w==128){
				asteroid[i].type 		= "large";
			}
			asteroid[i].x 				= x;
			asteroid[i].y 				= y;
			asteroid[i].speed 			= speed;
			asteroid[i].vX 				= cos( (3.14159265/180)*(angle) );
			asteroid[i].vY 				= sin( (3.14159265/180)*(angle) );
			asteroid[i].angle 			= 0.0;
			asteroid[i].damagedTimer 	= 0;
			asteroid[i].health			= 100;
			asteroid[i].angleSpe 		= randDouble(1.0, 2.5);
			if (asteroid[i].angleSpe % 2 == 0){
				asteroid[i].angleDir 	= 1;}else{
				asteroid[i].angleDir 	= -1;}
			asteroid[i].mouse = false;
			asteroid[i].collision 		= false;
			asteroid[i].alive 			= true;
			asteroid[i].onScreen 		= false;
			asteroid[i].damaged 		= false;
			ASTEROIDS++;
			break;
		}
	}
}

void Asteroid::spawnAsteroid(Asteroid asteroid[], int spawnx, int spawny, int w, int h, int targetx,int targety, float speed, std::string type) {
	for (int i = 0; i < 128; i++) {
		if (!asteroid[i].alive) {
			asteroid[i].health 			= 100;
			asteroid[i].type 			= type;
			asteroid[i].x 				= spawnx;
			asteroid[i].y 				= spawny;
			asteroid[i].w 				= w;
			asteroid[i].h 				= h;
			asteroid[i].speed 			= speed;
			float distance 				= sqrt((targetx - spawnx)* (targetx - spawnx - asteroid[i].w / 2)+
										   	   (targety - spawny)* (targety - spawny - asteroid[i].h / 2));
			asteroid[i].vX 				= (targetx - spawnx - asteroid[i].w / 2) / distance;
			asteroid[i].vY 				= (targety - spawny - asteroid[i].h / 2) / distance;
			asteroid[i].damagedTimer 	= 0;
			asteroid[i].angle 			= 0.0;
			asteroid[i].angleSpe 		= randDouble(1.0, 2.5);
			if (asteroid[i].angleSpe % 2 == 0){
				asteroid[i].angleDir 	= 1;}else{
				asteroid[i].angleDir 	= -1;}
			asteroid[i].collision 		= false;
			asteroid[i].damaged 		= false;
			asteroid[i].mouse = false;
			asteroid[i].onScreen 		= false;
			asteroid[i].alive 			= true;
			ASTEROIDS++;
			break;
		}
	}
}

// [0]: remove asteroid,
void Asteroid::configAsteroid(Asteroid asteroid[], int command, int mx, int my) {
	for (int i = 0; i < 128; i++) {
		if (asteroid[i].alive) {
			switch (command){
				case 0:
					if (asteroid[i].mouse) {
						asteroid[i].alive = false;
						ASTEROIDS--;
					}
				break;
			}
		}
	}
}

void Asteroid::copyAsteroidOnMouse(Asteroid asteroid[], int &BLOCKX, int &BLOCKY, int &BLOCKW, int &BLOCKH) {
	//COPY THE WIDTH/HEIGHT OF THE BLOCK YOU ARE CURRENTLY OVER
	for (int i = 0; i < 128; i++) {
		if (asteroid[i].alive) {
			if (asteroid[i].mouse){
				BLOCKX = asteroid[i].x;
				BLOCKY = asteroid[i].y;
				BLOCKW = asteroid[i].w;
				BLOCKH = asteroid[i].h;
			}
		}
	}
}

// Update asteroid
void Asteroid::updateAsteroid(Asteroid asteroid[],
							  Particle particle[], Particle &p_dummy,
							  bool alive, bool shield,
							  unsigned int &score, int &mileScore,
							  int &health, int mx, int my, int camx, int camy,
							  int screenWidth, int screenHeight,
							  int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < 128; i++)
	{
		if (asteroid[i].alive)
		{

			// Asteroid movement
			asteroid[i].x += asteroid[i].vX * asteroid[i].speed;
			asteroid[i].y += asteroid[i].vY * asteroid[i].speed;

			// Asteroid spin
			asteroid[i].angle += asteroid[i].angleSpe * asteroid[i].angleDir;

			// Asteroid screen check
			if (asteroid[i].x + asteroid[i].w >= camx && asteroid[i].x <= camx + screenWidth &&
				asteroid[i].y + asteroid[i].h >= camy && asteroid[i].y <= camy + screenHeight) {
				asteroid[i].onScreen = true;
			}else{
				asteroid[i].onScreen = false;
			}

			// Mouse on Asteroid
			if (mx > asteroid[i].x && mx < asteroid[i].x + asteroid[i].w &&
				my > asteroid[i].y && my < asteroid[i].y + asteroid[i].h) {
				asteroid[i].mouse = true;
			} else {
				asteroid[i].mouse = false;
			}

			//Asteroid damaged animation timer
			if (asteroid[i].damaged){
				asteroid[i].damagedTimer += 1;
				if (asteroid[i].damagedTimer > 20){
					asteroid[i].damaged = false;
					asteroid[i].damagedTimer = 0;
				}
			}

			// Asteroid death
			if (asteroid[i].health <= 0)
			{
				//Spawn explosion after asteroid death
				/*for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
					int rands = rand() % 3 + 2;
					p_dummy.spawnParticleAngle(particle, "none", 2,
									   asteroid[i].x+asteroid[i].w/2-rands/2,
									   asteroid[i].y+asteroid[i].h/2-rands/2,
									   rands, rands,
									   j, randDouble(0.1, 1.5),
									   0.0,
									   {255, 255, 255, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 2,
									   100, 0,
									   false, 0);
				}*/

				if (asteroid[i].type=="large"){
					for (int j = 0; j < 2; j++) {
			            double randAngle 	= randDouble(0.0, 360.0);
			            double radian		= (3.14159265/180)*(randAngle);
			            double radianCos	= cos(radian);
			            double radianSin	= sin(radian);
			            spawnAsteroidAngle(asteroid, asteroid[i].x+asteroid[i].w/2-96/2 + (10 * radianCos),
			            							 asteroid[i].y+asteroid[i].h/2-96/2 + (10 * radianSin),
													 96, 96,
			    									 randAngle, 1.3);
					}
					score += 20;
					mileScore += 20;

				}else if (asteroid[i].type=="medium"){
					for (int j = 0; j < 2; j++) {
			            double randAngle 	= randDouble(0.0, 360.0);
			            double radian		= (3.14159265/180)*(randAngle);
			            double radianCos	= cos(radian);
			            double radianSin	= sin(radian);
			            spawnAsteroidAngle(asteroid, asteroid[i].x+asteroid[i].w/2-48/2 + (10 * radianCos),
			            							 asteroid[i].y+asteroid[i].h/2-48/2 + (10 * radianSin),
													 48, 48,
			    									 randAngle, 2.3);
					}
					score += 50;
					mileScore += 50;
				}else if (asteroid[i].type=="small"){
					score += 100;
					mileScore += 100;
				}
				asteroid[i].alive = false;
				ASTEROIDS--;
			}

			// Asteroid and Map collision
			if (asteroid[i].x+asteroid[i].w < mapX) {
				asteroid[i].x = mapX+mapW;
			}
			if (asteroid[i].x > mapX+mapW) {
				asteroid[i].x = mapX-asteroid[i].w;
			}
			if (asteroid[i].y+asteroid[i].h < mapY) {
				asteroid[i].y = mapY+mapH;
			}
			if (asteroid[i].y > mapY+mapH) {
				asteroid[i].y = mapY-asteroid[i].h;
			}
		}
	}
}

//Render asteroid
void Asteroid::renderAsteroid(Asteroid asteroid[], int camx, int camy, SDL_Renderer* gRenderer) {
	for (int i = 0; i < 128; i++) {
		if (asteroid[i].alive) {

			//if (asteroid[i].onScreen){
			//}

			if (asteroid[i].damaged) {
				gAsteroid.setColor(255, 255, 0);
			}else{
				gAsteroid.setColor(255, 255, 255);
			}

			if (asteroid[i].type=="large"){
				gAsteroid.render(gRenderer, asteroid[i].x - camx, asteroid[i].y - camy, asteroid[i].w, asteroid[i].h, &rAsteroid[2], asteroid[i].angle);
			}
			if (asteroid[i].type=="medium"){
				gAsteroid.render(gRenderer, asteroid[i].x - camx, asteroid[i].y - camy, asteroid[i].w, asteroid[i].h, &rAsteroid[1], asteroid[i].angle);
			}
			if (asteroid[i].type=="small"){
				gAsteroid.render(gRenderer, asteroid[i].x - camx, asteroid[i].y - camy, asteroid[i].w, asteroid[i].h, &rAsteroid[0], asteroid[i].angle);
			}


			/*std::stringstream tempsi;

			tempsi.str(std::string());
			tempsi << "asteroid[i].x: " << asteroid[i].x;
			gText.loadFromRenderedText(tempsi.str().c_str(), white, gFont13);
			gText.render(50, 0, gText.getWidth(), gText.getHeight());

			tempsi.str(std::string());
			tempsi << "asteroid[i].y: " << asteroid[i].y;
			gText.loadFromRenderedText(tempsi.str().c_str(), white, gFont13);
			gText.render(50, 15, gText.getWidth(), gText.getHeight());

			tempsi.str(std::string());
			tempsi << "asteroid[i].w: " << asteroid[i].w;
			gText.loadFromRenderedText(tempsi.str().c_str(), white, gFont13);
			gText.render(50, 30, gText.getWidth(), gText.getHeight());

			tempsi.str(std::string());
			tempsi << "asteroid[i].h: " << asteroid[i].h;
			gText.loadFromRenderedText(tempsi.str().c_str(), white, gFont13);
			gText.render(50, 45, gText.getWidth(), gText.getHeight());

			tempsi.str(std::string());
			tempsi << "asteroid[i].alive: " << asteroid[i].alive;
			gText.loadFromRenderedText(tempsi.str().c_str(), white, gFont13);
			gText.render(50, 60, gText.getWidth(), gText.getHeight());


			// Render asteroid collision box
			SDL_Rect tempRect = { asteroid[i].x  - camx, asteroid[i].y - camy, asteroid[i].w, asteroid[i].h };
			SDL_SetRenderDrawColor(gRenderer12, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer12, &tempRect);

			//If the player's mouse is on top of asteroid then show certain details of asteroid
			if (asteroid[i].mouse) {
				SDL_Rect tempRect = { asteroid[i].x  - camx, asteroid[i].y - camy, asteroid[i].w, asteroid[i].h };
				SDL_SetRenderDrawColor(gRenderer12, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer12, &tempRect);
			}

			if (asteroid[i].damaged) {
				SDL_Rect blockcb = { asteroid[i].x  - camx, asteroid[i].y
						- camy, asteroid[i].w, asteroid[i].h };
				SDL_SetRenderDrawColor(gRenderer12, 244, 100, 0, 255);
				SDL_RenderDrawRect(gRenderer12, &blockcb);
			}*/
		}
	}
}

