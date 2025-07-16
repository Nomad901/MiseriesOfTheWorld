#include "Game/Handler.h"
#include "Game/InputManager.h"
#include "Time/Timer.h"
#include "Game/Assets.h"
#include "Game/AnimatedTexture.h"
#include "States/GameStates.h"
#include "States/Menu.h"
#include "States/InGame.h"

Handler::Handler(Game* pGame)
	:mGame(pGame),
	mInterpolation{ 0.0f }
{
	Assets::init();
	TextureManager::init();
	InputManager::init();
	FactoryOfFonts::init();
	//mBulletsPool = std::make_unique<BulletsPool>();
	mFactoryObjects = std::make_unique<FactoryObjects>();
	//mRandomizerX = std::make_unique<Randomizer>(0, WIN_WIDTH);
	//mRandomizerY = std::make_unique<Randomizer>(0, WIN_HEIGHT);
}

Handler::~Handler()
{
	Assets::shutDown();
	InputManager::shutDown();
	TextureManager::shutDown();
	FactoryOfFonts::shutDown();
}

void Handler::loopBack()
{
	mFactoryObjects->appendObject("Character", { 600,600, 100,100 }, { 255,255,255,255 });
	mFactoryObjects->appendObject("Enemy", { 800, 600, 100,100 }, { 0,0,0,255 });
	
	mWeaponManager.initWeapons(mGame->getRenderer(), mFactoryObjects->convertRect(mFactoryObjects->getRect("Character")));
}

void Handler::actions()
{
	static int x = 0, y = 0;
	
	SDL_Event events;
	while (SDL_PollEvent(&events))
	{
		InputManager::getInstance().update(mGame, events);

		if (InputManager::getInstance().isMouseHeld(MouseButton::LEFT))
		{
			auto tmpName = mWeaponManager.weaponIsGripedBy(mFactoryObjects->convertRect(mFactoryObjects->getRect("Character")));
			if (!tmpName.has_value())
			{

			}
			else
			{
			}
		}
		if (InputManager::getInstance().isMousePressed(MouseButton::RIGHT))
		{

		}

		if (InputManager::getInstance().isHeld(SDL_SCANCODE_W))
			mFactoryObjects->setPosition("Character", { mFactoryObjects->getPos("Character").mX, mFactoryObjects->getPos("Character").mY - 5 });
		if (InputManager::getInstance().isHeld(SDL_SCANCODE_S))
			mFactoryObjects->setPosition("Character", { mFactoryObjects->getPos("Character").mX, mFactoryObjects->getPos("Character").mY + 5 });
		if (InputManager::getInstance().isHeld(SDL_SCANCODE_A))
			mFactoryObjects->setPosition("Character", { mFactoryObjects->getPos("Character").mX - 5, mFactoryObjects->getPos("Character").mY });
		if (InputManager::getInstance().isHeld(SDL_SCANCODE_D))
			mFactoryObjects->setPosition("Character", { mFactoryObjects->getPos("Character").mX + 5, mFactoryObjects->getPos("Character").mY });

		InputManager::getInstance().updatePrevStates();
	}
}

void Handler::outro()
{
	mTimer.startTimer();



	mTimer.setProperFPS(1);
}

