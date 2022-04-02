/*
 * Players.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

// the "../" in the beginning means that it goes back one folder
//#include "../Engine/Players.h"
#include "Players.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_mouse.h>

//#include "Input.h"


// Renders a circle
int
SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


// Check collision between 2 objects
bool Players::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2){
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

// Load asteroid resources
void Players::loadResources(SDL_Renderer* gRenderer){

	// open joystick i
    joy = SDL_JoystickOpen(1);

    // load textures
	spr_player_left_arm.loadFromFile(gRenderer, 		"resource/gfx/player/player_left_arm.png");
	spr_player_left_bicep.loadFromFile(gRenderer, 		"resource/gfx/player/player_left_bicep.png");
	spr_player_right_arm.loadFromFile(gRenderer, 		"resource/gfx/player/player_right_arm.png");
	spr_player_gun.loadFromFile(gRenderer, 				"resource/gfx/player/player_gun.png");
	spr_player_muzzle_flash.loadFromFile(gRenderer, 	"resource/gfx/player/player_muzzle_flash.png");
	spr_player_body.loadFromFile(gRenderer, 			"resource/gfx/player/player_body.png");
	spr_player_right_shoulder.loadFromFile(gRenderer, 	"resource/gfx/player/player_right_shoulder.png");
	spr_player_left_shoulder.loadFromFile(gRenderer, 	"resource/gfx/player/player_left_shoulder.png");
	spr_player_backpack.loadFromFile(gRenderer, 		"resource/gfx/player/player_backpack.png");
	spr_player_head.loadFromFile(gRenderer, 			"resource/gfx/player/player_head.png");
	gShield.loadFromFile(gRenderer, "img/shield.png");
	for (int i=0; i<7; i++){setClips(rShield[i], i*48, 0, 48, 48);}
}

// Free asteroid resources
void Players::freeResources(){
    SDL_JoystickClose(joy);
	spr_player_left_arm.free();
	spr_player_left_bicep.free();
	spr_player_right_arm.free();
	spr_player_gun.free();
	spr_player_muzzle_flash.free();
	spr_player_body.free();
	spr_player_right_shoulder.free();
	spr_player_left_shoulder.free();
	spr_player_backpack.free();
	spr_player_head.free();
    gShield.free();
}

// Reset high scores
void Players::resetHighScore(){
	// Overwrite "highscores.txt"
	std::ofstream fileS;
	fileS.open("highscores.txt");
	for (int i=0; i<10; i++){
		fileS << "AAA 0" << "\n";
	}
	fileS.close();
}

// Load Player high score data
void Players::loadScore(){
	indx = 0;
	bool getHighest = true;
	std::ifstream fileO("highscores.txt");
	std::string str;
	while (std::getline(fileO, str))
	{
		// Read current line
		std::stringstream iss(str);
		std::string temps[2];
		iss >> temps[0] >> temps[1];

		// Store first line as highscore
		if (getHighest){
			getHighest = false;
			highscore = atoi(temps[1].c_str());
		}

		// Load highscore data
		std::stringstream tempss;
		tempss << temps[0] << " " << temps[1];
		highList[indx] = tempss.str().c_str();
		indx++;
	}

	fileO.close();
	std::cout << "DONE" << std::endl;
}

// Save high score
void Players::saveHighScore() {
	std::ifstream fileO("highscores.txt");
	std::string str;
	std::stringstream tempss;
	std::vector<std::string> t_name;
	std::vector<int> t_score;
	int indx = 0;
	bool replace = true;

	while (getline(fileO, str)){
		// Read current line
		std::stringstream iss(str);

		// Temp string to store Name & Score
		std::string temps[2];

		// Store Name & Score in temp string
		iss >> temps[0] >> temps[1];

		// Now store everything in a vector for checking
		t_name.push_back(temps[0]);
		t_score.push_back( atoi(temps[1].c_str()) );
	}
	fileO.close();


	// If score > saveScore, then insert current score at current index, and remove last index
	for (unsigned int i=0; i<t_name.size(); i++){
		if (score > t_score[i])
		{
			// Insert Player data if we haven't yet
			if (replace)
			{
				replace = false;
				// Insert data at current index
				t_name.insert(t_name.begin()+i, name);
				t_score.insert(t_score.begin()+i, score);
				indexSaved = i;

				// Remove last index
				t_score.pop_back();
				t_name.pop_back();
			}
		}
	}

	// After replacing data
	for (unsigned int i=0; i<t_name.size(); i++){
		tempss << t_name[i] << " " << t_score[i] << std::endl;
	}

	// Overwrite "highscores.txt"
	std::ofstream fileS;
	fileS.open("highscores.txt");
	fileS << tempss.str().c_str();
	fileS.close();

	// Update score display
	loadScore();
}

// Reset game
void Players::reset(float spawnX, float spawnY, std::string newName, bool respawn){

	// Health
	alive 				= true;
	deathScreen 		= false;
	health				= 20;
	healthDecay			= 20;
	maxHealth			= 20;

	// Position
	x 					= spawnX;
	y 					= spawnY;
	vX 					= 0.0;
	vY 					= 0.0;

	// Name
	name				= newName;

	// Stats
	accuracy			= 0.0;
	hits				= 0.0;
	totalShot			= 0.0;

	// Shooting
	delayT 				= 0;
	delay 				= false;
	initialshot 		= false;
	thrust 				= false;
	returned			= false;
	indexSaved 			= -1;

	// Shield
	shieldFrame			= 1;
	shieldTick			= 0;
	shieldT				= 300;
	shield				= true;

	controls 				= 0;
	A 					= false;
	B 					= false;
	X 					= false;
	Y 					= false;
	D_UP 					= false;
	D_DOWN 				= false;
	D_LEFT 				= false;
	D_RIGHT 				= false;
	LB 					= false;
	RB 					= false;
	BACK 					= false;
	START 				= false;
	L3 					= false;
	R3 					= false;
	LAngle 				= 0.0;
	RAngle 				= 0.0;
	zAxisLeft 			= 0.0;
	zAxisRight 			= 0.0;
	leftclick 			= false;
	rightclick 			= false;
	test 				= false;
	trigger 			= false;
	moving				= false;
	renderFlash 		= false;
	tag 				= "player";

	// New variables
	shooting			= false;
	recoilBloomAmount	= 1.0;

	/* Player abilities */
	grenades = 3;
	grenadesMax = 3;
	grenadesCD = 0;
	grenadesCDSpeed = 1;
	grenadesCDMax = 60 * 5;
	grenadeTrigger = false;

	collectedKeys = 0;

	// If we are not respawning, reset everything
	if (!respawn){
		score 			= 0;
		wave 			= 0;
		increment	 	= 1;
		lives 			= 3;
	}
}

