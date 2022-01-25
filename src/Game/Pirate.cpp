/*
 * Pirate.cpp
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#include "Pirate.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../LTexture.h"
#include "../Particless.h"

// TODO (1-24-2022) [ ] - create ability to reload for Pirates


// Clear all pirate
void Pirate::clear(Pirate pirate[]){
	for (int i = 0; i < max; i++) {
		pirate[i].alive = false;
	}
	count = 0;
}

void Pirate::Init(Pirate pirate[]){
	count = 0;
	asteroidsClear	= true;
	for (int i = 0; i < max; i++) {
		pirate[i].x 				= 0;
		pirate[i].y 				= 0;
		pirate[i].w 				= 0;
		pirate[i].h 				= 0;
		pirate[i].vX 				= 0.0;
		pirate[i].vY 				= 0.0;
		pirate[i].speed 			= 0.0;
		pirate[i].angle 			= 0.0;
		pirate[i].health 			= 100;
		pirate[i].damage 			= 5;
		pirate[i].distance 			= 1;
		pirate[i].alive 			= false;
		pirate[i].collision 		= false;
		pirate[i].onScreen 			= false;
		pirate[i].attacking 		= false;
		pirate[i].attackLength 		= 0;
		pirate[i].type 				= 0;
		pirate[i].timer 			= 0;
		pirate[i].reloading 		= false;
	}
}

void Pirate::Load(SDL_Renderer* gRenderer){
	gTexture.loadFromFile(gRenderer, "resource/gfx/player_clone.png");
	gBoss.loadFromFile(gRenderer, "resource/gfx/boss.png");
}

void Pirate::Free(){
	gTexture.free();
	gBoss.free();
}



// Create an pirate
void Pirate::spawn(Pirate pirate[], float x, float y,
		           float w, float h, int realw, int realh,
				   float angle, float speed,
				   int type, double health,
				   int distanceHeadIsFromCenterOfImage,
				   int bulletW, int bulletH) {
	for (int i = 0; i < max; i++)
	{
		if (!pirate[i].alive)
		{
			pirate[i].tag 				= "enemy";
			pirate[i].x 				= x;
			pirate[i].y 				= y;
			pirate[i].w 				= w;
			pirate[i].h 				= h;
			pirate[i].realw 			= realw;
			pirate[i].realh 			= realh;
			pirate[i].radius 			= w/2;
			pirate[i].speed 			= speed;
			//pirate[i].vX 				= cos( (3.14159265/180)*(angle) );
			//pirate[i].vY 				= sin( (3.14159265/180)*(angle) );
			pirate[i].vX 				= 0.0;
			pirate[i].vY 				= 0.0;
			pirate[i].angle 			= 0.0;
			pirate[i].bulletW 			= bulletW;
			pirate[i].bulletH 			= bulletH;
			//------------------------------------------------------------------------//
			/* initial centering of pirate's circle collision shape */
			float radians 				= (3.1415926536/180)*(0.0);
			float Cos 					= floor(cos(radians)*10+0.5)/10;
			float Sin 					= floor(sin(radians)*10+0.5)/10;
			pirate[i].distanceHeadIsFromCenterOfImage	= distanceHeadIsFromCenterOfImage;
			int newW					= distanceHeadIsFromCenterOfImage * (-Cos);
			int newH 					= distanceHeadIsFromCenterOfImage * (-Sin);
			pirate[i].xCenter 				= x+w/2 + newW - pirate[i].radius;
			pirate[i].yCenter 				= y+h/2 + newH - pirate[i].radius;
			//------------------------------------------------------------------------//
			pirate[i].health			= health;
			pirate[i].damage			= 5;
			pirate[i].type 				= type;
			pirate[i].distance 			= 1;
			pirate[i].timer 			= 0;
			pirate[i].attackLength		= 180;
			pirate[i].attacking 		= false;
			pirate[i].collision 		= false;
			pirate[i].onScreen 			= false;

			// Determine ammo and magazine size of pirate
			// based on type of pirate being spawn
			if (type == 0) {
				pirate[i].atkSpeed = 2.75;
				pirate[i].ammo = 9;
				pirate[i].magSize = 9;
				pirate[i].reloadSpeed = 4.25;
				pirate[i].reloadTimer = 0.0;
				pirate[i].atkRange = 700;
			}
			if (type == 1) {
				pirate[i].atkSpeed = 9.65;
				pirate[i].ammo = 30;
				pirate[i].magSize = 30;
				pirate[i].reloadSpeed = 9.25;
				pirate[i].reloadTimer = 0.0;
				pirate[i].atkRange = 825;
			}
			pirate[i].atkSpeedTimer = 0.0;
			pirate[i].shooting 		= false;
			pirate[i].reloading 	= false;

			//-------------------------------------------------------------------------//
			pirate[i].alive 			= true;
			count++;
			break;
		}
	}
}

