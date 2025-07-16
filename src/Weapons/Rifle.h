#pragma once
#include <SDL.h>

#include "Gun.h"

class SingleOrSequence;
class Irregular;

enum class RifleType
{
	DEFAULT = 0,
	SNIPER = 1, 
	IRREGULAR = 2
};

/*
	how to init this with your hands -> look at initRifleAutomaticaly function;
*/

class Rifle : public Gun
{
public:
	Rifle() = default;

	void initRifle(SDL_Renderer* pRenderer, RifleType pType, SDL_FRect pCharRect, const Config::ReloadConfig& pReloadConfig,
					const Config::BulletsConfig& pBulletsConfig, const Config::WeaponConfig& pWeaponConfig);
	void initRifleAutomaticaly(SDL_Renderer* pRenderer, RifleType pType, SDL_FRect pCharRect);

	void updateBullets(SDL_Renderer* pRenderer);
	std::pair<int32_t, bool> manageDamage(SDL_FRect pEnemyRect);

	void shoot() override;
	void reload() override;
	bool checkDamage(SDL_FRect pEnemyRect) override;
	void setAsASpecialWeapon() override;
	void accept(WeaponVisitor& pWeaponVisitor) override;

private:
	void seterModes();

	void makeUsualRifle(SDL_Renderer* pRenderer, SDL_FRect pCharRect);
	void makeSniperRifle(SDL_Renderer* pRenderer, SDL_FRect pCharRect);

private:
	bool mWasReload{ false };
	RifleType mRifleType{ RifleType::DEFAULT };
	FireModeFactory mFireModeFactory;
	FactoryObjects mFactoryObjects;

};