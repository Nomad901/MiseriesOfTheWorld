#pragma once
#include "Loger/Loging.h"
#include "Game/TextureManager.h"
#include "Game/Assets.h"
#include "Game/Game.h"

#define SCREEN_RECT { 0, 0, WIN_WIDTH, WIN_HEIGHT }

class GameStates
{
public:
	~GameStates() = default;

	virtual void hopIn() = 0;
	virtual void getOutta() = 0;

	virtual void handleEvents(std::weak_ptr<class Game> pGame) = 0;

	virtual void render() = 0;
	virtual void update() = 0;

};

