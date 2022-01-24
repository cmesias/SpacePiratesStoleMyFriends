/*
 * Zombie.cpp
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../LTexture.h"
#include "../Particless.h"
#include "Zombie.h"



// Clear all zombie
void Zombie::clear(Zombie zombie[]){
	for (int i = 0; i < max; i++) {
		zombie[i].alive = false;
	}
	count = 0;
}

void Zombie::Init(Zombie zombie[]){
	count = 0;
	asteroidsClear	= true;
	for (int i = 0; i < max; i++) {
		zombie[i].x 				= 0;
		zombie[i].y 				= 0;
		zombie[i].w 				= 0;
		zombie[i].h 				= 0;
		zombie[i].vX 				= 0.0;
		zombie[i].vY 				= 0.0;
		zombie[i].speed 			= 0.0;
		zombie[i].angle 			= 0.0;
		zombie[i].health 			= 100;
		zombie[i].damage 			= 5;
		zombie[i].distance 			= 1;
		zombie[i].alive 			= false;
		zombie[i].collision 		= false;
		zombie[i].onScreen 			= false;
		zombie[i].attacking 		= false;
		zombie[i].attackLength 		= 0;
		zombie[i].type 				= 0;
		zombie[i].timer 			= 0;
	}
}

void Zombie::Load(SDL_Renderer* gRenderer){
	gTexture.loadFromFile(gRenderer, "resource/gfx/player_clone.png");
	gBoss.loadFromFile(gRenderer, "resource/gfx/boss.png");
}

void Zombie::Free(){
	gTexture.free();
	gBoss.free();
}



// Create an zombie
void Zombie::spawn(Zombie zombie[], float x, float y,
		           float w, float h, int realw, int realh,
				   float angle, float speed,
				   int type, double health,
				   int distanceHeadIsFromCenterOfImage,
				   int bulletW, int bulletH) {
	for (int i = 0; i < max; i++)
	{
		if (!zombie[i].alive)
		{
			zombie[i].tag 				= "enemy";
			zombie[i].x 				= x;
			zombie[i].y 				= y;
			zombie[i].w 				= w;
			zombie[i].h 				= h;
			zombie[i].realw 			= realw;
			zombie[i].realh 			= realh;
			zombie[i].radius 			= w/2;
			zombie[i].x2 				= x + w/2;
			zombie[i].y2 				= y + h/2;
			zombie[i].speed 			= speed;
			//zombie[i].vX 				= cos( (3.14159265/180)*(angle) );
			//zombie[i].vY 				= sin( (3.14159265/180)*(angle) );
			zombie[i].vX 				= 0.0;
			zombie[i].vY 				= 0.0;
			zombie[i].angle 			= 0.0;
			zombie[i].bulletW 			= bulletW;
			zombie[i].bulletH 			= bulletH;
			//------------------------------------------------------------------------//
			/* initial centering of zombie's circle collision shape */
			float radians 				= (3.1415926536/180)*(0.0);
			float Cos 					= floor(cos(radians)*10+0.5)/10;
			float Sin 					= floor(sin(radians)*10+0.5)/10;
			zombie[i].distanceHeadIsFromCenterOfImage	= distanceHeadIsFromCenterOfImage;
			int newW					= distanceHeadIsFromCenterOfImage * (-Cos);
			int newH 					= distanceHeadIsFromCenterOfImage * (-Sin);
			zombie[i].x2 				= x+w/2 + newW - zombie[i].radius;
			zombie[i].y2 				= y+h/2 + newH - zombie[i].radius;
			//------------------------------------------------------------------------//
			zombie[i].health			= health;
			zombie[i].damage			= 5;
			zombie[i].type 				= type;
			zombie[i].distance 			= 1;
			zombie[i].timer 			= 0;
			zombie[i].attackLength		= 180;
			zombie[i].attacking 		= false;
			zombie[i].collision 		= false;
			zombie[i].onScreen 			= false;
			zombie[i].alive 			= true;
			count++;
			break;
		}
	}
}

// TODO [ ] - do something about having a mix chunk in this class, maybe have all manipulator code inside of PlayerGame class
// TODO [ ] - inside Player class, after player dies and comes back to life, zombies position is persistent, fix this.
//			- Following up on the last sentence, the players animations (rustling) do not move as well

// TODO [x] - for the enemy, also render a circle to see where the enemy is, we will be doing
//			- circle collision for this game

// TODO [x] - get circle collision boxes for zombie, and also change position of particle that comes out of player
// 			- and make it be on the green box, not the white (if possible)

// TODO [ ] - instead of the enemy turning their body instantly towards the player,
// 			- have them do a turn of some sort to face the player. This will probably
// 			- require the use of creating a distance formula in the angle (0-360)

