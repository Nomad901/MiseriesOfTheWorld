#pragma once
#include <SDL/SDL.h>

#include "Assets.h"
#include "AnimatedTexture.h"
#include "Game objets/FactoryObjects.h"
#include "Game objets/FactoryOfEntities.h"
#include "Game objets/FactoryOfFonts.h"
#include "Time/Timer.h"

//
// temporary class, which is not used yet. will be set in the future
//

class Renderer
{
public:
	Renderer(SDL_Renderer* pRenderer);

	//
	// invokes setImplementation and operates over FPS;
	//
	void drawAll(uint32_t pDelay);

private:
	SDL_Renderer* mRenderer{};
	
	Timer mTimer;
};