// Applies a shield to Player
void Players::applyShield(){
	shieldFrame			= 1;
	shieldTick			= 0;
	shieldT				= 300;
	shield				= true;
}

// Player shoot
void Players::fire(Particle particle[], Particle &p_dummy, int mx, int my,
		 	 	   Mix_Chunk* sLazer, Mix_Chunk* sGrenade, Mix_Chunk* sGrenadePickup,
				   Mix_Chunk* sPistolReload){

	// Determine controls
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
	}else if(controls==2){
		trigger = initialshot;
	}

	// Get shoot trigger
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
		A = false;
	}else if(controls==2){
		trigger = initialshot;
		initialshot = false;
	}

	// Get player angle based on mouse coordinates
	angle = atan2(my - y-h/2,mx - x-w/2);
	angle = angle * (180 / 3.1416);
	//Set player angle max limits
	if (angle < 0) {
		angle = 360 - (-angle);
	}

	// Get radiant from angle
	radians = (3.1415926536/180)*(angle);
	radianCos = floor(cos(radians)*10+0.5)/10;
	radianSin = floor(sin(radians)*10+0.5)/10;

	// get player new center position
	//x2 = (x+w/2 + (41 * (-radianCos)) - radius);
	//y2 = (y+h/2 + (41 * (-radianSin)) - radius);
	int newW = 41 * (-radianCos);
	int newH = 41 * (-radianSin);
	x2 = x+w/2 + newW - radius;
	y2 = y+h/2 + newH - radius;


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
	barrelW  = (bulletW * cos(radians) ) - (bulletH * sin(radians) );	// add this to center of pirate (this will give us the guns barrel position)
	barrelH  = (bulletW * sin(radians) ) + (bulletH * cos(radians) );

	int wDifference = realw - w;
	int hDifference = realh - h;

	// Update x and y coordinates of the player's gun barrel
	barrelX = x+realw/2-wDifference/2 - particleW/2 + barrelW;
	barrelY = y+realh/2-hDifference/2 - particleH/2 + barrelH;

	/*int newmx = mx-particleW/2;
	int newmy = my-particleH/2;
	float distance = sqrt((newmx - barrelX) * (newmx - barrelX)+
						  (newmy - barrelY) * (newmy - barrelY));
	if (distance <= 1) {
		distance = 1;
	}
	float newAngle = atan2(newmy - barrelY,newmx - barrelX);
	newAngle = newAngle * (180 / 3.1416);
	//Set player angle max limits
	if (newAngle < 0) {
		newAngle = 360 - (-newAngle);
	}*/

	///////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------//
	//---------------------- Handle Basic Shooting ----------------------//
	// Shoot particle
	if (trigger){
		if (!reload){
			// If there is more than 0 ammo in magazine
			if (ammo > 0) {
				if (!delay){
					delay= true;
					if (powerup == 1) {

						// Shooting state, on
						shooting = true;

						// recoil of gun and shoulder animation
						recoilX = 11 * radianCos;
						recoilY = 11 * radianSin;

						// count up total shots
						totalShot++;

						// play audio
						Mix_PlayChannel(1, sLazer, 0);

						////////////////////////////////////////////////////////////////////////////////////////////////
						////////////////////////////////////////////////////////////////////////////////////////////////
						//------------------- Determines clockwise or counter-close wise direction -------------------//

						// Random number of 0 or 1
						int randir = rand() % 2;
						int dir;
						// Shoots a little counter-clockwise to cursor
						if (randir == 0) { dir = -1; }
						// Shoots a little clockwise to cursor
						else { dir = 1; }

						//------------------- Determines clockwise or counter-close wise direction -------------------//
						////////////////////////////////////////////////////////////////////////////////////////////////
						////////////////////////////////////////////////////////////////////////////////////////////////

						////////////////////////////////////////////////////////////////////////////////////////////////
						////////////////////////////////////////////////////////////////////////////////////////////////
						//------------------------- Sometimes the bullet will hit the center -------------------------//

						// Determine if the bullet will follow the bloom rules, or sometimes shoot in the center of your cursor
						int followBloom = rand() % 2;
						float angeledRecoil;

						// Start shooting off-center of mouse (after 2 shots fired)
						//if (recoilBloomAmount > 2) {
						if (followBloom == 1) {

							// Regular bloom direction. Option 1.
							int value = recoilBloomAmount + 1;
							float randRecoilSize = rand() % value + 1;
							angeledRecoil = angle + (randRecoilSize * dir);

							// Regular bloom direction. Option 2.
							// ISSUE: as the bloom increases, direction of bullet shoots on the edges of the bloom amount
							//angeledRecoil = angle + (recoilBloomAmount * dir);
						}

						// RNG - Shoot in the center every once in a while
						else {
							angeledRecoil = angle;
						}

						//------------------------- Sometimes the bullet will hit the center -------------------------//
						////////////////////////////////////////////////////////////////////////////////////////////////
						////////////////////////////////////////////////////////////////////////////////////////////////

						// spawn particle
						p_dummy.spawnParticleAngle(particle, tag, 0,
								barrelX,
								barrelY,
								particleW, particleH,
								angeledRecoil, 65,
							   25,
							   {255,255,255}, 1,
							   1, 1,
							   255, 0,
							   100, 2,
							   false, 0);

						// Increase bloom amount every shot
						if (recoilBloomAmount < recoilBloomAmountMax) {
							recoilBloomAmount += 1;
						}

						// muzzle flash
						renderFlash = true;

						// subtract remaining grenades
						ammo--;

						// Check if we ran out of ammo
						if (ammo <= 0) {

							// start reloading ammo
							reload = true;
						}
					}
				}
			}
		}
	}

	// No longer holding down trigger of gun
	else {
		// Shooting state, off
		shooting = false;
	}

	// Reload
	if (reload) {
		reloadTimer += reloadSpeed;
		if (reloadTimer > 60) {
			reloadTimer = 0;
			// play audio
			Mix_PlayChannel(7, sPistolReload, 0);
			// add ammo into clip
			ammo++;
		}
	}
	if (ammo >= 30) {
		ammo = 30;
		reload = false;
	}

	// Shoot delay
	if (delay) {
		delayT += AttackSpeed;
		if (delayT > 60) {
			delay = false;
			delayT = 0;
		}
	}
	//---------------------- Handle Basic Shooting ----------------------//
	//-------------------------------------------------------------------//
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------//
	//---------------------- Handle Grenade Throw -----------------------//

	if (grenadeTrigger) {
		grenadeTrigger = false;
		if (grenades > 0) {
			// play audio
			Mix_PlayChannel(5, sGrenade, 0);
			// spawn particle
			p_dummy.spawnParticleAngle(particle, tag, 3,
					barrelX,
					barrelY,
					24, 31,
				   angle, 25,
				   100,
				   {255, 255,255}, 1,
				   1, 1,
				   255, 0,
				   60*2, 1,
				   false, 0,
				   true, 0.11);
			// subtract remaining grenades
			grenades--;
		}
	}
	// Replenish grenades
	if (grenades < 3) {
		grenadesCD += grenadesCDSpeed;
		if (grenadesCD > grenadesCDMax) {
			grenadesCD = 0;
			// play audio
			Mix_PlayChannel(6, sGrenadePickup, 0);
			// add grenade
			grenades++;
		}
	}

	//---------------------- Handle Grenade Throw -----------------------//
	//-------------------------------------------------------------------//
	///////////////////////////////////////////////////////////////////////



	/*bool trigger = false;
	// Player shoot
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
	}*/

	//if (test){
		//if (!delay) {
			//if (A) {
				//A = false;
				//trigger = false;
				//delay = true;

				// Remove shield
				/*if (shield){
					shield 	= false;
					shieldT 	= 300;
				}*/

				// Shoot particle
				/*if (powerup == "LAZER") {
					totalShot++;
					Mix_PlayChannel(-1, sLazer, 0);

					// TODO [ ] - Create power up drops, and health drops
					// TODO [ ] - Create option for Player to use Game Controller or Keyboard and Mouse
					// TODO [ ] - Create proper title screen with options to setttings etc.
					p_dummy.spawnParticleAngle(particle,
							   x+w/2-4/2+(cos(radians)*9)+(sin(radians)*12),
							   y+h/2-4/2+(sin(radians)*9)-(cos(radians)*12),
							   4, 4,
							   angle, 21,
							   255, 0,
							   100, 2,
							   100, "LAZER",
							   {255, 255,0}, 1,
							   1, 1,
							   false, 0);
					p_dummy.spawnParticleAngle(particle,
							   x+w/2-4/2+(cos(radians)*9)-(sin(radians)*12),
							   y+h/2-4/2+(sin(radians)*9)+(cos(radians)*12),
							   4, 4,
							   angle, 21,
							   255, 0,
							   100, 2,
							   100, "LAZER",
							   {255, 255,0}, 1,
							   1, 1,
							   false, 0);
				}*/
			//}
		/*}
		//Start delay timer after our first shot!
		if (delay) {
			delayT += AttackSpeed;
			if (delayT > 60) {
				delay = false;
				delayT = 0;
			}
		}*/
	//}

	// Handle gun recoil
	if (recoilX < 0) {
		recoilX += 2.5;
	}
	if (recoilX > 0) {
		recoilX -= 2.5;
	}
	if (recoilY < 0) {
		recoilY += 2.5;
	}
	if (recoilY > 0) {
		recoilY -= 2.5;
	}
}