// Update zombie
void Zombie::update(Zombie zombie[], Particle particle[], Particle &p_dummy,
		Map &map, Players &player, Mix_Chunk* sLazer,
		int camx, int camy) {
	for (int i = 0; i < max; i++)
	{
		if (zombie[i].alive)
		{
			// zombie target
			float bmx2 = zombie[i].x+zombie[i].w/2;
			float bmy2 = zombie[i].y+zombie[i].h/2;
			float bmx  = player.x2+player.radius;
			float bmy  = player.y2+player.radius;
			// zombie distance from target
			zombie[i].distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
			if (zombie[i].distance <= 1) {
				zombie[i].distance = 1;
			}

			// zombie moves if target is less than 768 pixels and greater than 384 pixels away
			float constVX = 0.0;
			float constVY = 0.0;
			// shootPlayerDistance = 384;
			if (zombie[i].distance > 600 && zombie[i].distance < 800) {
				constVX = zombie[i].speed * (bmx - bmx2) / zombie[i].distance;
				constVY = zombie[i].speed * (bmy - bmy2) / zombie[i].distance;
				//zombie[i].x += zombie[i].speed * (bmx - bmx2) / zombie[i].distance;
				//zombie[i].y += zombie[i].speed * (bmy - bmy2) / zombie[i].distance;

				zombie[i].angle = atan2(bmy - bmy2,bmx - bmx2);
				zombie[i].angle = zombie[i].angle * (180 / 3.1416);

				if (zombie[i].angle < 0) {
					zombie[i].angle = 360 - (-zombie[i].angle);
				}
			}

			// Zombie shoots if target is 768 pixels away, start attack (attacks for 3 seconds)
			// calculate some things for shooting
			float radians = (3.1415926536/180)*(zombie[i].angle);
			if (zombie[i].distance <= 880) {

				zombie[i].angle = atan2(bmy - bmy2,bmx - bmx2);
				zombie[i].angle = zombie[i].angle * (180 / 3.1416);
				if (zombie[i].angle < 0) {
					zombie[i].angle = 360 - (-zombie[i].angle);
				}

				// calculate some things for shooting
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;
				int newW = zombie[i].distanceHeadIsFromCenterOfImage * (-Cos);
				int newH = zombie[i].distanceHeadIsFromCenterOfImage * (-Sin);
				zombie[i].x2 = zombie[i].x+zombie[i].w/2 + newW - zombie[i].radius;
				zombie[i].y2 = zombie[i].y+zombie[i].h/2 + newH - zombie[i].radius;


				/*
				// calculate some things for shooting
				float radians = (3.1415926536/180)*(zombie[i].angle);
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;

				// get player new center position


				// calculate some things for shooting
				float radians = (3.1415926536/180)*(zombie[i].angle);
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;
				int newW = 11 * (-Cos);
				int newH = 11 * (-Sin);
				zombie[i].x2 = zombie[i].x+zombie[i].w/2 + newW - zombie[i].radius;
				zombie[i].y2 = zombie[i].y+zombie[i].h/2 + newH - zombie[i].radius;

				double barrelW  = (57 * cos(radians) ) - (16 * sin(radians) );				// add this to center of zombie (this will give us the guns barrel position)
				double barrelH  = (57 * sin(radians) ) + (16 * cos(radians) );
				int realw = 0;															// zombie actual size in pixels
				int realh = 0;															// zombie actual size in pixels
				if (zombie[i].type == 0) {
					realw = 128;															// zombie actual size in pixels
					realh = 128;
				}
				else if (zombie[i].type == 1) {
					realw = 512;															// zombie actual size in pixels
					realh = 512;
				}


				double particleW = 24;														// particle collision size
				double particleH = 24;
				int wDifference = realw - zombie[i].w;										// get difference between zombie collision size and actual size (used to center bullets)
				int hDifference = realh - zombie[i].h;
				double barrelX = zombie[i].x+realw/2-wDifference/2 - particleW/2 + barrelW;	// get final position to spawn a particle
				double barrelY = zombie[i].y+realh/2-hDifference/2 - particleH/2 + barrelH;
				*/

				// Enemy shooting
				/*zombie[i].timer += 1;
				if (zombie[i].timer > 45){
					//zombie[i].vX = Cos * 11 - rand() % 8 + 5;
					//zombie[i].vY = Sin * 11 - rand() % 8 + 5;
					// restart shoo ttimer
					zombie[i].timer = 0;
					// play shoot sound effect
					Mix_PlayChannel(3, sLazer, 0);
					// spawn particle
					p_dummy.spawnParticleAngle(particle, 1,
							barrelX,
							barrelY,
							particleW, particleH,
							zombie[i].angle, 11,
						   5,
						   {255, 255,0}, 1,
						   0.0, 0.0,
						   255, 0,
						   100, 1,
						   false, 0);
				}*/
			}

				// Clone
				if (zombie[i].type == 0) {
					double particleW = 24;														// particle collision size
					double particleH = 24;

					/* If the object is at a rotation of 0.0 (facing right),
					 * then this will be the distance the particle
					 * will spawn from the center of the object,
					 *
					 * This will provide the correct distance from the center
					 * when the object is rotated from its center.
					 *
					 * With this distance you may add this to your objects x and y center,
					 * and this will be where your particle will spawn
					 */
					double barrelW  = (zombie[i].bulletW * cos(radians) ) - (zombie[i].bulletH * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
					double barrelH  = (zombie[i].bulletW * sin(radians) ) + (zombie[i].bulletH * cos(radians) );

					int wDifference = zombie[i].realw - zombie[i].w;
					int hDifference = zombie[i].realh - zombie[i].h;

					double barrelX = zombie[i].x+zombie[i].realw/2-wDifference/2 - particleW/2 + barrelW;
					double barrelY = zombie[i].y+zombie[i].realh/2-hDifference/2 - particleH/2 + barrelH;

					if (zombie[i].distance <= 900) {
						// TODO 1-24-2022 [ ] - make this an actual variable for this class
						float atkSped = 5.25;
						zombie[i].timer += atkSped;



						// TODO 1-24-2022 [ ] - create a set of actualy animation attacks with timers, and have them reload


						if (zombie[i].timer > 60){
							//zombie[i].vX = Cos * 11 - rand() % 8 + 5;
							//zombie[i].vY = Sin * 11 - rand() % 8 + 5;
							// restart shoo ttimer
							zombie[i].timer = 0;
							// play shoot sound effect
							Mix_PlayChannel(3, sLazer, 0);
							// spawn particle
							p_dummy.spawnParticleAngle(particle, zombie[i].tag, 1,
									barrelX, barrelY,
									particleW, particleH,
									zombie[i].angle, 20,
								   5,
								   {255, 100,100}, 1,
								   0.0, 0.0,
								   255, 0,
								   100, 1,
								   false, 0);
						}
					}
				}

				// Big Boss
				if (zombie[i].type == 1) {
					double particleW = 30;														// particle collision size
					double particleH = 30;

					/* If the object is at a rotation of 0.0 (facing right),
					 * then this will be the distance the particle
					 * will spawn from the center of the object,
					 *
					 * This will provide the correct distance from the center
					 * when the object is rotated from its center.
					 *
					 * With this distance you may add this to your objects x and y center,
					 * and this will be where your particle will spawn
					 */
					double barrelW  = (zombie[i].bulletW * cos(radians) ) - (zombie[i].bulletH * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
					double barrelH  = (zombie[i].bulletW * sin(radians) ) + (zombie[i].bulletH * cos(radians) );

					int wDifference = zombie[i].realw - zombie[i].w;
					int hDifference = zombie[i].realh - zombie[i].h;

					double barrelX = zombie[i].x+zombie[i].realw/2-wDifference/2 - particleW/2 + barrelW;
					double barrelY = zombie[i].y+zombie[i].realh/2-hDifference/2 - particleH/2 + barrelH;

					if (zombie[i].distance <= 500) {
						if (!zombie[i].attacking) {
							zombie[i].attackLength = 180;
							zombie[i].attacking = true;
						}
					}

					// If Zombie is attacking, attack only for a certain time
					if (zombie[i].attacking) {
						zombie[i].attackLength -= 0.5;
						if (zombie[i].attackLength <= 0) {
							zombie[i].attacking = false;
						}
						// Zombie shooting/attacking
						zombie[i].timer += 5;
						if (zombie[i].timer > 60){
							//zombie[i].vX = Cos * 11 - rand() % 8 + 5;
							//zombie[i].vY = Sin * 11 - rand() % 8 + 5;
							// restart shoo ttimer
						//	zombie[i].timer = 0;
							// play shoot sound effect
							Mix_PlayChannel(3, sLazer, 0);
							// spawn particle
							p_dummy.spawnParticleAngle(particle, zombie[i].tag, 0,
									barrelX,
									barrelY,
									particleW, particleH,
									zombie[i].angle, 9,
								   5,
								   {255, 100,100}, 1,
								   0.0, 0.0,
								   255, 0,
								   100, 1,
								   false, 0);
							p_dummy.spawnParticleAngle(particle, zombie[i].tag, 0,
									barrelX,
									barrelY,
									particleW, particleH,
									zombie[i].angle-15, 9,
								   5,
								   {255, 100,100}, 1,
								   0.0, 0.0,
								   255, 0,
								   100, 1,
								   false, 0);
							p_dummy.spawnParticleAngle(particle, zombie[i].tag, 0,
									barrelX,
									barrelY,
									particleW, particleH,
									zombie[i].angle+15, 9,
								   5,
								   {255, 100,100}, 1,
								   0.0, 0.0,
								   255, 0,
								   100, 1,
								   false, 0);
						}
					}
				}

			//-----------------------------------------------------------------------------------//

			/* NOTE: we create a different constant variable
			 * for the Zombie for it to follow the Player.
			 * We create another variable specifically for knocking back the Zombie.
			 */
			// zombie movement
			if (!zombie[i].attacking) {
				zombie[i].x += constVX;
				zombie[i].y += constVY;
			}

			// zombie movement
			zombie[i].x += zombie[i].vX;
			zombie[i].y += zombie[i].vY;

			// velocity decay
			zombie[i].vX = zombie[i].vX - zombie[i].vX * 0.08;
			zombie[i].vY = zombie[i].vY - zombie[i].vY * 0.08;

			// zombie screen check
			if (zombie[i].x + zombie[i].w >= camx && zombie[i].x <= camx + screenWidth &&
				zombie[i].y + zombie[i].h >= camy && zombie[i].y <= camy + screenHeight) {
				zombie[i].onScreen = true;
			}else{
				zombie[i].onScreen = false;
			}

			// zombie death
			if (zombie[i].health <= 0)
			{
				player.score += 20;
				// spawn blood particle effect
				/*for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
					int rands = rand() % 9 + 2;
					float newX = zombie[i].x+zombie[i].w/2;
					float newY = zombie[i].y+zombie[i].h/2;
					p_dummy.spawnParticleAngle(particle, 2,
										newX-rands/2,
										newY-rands/2,
									   rands, rands,
									   h, randDouble(2.1, 7.1),
									   0.0,
									   {255, 255, 0, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 7,
									   rand() % 50 + 90, 0,
									   true, 0.11,
									   true, randDouble(0.07, 0.69));
				}*/
				zombie[i].alive = false;
				count--;
			}

			// zombie and Map collision
			/*if (zombie[i].x+zombie[i].w < map.x) {
				zombie[i].x = map.x+map.w;
			}
			if (zombie[i].x > map.x+map.w) {
				zombie[i].x = map.x-zombie[i].w;
			}
			if (zombie[i].y+zombie[i].h < map.y) {
				zombie[i].y = map.y+map.h;
			}
			if (zombie[i].y > map.y+map.h) {
				zombie[i].y = map.y-zombie[i].h;
			}*/

			// Camera level bounds
			if( zombie[i].x < 0 ){
				zombie[i].x = 0;
			}
			if( zombie[i].y < 0 ){
				zombie[i].y = 0;
			}
			if( zombie[i].x+zombie[i].w > map.w ){
				zombie[i].x = map.w-zombie[i].w;
			}
			if( zombie[i].y+zombie[i].h > map.h ){
				zombie[i].y = map.h-zombie[i].h ;
			}

			// zombie circle collision check with other zombies
			for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (zombie[j].alive) {
						float bmx = zombie[j].x+zombie[j].w/2;
						float bmy = zombie[j].y+zombie[j].h/2;
						float bmx2 = zombie[i].x+zombie[i].w/2;
						float bmy2 = zombie[i].y+zombie[i].h/2;
						float angle = atan2(bmy - bmy2,bmx - bmx2);
						angle = angle * (180 / 3.1416);
						if (angle < 0) {
							angle = 360 - (-angle);
						}
						float radians = (3.1415926536/180)*(angle);
						float Cos = floor(cos(radians)*10+0.5)/10;
						float Sin = floor(sin(radians)*10+0.5)/10;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						if (distance <= 1) {
							distance = 1;
						}
						if (distance < zombie[i].w/2 + zombie[j].w/2) {
							zombie[i].x -= 1 * Cos;
							zombie[i].y -= 1 * Sin;
						}
					}
				}
			}
		}
	}
}

//Render zombie
void Zombie::render(SDL_Renderer* gRenderer, Zombie zombie[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (zombie[i].alive) {
			int wDifference = zombie[i].realw - zombie[i].w;
			int hDifference = zombie[i].realh - zombie[i].h;
			int newX = zombie[i].x-wDifference/2;					// player starting position
			int newY = zombie[i].y-hDifference/2;
			if (zombie[i].type == 0) {
				gTexture.render(gRenderer, newX - camx, newY - camy,
						zombie[i].realw, zombie[i].realh, NULL, zombie[i].angle);
			}
			if (zombie[i].type == 1) {
				gBoss.render(gRenderer, newX - camx, newY - camy,
						zombie[i].realw, zombie[i].realh, NULL, zombie[i].angle);
			}
		}
	}
}



