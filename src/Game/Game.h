#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <array>
#include <unordered_map>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "States/GameStates.h"
#include "Loger/Loging.h" 
#include "Time/Timer.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

enum class Type{LOOPBACK,ACTIONS,OUTRO};

class Game
{
public:

	Game(const int pFlags, const std::string& pNameOfWindow, Uint32 pX, Uint32 pY);
	~Game() {
		SDL_DestroyRenderer(mRenderer);
		SDL_DestroyWindow(mWindow);
		IMG_Quit();
		SDL_Quit();
	}

	//enum class Type{LOOPBACK,ACTIONS,OUTRO};
	void setActions(Type pType, std::function<void(void)> pFunc);
	void startLoop();
	void stopLoop();

	//void setCurrentState(GameStates* pStates);
	void setPositionOfWindow(const int pX, const int pY);
	void setRgbOfWindow(uint8_t pR, uint8_t pG, uint8_t pB, uint8_t pA);
	void setFps(Uint32 FPS);

	std::filesystem::path getPath() const { return std::filesystem::current_path(); }
	inline int getWindowsWidth() const { return WIN_WIDTH; }
	inline int getWindowsHeight() const { return WIN_HEIGHT; }
	inline SDL_Window* getWindow() const { return mWindow; }
	inline SDL_Renderer* getRenderer() const { return mRenderer; }
	inline SDL_bool loopIsRunning() const { return mIsRunning; }
	inline Uint32 getFPS() const { return (1000 / mDelay); }

private:
	void seter();

private:
	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	SDL_bool mIsRunning;

	Timer mTimer;

	uint8_t mR, mG, mB, mA;
	Uint32 mWindowsX, mWindowsY;
	Uint32 mDelay;
	
	std::unordered_map<Type, std::function<void(void)>> mMapOfFunc;
};