// Controls
void Players::move(Particle particle[], Particle &p_dummy, int mx, int my){

	bool trigger = false;

	// Get Angle
	if (controls==0){
		trigger = thrust;
	}else if(controls==1){
		angle = LAngle;
		trigger = RB;
	}else if(controls==2){
		trigger = thrust;
	}

	//Accelerate Player
	/*if (trigger){
		vX += cos(radians) * 0.05;
		vY += sin(radians) * 0.05;

		// Thrust effect
		thrustT += 1;
		if (thrustT > 2){
			thrustT = 0;
			float x1, x2, y1, y2, anglet;
			int rands, randd, randdd;
			//SDL_Color tempColor = { rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1};
			SDL_Color tempColor = { 255, 255, 255 };
			x1 				= x+w/2,
			y1 				= y+h/2;
			x2 				= x+w/2 + cos(radians),
			y2 				= y+h/2 + sin(radians);
			anglet 			= atan2(y1 - y2, x1 - x2);
			anglet 			= anglet * (180 / 3.1416);
			rands 			= rand() % 3 + 6;
			randd			= rand() % 2 + 1;
			if (randd==1) {randdd = 1;} else {randdd = -1;}
			p_dummy.spawnParticleAngle(particle, 2,
							   x+w/2-rands/2, y+h/2-rands/2,
							   rands, rands,
							   anglet, rand() % 2 + 2,
							   0.0,
							   tempColor, 1,
							   rand() % 4 + 5, randdd,
							   rand() % 150 + 50, 4,
							   100, 0,
							   false, 0);
		}
	}else{
	    // Use Stokes' law to apply drag to the ship
		vX = vX - vX * 0.001;
		vY = vY - vY * 0.001;
	}*/

	if (moveleft) {
		vX -= 3.0;
		moving = true;
	}else if (moveright) {
		vX += 3.0;
		moving = true;
	}
	if (moveup) {
		vY -= 3.0;
		moving = true;
	}else if (movedown) {
		vY += 3.0;
		moving = true;
	}

	// Rustle timer
	// player left and right swaying
	if (moving) {
		rustleW += 1 * rustleDirX;
		rustleH += 1 * rustleDirY;
	}
	if (rustleW > 4) {
		rustleDirX = rustleDirX * -1;
	}
	if (rustleW < -4) {
		rustleDirX = rustleDirX * -1;
	}
	if (rustleH > 4) {
		rustleDirY = rustleDirY * -1;
	}
	if (rustleH < -4) {
		rustleDirY = rustleDirY * -1;
	}

	//Add player velocity
	x += vX;
	y += vY;

	vX = vX - vX * 0.08;
	vY = vY - vY * 0.08;

	// Velocity limit
	if (vX > 6) {
		vX = 6;
	}
	if (vX < -6) {
		vX = -6;
	}
	if (vY > 6) {
		vY = 6;
	}
	if (vY < -6) {
		vY = -6;
	}

	// Player not moving
	if (!moveleft && !moveright && !moveup && !movedown) {
		moving = false;
	}
}

