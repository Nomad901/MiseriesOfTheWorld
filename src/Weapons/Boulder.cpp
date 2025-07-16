#include "Boulder.h"
#include "SingleOrSequence.h"

void Boulder::initBoulder(SDL_Renderer* pRenderer, const Config::WeaponConfig& pWeaponConfig,
						  SDL_FRect pRect)
{
	Weapon::setAllParameteres(pWeaponConfig.mPosWeapon, pWeaponConfig.mW, 
							  pWeaponConfig.mH, 101, pWeaponConfig.mPower, 5);
	initThrowable(pRenderer, pWeaponConfig, pRect);
	mFireModeFactory.appendMode("Single", std::make_unique<SingleOrSequence>());
	auto mode = mFireModeFactory.getExactMode<SingleOrSequence>().value();
	mode.get().init(pRenderer, 500, pWeaponConfig.mPathBullets,
					pWeaponConfig.mDelayBullets, 500, 500, pWeaponConfig.mPosWeapon,
					pWeaponConfig.mW, pWeaponConfig.mH, 3);
	mode.get().setMode(Mode::SINGLE);
	mode.get().getBulletsPoolInstance().makeBulletsCirclingAround(true, 0.1f);
}

void Boulder::initBoulderAutomatically(SDL_Renderer* pRenderer, SDL_FRect pRect)
{
	Weapon::enableBroken(false);
	initBoulder(pRenderer,
				Throwable::Config::WeaponConfig{ getCurrentPath() / "Assets" / "photos and ttf" / "boulder.png",
												 std::make_pair(1, 3),100,100, 400, 3, Weapon::getWeaponStats().mPos }, pRect);
	Weapon::setPaths(getCurrentPath() / "Assets" / "photos and ttf" / "boulder.png",
					 getCurrentPath());
	setAsASpecialWeapon();
}

void Boulder::updateBullets(SDL_Renderer* pRenderer)
{
	mFireModeFactory.getMode().value().get().render();
	mFireModeFactory.getMode().value().get().update(pRenderer, Weapon::getWeaponStats().mPos);
}

std::pair<int32_t, bool> Boulder::manageDamage(SDL_FRect pEnemyRect)
{
	checkDamage(pEnemyRect);
	auto tmpFireMode = mFireModeFactory.getMode();
	if (!tmpFireMode)
	{
		LOG("Unproper value!");
		return std::make_pair(0, false);
	}
	if (tmpFireMode.value().get().wasDamage())
		return std::make_pair(Weapon::getPower(), true);
	return std::make_pair(0, false);
}

void Boulder::shoot()
{
	auto tmpFireMode = mFireModeFactory.getMode().value();
	if (!Weapon::getWeaponStates().mRealodingState &&
		!Weapon::getWeaponStates().mIsBroken &&
		!Weapon::getWeaponStates().mIsFreezed)
	{
		tmpFireMode.get().shoot(mFactoryObjects.convertFRect(Weapon::getCharCollisions().mChar),
								mFactoryObjects.convertFRect(Weapon::getCharCollisions().mWeapon),
								false);
		Weapon::makeShoot(true);
	}
	if (tmpFireMode.get().getQuantityBullets() == 0)
	{
		tmpFireMode.get().setQuantityBullets(500);
		tmpFireMode.get().setQuantitySets(500);
	}
}

bool Boulder::checkDamage(SDL_FRect pEnemyRect)
{
	auto result = mFireModeFactory.getMode();
	if (!result)
	{
		LOG("Unproper Value!");
		return false;
	}
	auto& fireMode = result.value();
	auto& bulletsPool = fireMode.get().getBulletsPoolInstance();
	auto intersection = bulletsPool.hasIntersection(mFactoryObjects.convertFRect(pEnemyRect));

	if (intersection.second)
	{
		fireMode.get().setWasDamage(true);
		fireMode.get().getBulletsPoolInstance().destroyBullet(intersection.first);
		return true;
	}
	fireMode.get().setWasDamage(false);
	return false;
}

void Boulder::setAsASpecialWeapon()
{
	mFireModeFactory.getExactMode<SingleOrSequence>()->get().setAsSpecial();
	Weapon::setAllParameteres(Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW, Weapon::getWeaponStats().mH,
							  101, std::make_pair(1, 3), 5);
}

void Boulder::accept(WeaponVisitor& pWeaponVisitor)
{
	pWeaponVisitor.visit(*this);
}
