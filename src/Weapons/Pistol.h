#pragma once
#include <SDL.h>

#include "Gun.h"

class SingleOrSequence;

enum class PistolType
{
	DEFAULT = 0,
	FAST = 1
};

/*
	how to init this with your hands -> look at initPistolAutomaticaly function;
*/

class Pistol : public Gun
{
public:
	Pistol() = default;

	void initPistol(SDL_Renderer* pRenderer, PistolType pType, SDL_FRect pCharRect, const Config::ReloadConfig& pReloadConfig,
				    const Config::BulletsConfig& pBulletsConfig, const Config::WeaponConfig& pWeaponConfig);
	void initPistolAutomaticaly(SDL_Renderer* pRenderer, PistolType pType, SDL_FRect pCharRect);

	void updateBullets(SDL_Renderer* pRenderer);
	std::pair<int32_t, bool> manageDamage(SDL_FRect pEnemyRect);

	void shoot() override;
	void reload() override;
	bool checkDamage(SDL_FRect pEnemyRect) override;
	void setAsASpecialWeapon() override;
	void accept(WeaponVisitor& pWeaponVisitor) override;

private:
	void seterModes();

private:
	bool mWasReload{ false };
	PistolType mPistolType{ PistolType::DEFAULT };
	FireModeFactory mFireModeFactory;
	FactoryObjects mFactoryObjects;

};
