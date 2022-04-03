/* Game Engine by Carl Mesias using 'SDL2 Graphics Library'
 *
 * 4-19-2017
 * Finish Player.h class by also updating input events
 * Add inheritance of LTexture.h into Player.h
 *
 *
 *
 *
 */

#include "Game.h"
#include "SplashScreen.h"
#include "Engine/MainMenu.h"
#include "CustomizeCharacter.h"
#include "ActSelection.h"
#include "PlayGame.h"

#include "stdlib.h"
#include "stdio.h"
#include "time.h"

// TODO [ ] - get input from player so they can input their names
// TODO [ ] - make pause menu

// Initialize
void Game::Init() {
	if (_gameState != Uninitialized)
		return;

	// Load Video settings from file
	loadVideoCFG();

	// Initialize Video
	SDL_Init( SDL_INIT_VIDEO);

	// Create window
	gWindow.create("CEngine V2", RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN );//| SDL_WINDOW_RESIZABLE );

	// Create renderer for window
	gRenderer = gWindow.createRenderer();
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Set render size
	SDL_RenderSetLogicalSize(gRenderer,screenWidth,screenHeight);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);
	TTF_Init();

	//SDL_Init(SDL_INIT_EVERYTHING);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
		Mix_GetError());
	}

	// Set Game-State to ShowSplash() first before anything else
	_gameState = Game::ShowingSplash;

	//SDL_MaximizeWindow(gWindow.getWindow());
	//SDL_SetWindowFullscreen(gWindow.getWindow(), SDL_TRUE);


}

// Load
void Game::Load() {

	// Load Audio settings from file
	loadAudioCFG();

	// load audio
	LoadAudioFiles();

	// Apply audio configurations
	applyOldAudioCFG();

	// load fonts
	gFont 	= TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 18);
	gFont13 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 13);
	gFont26 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 26);

	// load particle textures
	setClips(cParticles, 32, 0, 8, 8);
	gParticles.loadFromFile(gRenderer, "resource/gfx/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);
}

// Free
void Game::Free() {
	// free audio
	FreeAudioFiles();

	// free textures
	gText.free();
	gParticles.free();

	// free fonts
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont26);
	gFont = NULL;
	gFont13 = NULL;
	gFont26 = NULL;
}

// Start Game
void Game::Start() {

	srand(time(0));

	Init();
	Load();

	// While loop
	while (!IsExiting()) {
		GameLoop();
	}

	Free();

	// Close upon exit
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Exit Game
bool Game::IsExiting() {
	if (_gameState == Game::Exiting)
		return true;
	else
		return false;
}

// Loop Game
void Game::GameLoop()
{
	switch (_gameState)
	{
		case Game::ShowingSplash:							// Splash Screen
			{
				ShowSplashScreen();
				break;
			}
		case Game::ShowingMenu:								// Menu
			{
				ShowMenu();
				break;
			}
		case Game::ShowingCustomizeCharacter:				// Customize Character
			{
				ShowCustomizeCharacter(gWindow, gRenderer);
				break;
			}
		case Game::ShowingActSelection:						// Play Game
			{
				ShowActSelectionScreen(gWindow, gRenderer);
				break;
			}
		case Game::ShowingPlayGame:							// Play Game
			{
				ShowPlayGame();
				break;
			}
		case Game::Exiting:									// Quit
			{
				IsExiting();
				break;
			}
	}
}


// Splash Screen
void Game::ShowSplashScreen() {
	// Create Splash Screen
	SplashScreen splashScreen;
	// Show Main Menu
	SplashScreen::SplashResult result;
	splashScreen.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case SplashScreen::Play:					// Play (Next scene is Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case SplashScreen::Nothing:
		//
		break;
	case SplashScreen::Exit:					// Quit
		_gameState = Game::Exiting;
		break;
	}
}

// Splash Screen
void Game::ShowMenu() {
	// Create Main Menu
	MainMenu mainMenu;
	// Show Main Menu
	MainMenu::MenuResult result;
	mainMenu.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case MainMenu::NewGame:					// Menu returned 'Play', NewGame()
		//_gameState = Game::ShowingPlayGame;
		//_gameState = Game::ShowingActSelection;
		_gameState = Game::ShowingCustomizeCharacter;

		break;
	case MainMenu::HowToPlay:				// Menu returned 'Load', LoadGame()
		//_gameState = Game::LoadGameScene;
		break;
	case MainMenu::Options:					// Menu returned 'Options', Options()
		_gameState = Game::OptionScene;
		break;
	case MainMenu::Credits:					// Menu returned 'Credits', Credits()
		_gameState = Game::CreditScene;
		break;
	case MainMenu::Nothing:
		//
		break;
	case MainMenu::Exit:					// Menu returned 'Exit', Quit()
		_gameState = Game::Exiting;
		break;
	}
}

/* Customize Character */
void Game::ShowCustomizeCharacter(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Create Main Menu
	CustomizeCharacter customizeCharacter;
	// Show Main Menu
	CustomizeCharacter::Result result;
	customizeCharacter.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case CustomizeCharacter::Back:				// Back (to Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case CustomizeCharacter::StartGame:			// Start Game
		//_gameState = Game::ShowingActSelection;
		_gameState = Game::ShowingPlayGame;
		break;
	case CustomizeCharacter::Nothing:			// Nothing
		//
		break;
	case CustomizeCharacter::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

/* New Game: Choose Act # to Play */
void Game::ShowActSelectionScreen(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Create Main Menu
	ActSelection actSelection;
	// Show Main Menu
	ActSelection::Result result;
	actSelection.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case ActSelection::Back:				// Back (to Main Menu)
		//_gameState = Game::ShowingMenu;
		break;
	case ActSelection::StartGame:			// Start Game
		//_gameState = Game::PlayGameScene;
		break;
	case ActSelection::Nothing:				// Nothing
		//
		break;
	case ActSelection::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

/* Play Game */
void Game::ShowPlayGame() {
	// Create Main Menu
	PlayGame game1;
	// Show Main Menu
	PlayGame::Result result;
	game1.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case PlayGame::Back:				// Back (to Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case PlayGame::Nothing:				// Nothing
		//
		break;
	case PlayGame::StartGame:			// Start Game
		//_gameState = Game::PlayGameScene;
		break;
	case PlayGame::ShowingMenu:			// Nothing
		//
		break;
	case PlayGame::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}


Game::GameState Game::_gameState = Uninitialized;



