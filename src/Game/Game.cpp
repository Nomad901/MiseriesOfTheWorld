#include "Game.h"

Game::Game(const int pFlags, const std::string& pNameOfWindow, Uint32 pX, Uint32 pY) 
	: mWindowsX{ pX },
	  mWindowsY{ pY }
{
	seter();
	mMapOfFunc.reserve(3);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		LOG("Couldn't initialize SDL video / audio: " + std::string(SDL_GetError()));
	if (IMG_Init(IMG_INIT_PNG) == 0)
		LOG("Couldn't initialize PNG support: " + std::string(SDL_GetError()));
	if (IMG_Init(IMG_INIT_JPG) == 0)
		LOG("Couldn't initialize JPG support: " + std::string(SDL_GetError()));

	mWindow = SDL_CreateWindow(pNameOfWindow.c_str(), pX, pY, WIN_WIDTH, WIN_HEIGHT, pFlags);
	if (!mWindow)
		LOG("Couldn't create window: " + std::string(SDL_GetError()));

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
		LOG("Couldn't create renderer: " + std::string(SDL_GetError()));
}

void Game::setActions(Type pType, std::function<void(void)> pFunc)
{
	mMapOfFunc.insert_or_assign(pType, pFunc);
}

void Game::startLoop()
{
	mMapOfFunc[Type::LOOPBACK]();
	while (mIsRunning) {
		mTimer.startTimer();

		mMapOfFunc[Type::ACTIONS]();

		SDL_SetRenderDrawColor(mRenderer, mR, mG, mB, mA);
		SDL_RenderClear(mRenderer);

		mMapOfFunc[Type::OUTRO]();

		SDL_RenderPresent(mRenderer);

		mTimer.setProperFPS(mDelay);
	}
}

void Game::stopLoop()
{
	mIsRunning = SDL_FALSE;
}

void Game::setPositionOfWindow(const int pX, const int pY)
{
	mWindowsX = pX; mWindowsY = pY;
	SDL_SetWindowPosition(mWindow, pX, pY);
}

void Game::setRgbOfWindow(uint8_t pR, uint8_t pG, uint8_t pB, uint8_t pA)
{
	mR = pR; mG = pG; mB = pB; mA = pA;
}

void Game::setFps(Uint32 FPS)
{
	mDelay = (1000 / FPS);
}

void Game::seter()
{
	mR = 0; mG = 0; mB = 0; mA = SDL_ALPHA_OPAQUE;
	mDelay = 16;
	mIsRunning = SDL_TRUE;
}