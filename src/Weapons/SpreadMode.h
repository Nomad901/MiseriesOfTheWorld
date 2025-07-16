#pragma once
#include "Weapons/FireMode.h"
#include "Game/Randomizer.h"

//just a mode for shotguns!

class SpreadMode : public FireMode
{
public:
	SpreadMode();

	void shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload) override;
	void update(SDL_Renderer* pRenderer, const Vector2f& pPos) override;
	void render();

	void setAsSpecial() override;
	void setQuantityBulletsPerTime(int32_t pQuantityBullets);
	void setRangeOfSpread(int32_t pRange);
	bool manageDelay() override;

protected:

	void manageReload(int32_t& pTmpBreakingWeapon, bool pWasReload);
	void subtractionBullets(int32_t& pTmpBreakingWeapon);

private:
	int32_t mQuantityBulletsPerTime{};
	int32_t mMultiplier{20};

	RotateMachine mRotateMachine;
	Timer mTimer;
};
