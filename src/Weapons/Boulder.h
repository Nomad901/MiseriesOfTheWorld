#pragma once
#include "Throwable.h"
#include "FireModeFactory.h"

class SingleOrSequence;

class Boulder : public Throwable
{
public:
	Boulder() = default;

	void initBoulder(SDL_Renderer* pRenderer, const Config::WeaponConfig& pWeaponConfig,
					 SDL_FRect pRect);
	void initBoulderAutomatically(SDL_Renderer* pRenderer, SDL_FRect pRect);

	std::pair<int32_t, bool> manageDamage(SDL_FRect pEnemyRect);

	void updateBullets(SDL_Renderer* pRenderer) override;
	void shoot() override;
	bool checkDamage(SDL_FRect pEnemyRect) override;
	void setAsASpecialWeapon() override;
	void accept(WeaponVisitor& pWeaponVisitor) override;

private:
	FireModeFactory mFireModeFactory;
	FactoryObjects mFactoryObjects;

};