// Update Player
void Players::update(Map &map,
					Asteroid asteroid[], Asteroid &a_dummy,
					Enemy enemy[], Enemy &e_dummy,
					Particle particle[], Particle &p_dummy,
					int mx, int my, int camx, int camy,
					float spawnX, float spawnY,
					LWindow gWindow, SDL_Renderer* gRenderer,
					LTexture gText, TTF_Font *gFont13, SDL_Color color,
					Mix_Chunk *sAtariBoom, Mix_Chunk* sLazer, Mix_Chunk* sGrenade,
					Mix_Chunk* sGrenadePickup, Mix_Chunk* sPistolReload)
{
	// Reset upon leaving pause menu
	if (returned){
		returned 		= false;
		leftclick 	= false;
		initialshot 	= false;
		thrust		= false;
		A				= false;
		RB			= false;
	}

	// Player alive
	if (alive)
	{
		// Player move
		move(particle, p_dummy, mx+camx, my+camy);

		// Player shoot
		fire(particle, p_dummy, mx+camx, my+camy, sLazer, sGrenade, sGrenadePickup, sPistolReload);

		// Player shield
		if (shield){
			shieldT-=1;
			shieldTick += 5;
			if (shieldTick>15){
				shieldTick = 0;
				shieldFrame += 1;
			}
			if (shieldFrame>7){
				shieldFrame = 1;
				shieldTick = 0;
			}
			if (shieldT<=0){
				shieldT= 300;
				shield = false;
			}
		}

		// Check high score MAX
		if (score > 999999999){
			score = 999999999;
		}

		// Player death
		if (health <=0)
		{
			//Spawn explosion after asteroid death
			// spawn blood particle effect
			for (double i=0.0; i< 360.0; i+=rand() % 10 + 10){
				int rands = rand() % 9 + 2;
				float newX = x+w/2;
				float newY = y+h/2;
				p_dummy.spawnParticleAngle(particle, tag, 2,
									newX-rands/2,
									newY-rands/2,
								   rands, rands,
								   i, randDouble(2.1, 5.1),
								   0.0,
								   {255, 0, 0, 255}, 1,
								   1, 1,
								   rand() % 100 + 150, rand() % 2 + 5,
								   rand() % 50 + 90, 0,
								   true, 0.11);
			}

			// Take away lives
			lives-= 1;

			// Reset Player
			reset(spawnX, spawnY, name, true);

			// Player ran out of lives, que Death Screen
			if (lives<=0){
				SDL_ShowCursor(true);

				// Reset some accumulated stuff
				e_dummy.ENEMY_SPAWN = 1;
				e_dummy.mileScore	= 0;
				e_dummy.SPAWN_ENEMY	= false;

				// Set variables
				shieldT 	= 300;
				shield 		= true;
				alive 		= false;
				deathScreen = true;

				// SAVE HIGH SCORE
				saveHighScore();
			}
		}

		// Update Player score and Wave
		if (score > highscore){
			highscore = score;
		}
		/*
		if (wave > HW){
			HW 	= wave;
		}*/

		// Player boundaries
		if (x+w < map.x) {
			x = map.x+map.w-w;
		}
		if (x > map.x+map.w) {
			x = map.x-w;
		}
		if (y+h < map.y) {
			y = map.y+map.h-h;
		}
		if (y > map.y+map.h) {
			y = map.y-h;
		}
	}else{
		// High-Score moving
		travel += 0.05 * dir;
		if (travel > 10){
			dir = -1;
		}
		if (travel < -10){
			dir = 1;
		}

		// Continue YES or NO Screen
		if (deathScreen)
		{
			SDL_ShowCursor(true);
			// Set button position
			continueButton[0] = {0 + screenWidth/2 -96/2-100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[1] = {0 + screenWidth/2 -96/2+100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {0 + screenWidth/2 -256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position  = 0+travel;
			position2 = continueButton[1].x+continueButton[1].w-gText.getWidth()+travel;

			// Left click
			if (leftclick)
			{
				// Option: Yes, reset everything
				if (checkCollision(mx, my, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					leftclick			= false;

					// Reset Player
					std::string newName;
					newName="AAA";
					/////////input.getInput(gameLoop, quit, newName, gWindow, gRenderer);
					reset(spawnX, spawnY, newName, false);

					// Clear Asteroids & Enemies
					a_dummy.clear(asteroid);
					e_dummy.clear(enemy);
					SDL_ShowCursor(false);
				}

				// Option: No, go to Main Menu
				if (checkCollision(mx, my, 1, 1, continueButton[1].x, continueButton[1].y, continueButton[1].w, continueButton[1].h))
				{
					score = 0;
					leftclick  = false;
					deathScreen = false;
				}

				// Option: Reset high scores
				if (checkCollision(mx, my, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					resetHighScore();

					// Load again
					loadScore();
				}
			}
		// Menu Screen
		}else{
			SDL_ShowCursor(true);
			// Set button position
			continueButton[0] = {screenWidth/2-96/2, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {screenWidth/2-256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position 	= continueButton[0].w/2-gText.getWidth()/2-100+travel;
			position2 = continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2+100+travel;

			// Left click
			if (leftclick)
			{
				// Option: Play
				if (checkCollision(mx, my, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					// Reset Player
					std::string newName;
					newName="AAA";
					///////////input.getInput(gameLoop, quit, newName, gWindow, gRenderer);
					reset(spawnX, spawnY, newName, false);

					// Clear Asteroids & Enemies
					a_dummy.clear(asteroid);
					e_dummy.clear(enemy);
					SDL_ShowCursor(false);
				}

				// Option: Reset high scores
				if (checkCollision(mx, my, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					resetHighScore();

					// Load again
					loadScore();
				}
			}
		}
	}

	// Recoil Bloom naturally goes down 8% each frame to 0 if not shooting
	if (!shooting && recoilBloomAmount > 0) {
		recoilBloomAmount -= 1;
		if (recoilBloomAmount < 0) { recoilBloomAmount=0; }
	}
}

// Render Player
void Players::render(int camx, int camy, LWindow gWindow, SDL_Renderer* gRenderer, int &PARTICLES) {
	// If alive
	if (alive){

		/* Get difference of real size and collision size,
		 * with this you can center the player by subtracting
		 * this with the player's position
		 */
		int wDifference = realw - w;				// get difference between player's collision size and actual size in pixels
		int hDifference = realh - h;
		int newX = x-wDifference/2;					// player starting position
		int newY = y-hDifference/2;
		int newRustleW = rustleW * radianCos;		// make swaying motions while moving
		int newRustleH = rustleH * radianSin;

		// left arm
		spr_player_left_arm.render(gRenderer, newX-recoilX+newRustleW-camx, newY-recoilY+newRustleH-camy, realw, realh, NULL, angle);

		// left arm biceps
		spr_player_left_bicep.render(gRenderer, newX-recoilX+newRustleW-camx, newY-recoilY+newRustleH-camy, realw, realh, NULL, angle);

		// right arm
		spr_player_right_arm.render(gRenderer, newX-camx, newY-recoilY-camy, realw, realh, NULL, angle);

		// gun
		spr_player_gun.render(gRenderer, newX-recoilX-camx, newY-recoilY-camy, realw, realh, NULL, angle);

		// muzzle flash
		if (renderFlash) {
			renderFlash = false;
			double barrelW  = (((bulletW+45) * cos(radians) ) - (bulletH * sin(radians) )) - 120/2;
			double barrelH  = (((bulletW+45) * sin(radians) ) + (bulletH * cos(radians) )) - 120/2;
			// muzzle flash
			spr_player_muzzle_flash.setBlendMode(SDL_BLENDMODE_ADD);
			spr_player_muzzle_flash.render(gRenderer, x+w/2 + barrelW-camx, y+h/2 + barrelH-camy, 120, 120, NULL, angle);
		}

		// body
		spr_player_body.render(gRenderer, newX-camx, newY-camy, realw, realh, NULL, angle);

		// left shoulder
		spr_player_left_shoulder.render(gRenderer, newX-recoilX-newRustleW-camx, newY-recoilY-newRustleH-camy, realw, realh, NULL, angle);

		// right shoulder
		spr_player_right_shoulder.render(gRenderer, newX-camx, newY-camy, realw, realh, NULL, angle);

		// backpack
		spr_player_backpack.render(gRenderer, newX-camx, newY-camy, realw, realh, NULL, angle);

		// head
		spr_player_head.render(gRenderer, newX-camx, newY-camy, realw, realh, NULL, angle);


		/*SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
		SDL_RenderDrawLine(gRenderer, x+w/2 + barrelW-camx,
									  y+h/2 + barrelH-camy,
									  mx, my);*/

		// Player sight to mouse
		//SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		//SDL_RenderDrawLine(gRenderer, x+w/2-camx, y+h/2-camy, mx, my);

		// Render Shield
		if (shield){

			gShield.render(gRenderer, x+w/2 - 256/2 - camx, y+h/2 - 256/2 - camy, 256, 256, &rShield[shieldFrame]);

			/*SDL_Rect tempRect = {X-camx, Y-16-camy, (w*shieldT)/300, 3};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);*/
		}

		// Render number of lives left
		/*for (int i=0; i<lives; i++){
			//gPlayer.render(gRenderer, screenWidth-32-i*16, 90, 16, 16);
			spr_player_head.render(gRenderer, screenWidth/2+i*16, 72, 16, 16);
		}*/

	}else{

	}
}

void Players::RenderUI(SDL_Renderer *gRenderer,
		SDL_Color color,
		LTexture gText,
		TTF_Font *gFont13, TTF_Font *gFont26,
		int mx, int my,
		float camx, float camy,
		int pirateCount) {

	gText.setAlpha(255);
	// If player alive
	if (this->alive) {
		const float yOffsetBar = 30;
		const float barHeight = 20;
		const float barWidth = this->w*1.25;
		float uiX = this->x + this->w/2 - barWidth/2;
		float uiY = this->y - barHeight - yOffsetBar;

		// Health Decay bar on Mobes
		{
			// Health Decay bar, bg
			RenderFillRect(gRenderer, uiX-camx, uiY-camy, (barWidth*this->maxHealth)/this->maxHealth, barHeight, {0, 0, 0} );

			// Render Decay health
			RenderFillRect(gRenderer, uiX-camx, uiY-camy, (barWidth*this->healthDecay)/this->maxHealth, barHeight, {30, 60, 30} );
		}

		// Health bar on Mobes
		{
			// Render health
			RenderFillRect(gRenderer, uiX-camx, uiY-camy, (barWidth*this->health)/this->maxHealth, barHeight, {30, 200, 30} );
		}

		// Render cross-hair of player
		double wedth = 21 * radianSin;
		double hedth = 19 * radianCos;
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(gRenderer, mx-16-wedth, my+hedth, mx+16-wedth, my+hedth);
		SDL_RenderDrawLine(gRenderer, mx-wedth, my-16+hedth, mx-wedth, my+16+hedth);

		// Draw a circle bloom cross-hair
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderDrawCircle(gRenderer, mx-wedth, my+hedth, recoilBloomAmount * 5);
	}// If player alive

	// If deathscreen
	if (deathScreen) {

		// Continue YES or NO Screen
		if (deathScreen)
		{
			// Render Text
			gText.loadFromRenderedText(gRenderer, "You have died. Continue?", color, gFont26);
			gText.render(gRenderer, screenWidth/2-gText.getWidth()/2, screenHeight/2-gText.getHeight()/2-50, gText.getWidth(), gText.getHeight());

			// Render buttons: Yes
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[0]);

			// Render buttons: No
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[1]);

			// Render buttons: ResetHighScore
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[2]);

			// Render button texts: Yes or No
			gText.loadFromRenderedText(gRenderer, "Yes", color, gFont26);
			gText.render(gRenderer,  continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
									 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());

			gText.loadFromRenderedText(gRenderer, "No", color, gFont26);
			gText.render(gRenderer,  continueButton[1].x+continueButton[1].w/2-gText.getWidth()/2,
									 continueButton[1].y+continueButton[1].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());

			// Render Text
			gText.loadFromRenderedText(gRenderer, "Reset High Scores", color, gFont26);
			gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
									 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());
		// Player Menu screen
		}else{

			// Render buttons: Play
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[0]);

			// Render buttons: ResetHighScore
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[2]);

			// Render Text
			gText.loadFromRenderedText(gRenderer, "PLAY", color, gFont26);
			gText.render(gRenderer, continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
									 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());

			// Render Text
			gText.loadFromRenderedText(gRenderer, "Reset High Scores", color, gFont26);
			gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
									 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());
		}

		// Render High Score text
		for (int i=0; i<10; i++){
			std::stringstream tempString(highList[i].c_str());
			std::string line;
			while (getline(tempString, line)) {
				std::stringstream iss(line);
				std::string temps[2];
				iss >> temps[0] >> temps[1];

				// Show Player where they are ranked
				if (indexSaved==i){
					gText.loadFromRenderedText(gRenderer, temps[0].c_str(), {244,144,20}, gFont13);
					gText.setAlpha(255-i*10);
					gText.render(gRenderer, continueButton[0].x+position,
							continueButton[0].y+continueButton[0].h+20+i*14,
							gText.getWidth(), gText.getHeight());
				}else{
					gText.loadFromRenderedText(gRenderer, temps[0].c_str(), color, gFont13);
					gText.setAlpha(255-i*10);
					gText.render(gRenderer, continueButton[0].x+position,
							continueButton[0].y+continueButton[0].h+20+i*14,
							gText.getWidth(), gText.getHeight());
				}

				gText.loadFromRenderedText(gRenderer, temps[1].c_str(), color, gFont13);
				gText.setAlpha(255-i*10);
				gText.render(gRenderer, position2,
						continueButton[1].y+continueButton[1].h+20+i*14,
						gText.getWidth(), gText.getHeight());
			}
		}
	} // end death screen check

	// Render highscore, score and wave text, and enemies left text
	std::stringstream tempsi;
	tempsi.str( std::string() );
	tempsi << "Highscore: " << highscore;
	//tempsi << "recoilBloomAmount: " << recoilBloomAmount;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {244, 144, 20}, gFont26);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 5, gText.getWidth(), gText.getHeight());

	tempsi.str( std::string() );
	tempsi << "Score: " << score;
	//tempsi << "Shooting: " << shooting;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont26);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 5+26, gText.getWidth(), gText.getHeight());

	tempsi.str( std::string() );
	tempsi << "Wave: " << wave;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont26);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 5+26*2, gText.getWidth(), gText.getHeight());

	tempsi.str( std::string() );
	tempsi << "Pirates: " << pirateCount;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {200,20,20}, gFont26);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 5+26*3, gText.getWidth(), gText.getHeight());

	/*tempsi.str( std::string() );
	tempsi << "Grenades: " << grenades;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont26);
	gText.render(gRenderer, 10, screenHeight-gText.getHeight(), gText.getWidth(), gText.getHeight());*/

	/*tempsi.str( std::string() );
	tempsi << "Health: " << health;
	gText.loadFromRenderedText(tempsi.str().c_str(), {0,255,0}, gFont26, gRenderer);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 58, gText.getWidth(), gText.getHeight());*/

	// Check what keys are pressed
	/*for (int i=0; i<25; i++){
		std::std::stringstream tempss;
		tempss << "i: " << i << " -        " << SDL_JoystickGetButton(joy, i);
		gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont13, gRenderer);
		gText.render(gRenderer, 5, 5+i*15, gText.getWidth(), gText.getHeight());
	}

	std::std::stringstream tempss;
	tempss.str(std::string());
	tempss << "Axis: " << 0 << " -        " << SDL_JoystickGetAxis(joy, 0);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont13, gRenderer);
	gText.render(gRenderer, 60, 5, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 1 << " -        " << SDL_JoystickGetAxis(joy, 1);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont13, gRenderer);
	gText.render(gRenderer, 60, 20, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 2 << " -        " << SDL_JoystickGetAxis(joy, 2);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont13, gRenderer);
	gText.render(gRenderer, 60, 35, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 3 << " -        " << SDL_JoystickGetAxis(joy, 3);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont13, gRenderer);
	gText.render(gRenderer, 60, 50, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 4 << " -        " << SDL_JoystickGetAxis(joy, 4);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont13, gRenderer);
	gText.render(gRenderer, 60, 65, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 5 << " -        " << SDL_JoystickGetAxis(joy, 5);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont13, gRenderer);
	gText.render(gRenderer, 60, 80, gText.getWidth(), gText.getHeight());*/
}


