#pragma once
#include "Melee.h"

class Knife : public Melee
{
public:
	Knife() = default;

	void initKnife(SDL_Renderer* pRenderer, const Config::WeaponConfig& pConfig,
				   SDL_FRect pCharRect);
	void initKnifeAutomatically(SDL_Renderer* pRenderer, SDL_FRect pCharRect);

	std::pair<int32_t, bool> manageDamage(SDL_FRect pEnemyRect);

	void attack() override;
	bool checkDamage(SDL_FRect pEnemyRect) override;
	void setAsASpecialWeapon() override;
	void accept(WeaponVisitor& pWeaponVisitor) override;

private:
	bool manageDelay();

private:
	FactoryObjects mFactoryObjects;
	Timer mTimer;

};