// TODO [ ] - do something about having a mix chunk in this class, maybe have all manipulator code inside of PlayerGame class
// TODO [ ] - inside Player class, after player dies and comes back to life, pirates position is persistent, fix this.
//			- Following up on the last sentence, the players animations (rustling) do not move as well

// TODO [x] - for the enemy, also render a circle to see where the enemy is, we will be doing
//			- circle collision for this game

// TODO [x] - get circle collision boxes for pirate, and also change position of particle that comes out of player
// 			- and make it be on the green box, not the white (if possible)

// TODO [ ] - instead of the enemy turning their body instantly towards the player,
// 			- have them do a turn of some sort to face the player. This will probably
// 			- require the use of creating a distance formula in the angle (0-360)

// Update pirate
void Pirate::update(Pirate pirate[], Particle particle[], Particle &p_dummy,
		Map &map, Players &player,
		Mix_Chunk* sLazer, Mix_Chunk* sPistolReload,
		int camx, int camy) {
	for (int i = 0; i < max; i++)
	{
		if (pirate[i].alive)
		{
			// pirate target
			float bmx2 = pirate[i].x+pirate[i].w/2;
			float bmy2 = pirate[i].y+pirate[i].h/2;
			float bmx  = player.x2+player.radius;
			float bmy  = player.y2+player.radius;
			// pirate distance from target
			pirate[i].distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
			if (pirate[i].distance <= 1) {
				pirate[i].distance = 1;
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////
			//------------------------- This code makes the pirate "walk" to the player ---------------------//

			// pirate moves if target is less than 768 pixels and greater than 384 pixels away
			float constVX = 0.0;
			float constVY = 0.0;
			// shootPlayerDistance = 384;

			// TODO [ ] BUG, if the pirate spawns within the player's attack range then it will not face the player
			// 				 it will only shoot

			// If the player's distance is > 400 pixels and < 400 pixels
			if (pirate[i].distance > 400 && pirate[i].distance < pirate[i].atkRange) {
				constVX = pirate[i].speed * (bmx - bmx2) / pirate[i].distance;
				constVY = pirate[i].speed * (bmy - bmy2) / pirate[i].distance;
				//pirate[i].x += pirate[i].speed * (bmx - bmx2) / pirate[i].distance;
				//pirate[i].y += pirate[i].speed * (bmy - bmy2) / pirate[i].distance;

				pirate[i].angle = atan2(bmy - bmy2,bmx - bmx2);
				pirate[i].angle = pirate[i].angle * (180 / 3.1416);

				if (pirate[i].angle < 0) {
					pirate[i].angle = 360 - (-pirate[i].angle);
				}
			}

			//------------------------- This code makes the pirate "walk" to the player ---------------------//
			///////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////
			//------------------------- This code makes the pirate "look" at the player ---------------------//
			if (pirate[i].distance < pirate[i].atkRange) {

				float radians = (3.1415926536/180)*(pirate[i].angle);
				pirate[i].angle = atan2(bmy - bmy2,bmx - bmx2);
				pirate[i].angle = pirate[i].angle * (180 / 3.1416);
				if (pirate[i].angle < 0) {
					pirate[i].angle = 360 - (-pirate[i].angle);
				}

				// calculate some things for shooting
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;
				int newW = pirate[i].distanceHeadIsFromCenterOfImage * (-Cos);
				int newH = pirate[i].distanceHeadIsFromCenterOfImage * (-Sin);

				// Set pirates angle in relation to the player
				// In other words, have the pirate face the player
				pirate[i].xCenter = pirate[i].x+pirate[i].w/2 + newW - pirate[i].radius;
				pirate[i].yCenter = pirate[i].y+pirate[i].h/2 + newH - pirate[i].radius;
			}

			//------------------------- This code makes the pirate "look" at the player ---------------------//
			///////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////

				// Clone
				if (pirate[i].type == 0) {
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
					float radians = (3.1415926536/180)*(pirate[i].angle);
					double barrelW  = (pirate[i].bulletW * cos(radians) ) - (pirate[i].bulletH * sin(radians) );	// add this to center of pirate (this will give us the guns barrel position)
					double barrelH  = (pirate[i].bulletW * sin(radians) ) + (pirate[i].bulletH * cos(radians) );

					int wDifference = pirate[i].realw - pirate[i].w;
					int hDifference = pirate[i].realh - pirate[i].h;

					double barrelX = pirate[i].x+pirate[i].realw/2-wDifference/2 - particleW/2 + barrelW;
					double barrelY = pirate[i].y+pirate[i].realh/2-hDifference/2 - particleH/2 + barrelH;

					// If not reloading
					if (!pirate[i].reloading && !pirate[i].shooting) {

						// If distance from player is within attack range
						if (pirate[i].distance < pirate[i].atkRange) {

							// Start shooting animation
							pirate[i].shooting = true;

							/*
							// TODO 1-24-2022 [ ] - make this an actual variable for this class
							float atkSped = 5.25;
							pirate[i].timer += atkSped;
							// TODO 1-24-2022 [ ] - create a set of actualy animation attacks with timers, and have them reload
							if (pirate[i].timer > 60){
								//pirate[i].vX = Cos * 11 - rand() % 8 + 5;
								//pirate[i].vY = Sin * 11 - rand() % 8 + 5;
								// restart shoo ttimer
								pirate[i].timer = 0;
								// play shoot sound effect
								Mix_PlayChannel(3, sLazer, 0);
								// spawn particle
								p_dummy.spawnParticleAngle(particle, pirate[i].tag, 1,
										barrelX, barrelY,
										particleW, particleH,
										pirate[i].angle, 20,
									   5,
									   {255, 100,100}, 1,
									   0.0, 0.0,
									   255, 0,
									   100, 1,
									   false, 0);
							}*/
						}
					}

					// Shooting animation
					if (pirate[i].shooting) {

						// If pirate has ammo
						if (pirate[i].ammo > 0) {

							// Increase reload timer with reload speed
							pirate[i].atkSpeedTimer += pirate[i].atkSpeed;

							// After 1 second (or every 60 frames)
							if (pirate[i].atkSpeedTimer > 60) {

								// Play gun shot SFX
								Mix_PlayChannel(3, sLazer, 0);

								// Reset timer
								pirate[i].atkSpeedTimer = 0;

								// Spawn bullet/particle
								p_dummy.spawnParticleAngle(particle, pirate[i].tag, 1,
										barrelX, barrelY,
										particleW, particleH,
										pirate[i].angle, 20,
									   5,
									   {255, 100,100}, 1,
									   0.0, 0.0,
									   255, 0,
									   100, 1,
									   false, 0);

								// Reduce ammo count
								pirate[i].ammo--;
							}
						}

						// Pirate has no more ammo
						else {

							// Stop shooting animation
							pirate[i].shooting = false;

							// Start reloading animation
							pirate[i].reloading = true;
						}
					}

					// Reloading animation
					else if (pirate[i].reloading) {

						// If pirate does not have a full clip yet
						if (pirate[i].ammo < pirate[i].magSize) {

							// Increase reload timer with reload speed
							pirate[i].reloadTimer += pirate[i].reloadSpeed;

							// After 1 second (or every 60 frames)
							if (pirate[i].reloadTimer > 60) {

								// Play reload SFX
								Mix_PlayChannel(7, sPistolReload, 0);

								// Reset timer
								pirate[i].reloadTimer = 0;

								// Increase ammo count for pirate
								pirate[i].ammo++;
							}
						}

						// Pirate has a full clip, stop reload animation
						else {
							pirate[i].reloading = false;
						}

					}
				}

				// Big Boss
				if (pirate[i].type == 1) {
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
					float radians = (3.1415926536/180)*(pirate[i].angle);
					double barrelW  = (pirate[i].bulletW * cos(radians) ) - (pirate[i].bulletH * sin(radians) );	// add this to center of pirate (this will give us the guns barrel position)
					double barrelH  = (pirate[i].bulletW * sin(radians) ) + (pirate[i].bulletH * cos(radians) );

					int wDifference = pirate[i].realw - pirate[i].w;
					int hDifference = pirate[i].realh - pirate[i].h;

					double barrelX = pirate[i].x+pirate[i].realw/2-wDifference/2 - particleW/2 + barrelW;
					double barrelY = pirate[i].y+pirate[i].realh/2-hDifference/2 - particleH/2 + barrelH;

					if (pirate[i].distance <= 500) {
						if (!pirate[i].attacking) {
							pirate[i].attackLength = 180;
							pirate[i].attacking = true;
						}
					}

					// If Pirate is attacking, attack only for a certain time
					if (pirate[i].attacking) {
						pirate[i].attackLength -= 0.5;
						if (pirate[i].attackLength <= 0) {
							pirate[i].attacking = false;
						}
						// Pirate shooting/attacking
						pirate[i].timer += 5;
						if (pirate[i].timer > 60){
							//pirate[i].vX = Cos * 11 - rand() % 8 + 5;
							//pirate[i].vY = Sin * 11 - rand() % 8 + 5;
							// restart shoo ttimer
						//	pirate[i].timer = 0;
							// play shoot sound effect
							Mix_PlayChannel(3, sLazer, 0);
							// spawn particle
							p_dummy.spawnParticleAngle(particle, pirate[i].tag, 0,
									barrelX,
									barrelY,
									particleW, particleH,
									pirate[i].angle, 9,
								   5,
								   {255, 100,100}, 1,
								   0.0, 0.0,
								   255, 0,
								   100, 1,
								   false, 0);
							p_dummy.spawnParticleAngle(particle, pirate[i].tag, 0,
									barrelX,
									barrelY,
									particleW, particleH,
									pirate[i].angle-15, 9,
								   5,
								   {255, 100,100}, 1,
								   0.0, 0.0,
								   255, 0,
								   100, 1,
								   false, 0);
							p_dummy.spawnParticleAngle(particle, pirate[i].tag, 0,
									barrelX,
									barrelY,
									particleW, particleH,
									pirate[i].angle+15, 9,
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
			 * for the Pirate for it to follow the Player.
			 * We create another variable specifically for knocking back the Pirate.
			 */
			// pirate movement
			if (!pirate[i].attacking) {
				pirate[i].x += constVX;
				pirate[i].y += constVY;
			}

			// pirate movement
			pirate[i].x += pirate[i].vX;
			pirate[i].y += pirate[i].vY;

			// velocity decay
			pirate[i].vX = pirate[i].vX - pirate[i].vX * 0.08;
			pirate[i].vY = pirate[i].vY - pirate[i].vY * 0.08;

			// pirate screen check
			if (pirate[i].x + pirate[i].w >= camx && pirate[i].x <= camx + screenWidth &&
				pirate[i].y + pirate[i].h >= camy && pirate[i].y <= camy + screenHeight) {
				pirate[i].onScreen = true;
			}else{
				pirate[i].onScreen = false;
			}

			// pirate death
			if (pirate[i].health <= 0)
			{
				player.score += 20;
				// spawn blood particle effect
				/*for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
					int rands = rand() % 9 + 2;
					float newX = pirate[i].x+pirate[i].w/2;
					float newY = pirate[i].y+pirate[i].h/2;
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
				pirate[i].alive = false;
				count--;
			}

			// pirate and Map collision
			/*if (pirate[i].x+pirate[i].w < map.x) {
				pirate[i].x = map.x+map.w;
			}
			if (pirate[i].x > map.x+map.w) {
				pirate[i].x = map.x-pirate[i].w;
			}
			if (pirate[i].y+pirate[i].h < map.y) {
				pirate[i].y = map.y+map.h;
			}
			if (pirate[i].y > map.y+map.h) {
				pirate[i].y = map.y-pirate[i].h;
			}*/

			// Camera level bounds
			if( pirate[i].x < 0 ){
				pirate[i].x = 0;
			}
			if( pirate[i].y < 0 ){
				pirate[i].y = 0;
			}
			if( pirate[i].x+pirate[i].w > map.w ){
				pirate[i].x = map.w-pirate[i].w;
			}
			if( pirate[i].y+pirate[i].h > map.h ){
				pirate[i].y = map.h-pirate[i].h ;
			}

			// pirate circle collision check with other pirates
			for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (pirate[j].alive) {
						float bmx = pirate[j].x+pirate[j].w/2;
						float bmy = pirate[j].y+pirate[j].h/2;
						float bmx2 = pirate[i].x+pirate[i].w/2;
						float bmy2 = pirate[i].y+pirate[i].h/2;
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
						if (distance < pirate[i].w/2 + pirate[j].w/2) {
							pirate[i].x -= 1 * Cos;
							pirate[i].y -= 1 * Sin;
						}
					}
				}
			}
		}
	}
}

//Render pirate
void Pirate::render(SDL_Renderer* gRenderer, Pirate pirate[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (pirate[i].alive) {
			int wDifference = pirate[i].realw - pirate[i].w;
			int hDifference = pirate[i].realh - pirate[i].h;
			int newX = pirate[i].x-wDifference/2;					// player starting position
			int newY = pirate[i].y-hDifference/2;
			if (pirate[i].type == 0) {
				gTexture.render(gRenderer, newX - camx, newY - camy,
						pirate[i].realw, pirate[i].realh, NULL, pirate[i].angle);
			}
			if (pirate[i].type == 1) {
				gBoss.render(gRenderer, newX - camx, newY - camy,
						pirate[i].realw, pirate[i].realh, NULL, pirate[i].angle);
			}
		}
	}
}



