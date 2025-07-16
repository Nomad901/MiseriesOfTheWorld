#pragma once
#include "Gun.h"

class SpreadMode;

/*
	how to init this with your hands -> look at initRifleAutomaticaly function;
*/

class Shotgun : public Gun
{
public:
	Shotgun() = default;

	void initShotgun(SDL_Renderer* pRenderer, SDL_FRect pCharRect, const Config::ReloadConfig& pReloadConfig,
					 const Config::BulletsConfig& pBulletsConfig, const Config::WeaponConfig& pWeaponConfig);
	void initShotgunAutomaticaly(SDL_Renderer* pRenderer, SDL_FRect pCharRect);
	
	void updateBullets(SDL_Renderer* pRenderer);
	std::pair<int32_t, bool> manageDamage(SDL_FRect pEnemyRect);

	void shoot() override;
	void reload() override;
	bool checkDamage(SDL_FRect pEnemyRect) override;
	void setAsASpecialWeapon() override;
	void accept(WeaponVisitor& pWeaponVisitor) override;

	void setBulletsPerShot(int32_t pQuantity);
	void setRangeOfBullets(int32_t pRange);

private:
	bool mWasReload{ false };

	FireModeFactory mFireModeFactory;
	FactoryObjects mFactoryObjects;

};
