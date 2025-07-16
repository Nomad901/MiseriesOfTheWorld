#pragma once
#include "Weapons/FireMode.h"
#include "Game/Randomizer.h"

/* sequence of bullets for guns
   How its working: 
   first {amountOfBullets} are going out of the weapon, then we are waiting
   100 miliseconds (we can set this), forth next {amountOfBullets} are open to shoot
*/

class Irregular : public FireMode
{
public:
	Irregular();

	void shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload) override;
	
	void setAsSpecial() override;
	void setQuantityBulletsPerTime(int32_t pQuantityBullets);
	void setBurstCooldown(int32_t pBurstCooldown);

	void render() override;
	void update(SDL_Renderer* pRenderer, const Vector2f& pPos) override;
	bool manageDelay() override;
protected:
	bool manageDelaySequence();

	void manageReload(int32_t& pTmpBreakingWeapon, bool pWasReload);
	void subtractionBullets(int32_t& pTmpBreakingWeapon);

private:
	bool mActive{ false };

	int32_t mCounterBullets{0};
	int32_t mQuantityBulletsPerTime{0};
	int32_t mBurstCooldown{0};

	RotateMachine mRotateMachine;
	Timer mTimer;
	Timer mTimerForBurst;
};

