#pragma once
#include <filesystem>
#include <array>

#include <SDL.h>

#include "Game.h"
#include "Main Character/Character.h"
#include "Game objets/FactoryObjects.h"
#include "Game/RotateMachine.h"
#include "Game objets/FactoryOfFonts.h"
#include "Weapons/WeaponManager.h"

class Timer;
class InputManager;
class AnimatedTexture;
class GameStates;
class Menu;
class InGame;

using PATH = std::filesystem::path;

class Handler
{
public:
	Handler(Game* pGame);
	~Handler();

	void loopBack();
	void actions();
	void outro();
	
	//void popState(GameStates* pGameState);
	//void pushState(GameStates* pGameState);
	//void changeCurState(GameStates* pGameState);

	void updateAll() {}

private:
	bool mFont{ false };
	Timer mTimerForChanged;

	bool mShouldUpdate{ false };
	float mInterpolation{0.0f};
	const int TICKS_PER_SECOND = 60;
	const int MS_PER_SEC = 1000 / TICKS_PER_SECOND;
	
	std::unique_ptr<Randomizer> mRandomizerX;
	std::unique_ptr<Randomizer> mRandomizerY;
	
	std::unique_ptr<FactoryObjects> mEnemy;

	AnimatedTexture mAnimated;
	AnimatedTexture mZombie;
	std::unique_ptr<FactoryObjects> mFactoryObjects;
	std::unique_ptr<Character> mCharacter;
	std::unique_ptr<Menu> mMenu;
	std::unique_ptr<InGame> mInGame;
	std::vector<std::unique_ptr<GameStates>> mGameStates;
	std::shared_ptr<Game> mGame;
	Timer mTimer;
	Timer mTimerForBullets;

	RotateMachine mRotate;

	WeaponManager mWeaponManager;

	bool mLost{ false };
};

