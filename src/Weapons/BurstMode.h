#pragma once
#include "Weapons/FireMode.h"
#include "Game/Randomizer.h"
#include "Game/AnimatedTexture.h"
#include "Game/AnimateStateMachine.h"
#include "Game objets/FactoryOfFonts.h"

// can hold the mouse in order to make more bullets. 
// but at the same time will be increasing percent of spreading

class BurstMode : public FireMode
{
public:
	BurstMode(SDL_Renderer* pRenderer);

	void shootChargedBullets(SDL_Rect pCharRect, SDL_Rect pWeaponRect);
	void shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload) override;
	void update(SDL_Renderer* pRenderer, const Vector2f& pPos) override;
	void render() override;
	bool manageDelay() override;

	void manageAnim(SDL_Renderer* pRenderer, const Vector2f pWeaponPos);

	void setAsSpecial() override;
	void setQuantityBulletsPerTime(int32_t pQuantityBullets);
	void setRangeOfSpread(int32_t pRange);

	// pDelay - milliseconds. each pDelay will be 
	// spared 1 bullet
	void setDelayOfStoraging(int32_t pDelay);
	
	void setSparing(bool pSparing);
	bool isSparing() const noexcept;
	int32_t getItensisty() const noexcept;

protected:
	bool managingBurst();

	void manageReload(bool pWasReload);
	void subtractionBullets();

private:
	bool mSparing{ false };
	bool mShowReady{ false };

	int32_t mDelayStoraging{};
	int32_t mCurrentQuantityBullets{ 1 };
	int32_t mQuantityBulletsPerTime{};
	int32_t mMeasureBreakingWeapon{};
	int32_t mMultiplier{ 20 };

	SDL_Rect mCharRect{};

	RotateMachine mRotateMachine;
	Timer mTimer;
	Timer mBurstTimer;
	AnimateStateMachine mAnimatedStateMachine;
};

