#pragma once
#include <vector>

#include <SDL.h>

#include "Time/Timer.h"
#include "Game/AnimatedTexture.h"
#include "Game/AnimateStateMachine.h"
#include "Game objets/FactoryOfFonts.h"

class ReloadLogic
{
public:
	ReloadLogic() = default;

	// if pShowReloadingQuote will be false - then the game will show just numbers'
	// Reloading time - in seconds
	void initReloadLogic(SDL_Renderer* pRenderer, SDL_FRect pCharRect,
						 int16_t pReloadingTime, bool pShowReloadingQuote,
						 SDL_Color pColorNumbers = { 255,255,255,255 },
						 int32_t pSizeNumbers = 25);

	bool isReloading();
	void startReloading();

	void showReloadQuote(bool pShowing);
	void setRelodingTime(float pReloadingTime);
	float getReloadingTime();

	void render(SDL_Renderer* pRenderer);
	void update(SDL_Rect pCharRect);

private:
	bool manageDelay();

private:
	SDL_Renderer* mRenderer{};

	bool mShowingQuote{ false };
	bool mIsReloading{ false };
	bool mNeedToSubtract{ false };

	int16_t mReloadingTime{};
	int16_t mCurrentReloadingTime{};
	int16_t mCounterTimer{1};

	SDL_Color mColor{};
	SDL_Rect mRectNumbers{};

	Timer mTimerReload;
	AnimateStateMachine mAnimateStateMachine;

};

