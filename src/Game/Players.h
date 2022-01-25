/*
 * Players.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#ifndef LOCAL_PLAYERS_H_
#define LOCAL_PLAYERS_H_

#include <SDL2/SDL_ttf.h>

#include "../LWindow.h"
#include "../LTexture.h"
#include "../Particless.h"
#include "Asteroids.h"
#include "Enemies.h"
#include "../Helper.h"
#include "Maps.h"

//Player
class Players: public Helper {
public:
	enum Result { QUIT, LOADMENU, PLAYGAME, QUITPLAYGAME };
public:
	// Joystick Sensitivity
	const int JOYSTICK_DEAD_ZONE = 8000;
	void OnKeyDown( Players &player, SDL_Keycode sym );
	void OnKeyUp( Players &player, SDL_Keycode sym );
	void mouseClickState(Players &player, SDL_Event &e);
	void updateJoystick(Players &player, SDL_Event &e);

public:	// resources
	// Local resources
	LTexture spr_player_left_arm;
	LTexture spr_player_left_bicep;
	LTexture spr_player_right_arm;
	LTexture spr_player_gun;
	LTexture spr_player_muzzle_flash;
	LTexture spr_player_body;
	LTexture spr_player_right_shoulder;
	LTexture spr_player_left_shoulder;
	LTexture spr_player_backpack;
	LTexture spr_player_head;
	LTexture gShield;
	SDL_Rect rShield[7];

public:	// variables
	int timer = 0;
	//int w = 208, h = 208;
	int w = 90, h = 90;				// object size in collision pixels
	int realw = 208, realh = 208;	// object size in actual pixel size
	//float x = map.x+map.w/2-32/2,
	//	  y = map.y+map.h/2-32/2;
	float x = 0,
		  y = 0;
	float x2, y2;					// player circle center
	int radius = 45;				// player radius
	int panTimer 		= 0;
	int shoottimer 		= 0;
	float angle 		= 0.0,
		   radians 		= 0.0,
		   radianCos 	= 0.0,
		   radianSin 	= 0.0;
	float vX = 0.0,
		  vY = 0.0,
		  thrustSpeed 	= 0.0;
	bool camlocked 		= true;
	bool moveleft 		= false,
		 moveright 		= false,
		 moveup 		= false,
		 movedown		= false;
	std::string name;
	bool thrust 		= false;
	int thrustT 		= 0;
	bool shift 			= false;
	int timer2 			= 0;
	double time 		= 0;
	bool deathScreen	= false;
	bool alive			= false;
	bool returned		= false;
	Uint8 alpha			= 255;
	unsigned int score	= 0;
	unsigned int highscore = 0;
	int HW				= 0;
	int increment		= 35;
	int wave			= 0;
	double accuracy		= 0.0;
	float totalShot		= 0;		// Total shot
	float hits			= 0;		// Total hits
	int bg 				= 1;

	// Fire-rate
	//string power-up 		= "LAZER";
	int newmx			= 0;	// new mouse position
	int newmy			= 0;	// new mouse position
	int powerup 		= 1;
	double barrelX		= 0;	// gun barrel position
	double barrelY		= 0;
	double barrelW		= 0;	// whether to add width or subtract from gun barrel position
	double barrelH		= 0;
	double bulletW 		= 92;	// position the bullet will come out of the player ONLY if player
	double bulletH 		= 21;	// is centered (have angle at 0.0 to test)
	double particleW 	= 30;
	double particleH 	= 30;
	float AttackSpeed = 10;
	float delayT 		= 0;
	bool delay 			= false;
	bool initialshot 	= false;
	bool trigger 		= false;
	bool renderFlash	= false; // shuts off right after 1 frame
	double recoilX = 0;			 // gun recoil
	double recoilY = 0;

public: // Shooting variables

	bool shooting = false;
	float recoilBloomAmount = 1.0;	// As the player shoots, bloom will increase
	const float recoilBloomAmountMax = 10;
public: // Health variables

	// Lives
	int lives = 1;
	int health			= 200;

	// Shield
	int shieldFrame;
	int shieldTick;
	int shieldT;
	bool shield;

	double tempc, temps;
	SDL_Rect continueButton[3];

	// High-Score display
	std::string highList[10];
	int indx 		= 0;
	float travel 	= 0.0;
	int position	= 0;
	int position2	= 0;
	int dir 		= 1;
	int indexSaved 	= -1;

	/* This will bring the player
	 * to life. At least, that's what
	 * it'll look like. This will rustle
	 * the player's armor as they move
	 */
	float rustleW = 0;
	float rustleH = 0;
	float rustleDirX = 1;
	float rustleDirY = 1;

	bool moving = false;
	std::string tag;

public: // Player abilities
	/* Grenade */
	int grenades;
	int grenadesMax;
	double grenadesCD;
	double grenadesCDSpeed;
	double grenadesCDMax;
	bool grenadeTrigger;

	/* Machine Gun Ammo */
	int ammo = 30;				// current ammo in clip
	int ammoClip = 30;			// magazine capacity
	int ammoMax = 300;			// max ammo capacity
	bool reload = false;		// reload
	double reloadSpeed = 30.0;	// reload speed
	double reloadTimer = 0.0;	// reload timer

public:	// controls
	// Player controls
	int controls;		// [0] Keyboard, [1] Xbox 360 Controller
	SDL_Joystick *joy;
	bool A;
	bool B;
	bool X;
	bool Y;
	bool D_UP;
	bool D_DOWN;
	bool D_LEFT;
	bool D_RIGHT;
	bool LB;
	bool RB;
	bool BACK;
	bool START;
	bool L3;
	bool R3;
	double LAngle;
	double RAngle;
	double zAxisLeft;
	double zAxisRight;
	bool leftclick;
	bool rightclick;
	bool test;

public:	// Call these functions to borrow something from the main

public:	// functions
	// Functions
	void resetHighScore();
	void loadResources(SDL_Renderer* gRenderer);
	void freeResources();
	void loadScore();
	void saveHighScore();
	void reset(float spawnX, float spawnY, std::string newName, bool respawn);
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	// Applies a shield to Player
	void applyShield();

	// Player shoot
	void fire(Particle particle[], Particle &p_dummy, int mx, int my,
			  Mix_Chunk* sLazer, Mix_Chunk* sGrenade, Mix_Chunk* sGrenadePickup,
			  Mix_Chunk* sPistolReload);

	// Player controls
	void move(Particle particle[], Particle &p_dummy, int mx, int my);

	void update(Map &map,
				Asteroid asteroid[], Asteroid &a_dummy,
				Enemy enemy[], Enemy &e_dummy,
				Particle particle[], Particle &p_dummy,
				int mx, int my, int camx, int camy,
				float spawnX, float spawnY,
				LWindow gWindow, SDL_Renderer* gRenderer,
				LTexture gText, TTF_Font *gFont, SDL_Color color,
				Mix_Chunk *sAtariBoom, Mix_Chunk* sLazer, Mix_Chunk* sGrenade,
				Mix_Chunk* sGrenadePickup, Mix_Chunk* sPistolReload);

	void render(int mx, int my, int camx, int camy, LWindow gWindow,
				SDL_Renderer* gRenderer,
				TTF_Font *gFont, TTF_Font *gFont2,
				SDL_Color color, int &PARTICLES, LTexture gText);
};

#endif /* LOCAL_PLAYERS_H_ */