// Key Pressed
void Players::OnKeyDown( Players &player, SDL_Keycode sym )
{
	switch (sym){
	case SDLK_a:					// move left
		player.controls = 0;
		player.moveleft = true;
		break;
	case SDLK_d:					// move right
		player.controls = 0;
		player.moveright = true;
		break;
	case SDLK_w:					// move up
		player.controls = 0;
		player.moveup = true;
		break;
	case SDLK_s:					// move down
		player.controls = 0;
		player.movedown = true;
		break;
	case SDLK_r:					// reload
		player.controls = 0;
		if (!player.reload && player.ammo < 30) {
			player.reload = true;
		}
		break;
	case SDLK_q:					// grenade throw
		player.controls = 0;
		player.grenadeTrigger = true;
		break;
	case SDLK_h:
		//debug = ( !debug );
		break;
	case SDLK_y:
		player.camlocked = ( !player.camlocked );
		break;
	case SDLK_SPACE:
		//player.initialshot = true;
		break;
	case SDLK_LSHIFT:
		player.shift = true;
		break;
	}
}

// Key Released
void Players::OnKeyUp( Players &player, SDL_Keycode sym )
{
	switch (sym){
	case SDLK_a:
		player.moveleft = false;
		break;
	case SDLK_d:
		player.moveright = false;
		break;
	case SDLK_w:
		player.moveup = false;
		break;
	case SDLK_s:
		player.movedown = false;
		break;
	case SDLK_SPACE:
		//player.initialshot 	= false;
		break;
	case SDLK_LSHIFT:
		player.shift 		= false;
		break;
	}
}

