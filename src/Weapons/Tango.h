#pragma once
#include "Weapons/Gun.h"
#include "Game/AnimateStateMachine.h"
#include "Weapons/BurstMode.h"
#include "Time/Timer.h"

/*
	This is the secret weapon of the game. Project TANGO
*/

class Tango : public Gun
{
public:
	Tango() = default;

	void initTango(SDL_Renderer* pRenderer, SDL_FRect pCharRect, const Config::ReloadConfig& pReloadConfig,
				   const Config::BulletsConfig& pBulletsConfig, const Config::WeaponConfig& pWeaponConfig);
	void initTangoAutomaticaly(SDL_Renderer* pRenderer, SDL_FRect pCharRect);

	std::pair<int32_t, bool> manageDamage(SDL_FRect pEnemyRect);

	BurstMode& getFireMode();

	void updateBullets(SDL_Renderer* pRenderer) override;
	void shoot() override;
	void reload() override;
	bool checkDamage(SDL_FRect pEnemyRect) override;
	void setAsASpecialWeapon() override;
	void accept(WeaponVisitor& pWeaponVisitor) override;

	void setBulletsPerShot(int32_t pQuantity);
	void setRangeOfBullets(int32_t pRange);
	void setDelayOfStoraging(int32_t pDelay);

	void renderCharge(SDL_Renderer* pRenderer);
private:
	bool mShowReady{ false };
	bool mWasReload{ false };

	Timer mTimer;
	FireModeFactory mFireModeFactory;
	FactoryObjects mFactoryObjects;
	AnimateStateMachine mAnimateStateMachine;
};