// Player Mouse Pressed
void Players::mouseClickState(Players &player, SDL_Event &e){
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			player.controls = 0;
			player.leftclick = true;
			player.initialshot = true;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			player.controls = 0;
			player.rightclick = true;
			player.thrust = true;
		}
	}else if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			player.leftclick = false;
			player.initialshot = false;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			player.rightclick = false;
			player.thrust = false;
		}
	}
}

// Update XBOX 360 controls
void Players::updateJoystick(Players &player, SDL_Event &e){

	/* Xbox 360 Controls */

	// Get Left Analog Angle
	if (((SDL_JoystickGetAxis(player.joy, 0) < -JOYSTICK_DEAD_ZONE) || (SDL_JoystickGetAxis(player.joy, 0) > JOYSTICK_DEAD_ZONE)) ||
		((SDL_JoystickGetAxis(player.joy, 1) < -JOYSTICK_DEAD_ZONE) || (SDL_JoystickGetAxis(player.joy, 1) > JOYSTICK_DEAD_ZONE))){
		player.LAngle = atan2(SDL_JoystickGetAxis(player.joy, 1), SDL_JoystickGetAxis(player.joy, 0)) * ( 180.0 / M_PI );
	}

	// Xbox 360 Controls
	if (e.type == SDL_JOYBUTTONDOWN && e.jbutton.state == SDL_PRESSED && e.jbutton.which == 0){
		switch(e.jbutton.button){
		case 0:
			player.D_UP = true;
			break;
		case 1:
			player.D_DOWN = true;
			break;
		case 2:
			player.D_LEFT = true;
			break;
		case 3:
			player.D_RIGHT = true;
			break;
		case 4:
			player.START = true;
			break;
		case 5:
			player.BACK = true;
			break;
		case 6:
			player.L3 = true;
			break;
		case 7:
			player.R3 = true;
			break;
		case 8:
			player.LB = true;
			break;
		case 9:
			player.RB = true;
			break;
		case 10:
			player.A = true;
			break;
		case 11:
			player.B = true;
			break;
		case 12:
			player.X = true;
			break;
		case 13:
			player.Y = true;
			break;
		}
	}else if (e.type == SDL_JOYBUTTONUP && e.jbutton.state == SDL_RELEASED && e.jbutton.which == 0){
		switch(e.jbutton.button){
		case 0:
			player.D_UP = false;
			break;
		case 1:
			player.D_DOWN = false;
			break;
		case 2:
			player.D_LEFT = false;
			break;
		case 3:
			player.D_RIGHT = false;
			break;
		case 4:
			player.START = false;
			break;
		case 5:
			player.BACK = false;
			break;
		case 6:
			player.L3 = false;
			break;
		case 7:
			player.R3 = false;
			break;
		case 8:
			player.LB = false;
			break;
		case 9:
			player.RB = false;
			break;
		case 10:
			player.A = false;
			break;
		case 11:
			player.B = false;
			break;
		case 12:
			player.X = false;
			break;
		case 13:
			player.Y = false;
			break;
		}
	}
}



int Players::getTargetID() {
	//return target.id;
}

float Players::getX() {
	return x;
}

float Players::getY() {
	return y;
}

float Players::getCenterX() {
	return x+w/2;
}

float Players::getCenterY() {
	return y+h/2;
}

float Players::getvX() {
	return vX;
}

float Players::getvY() {
	return vY;
}

float Players::getLookAngle() {
	//return lookAngle;
}
