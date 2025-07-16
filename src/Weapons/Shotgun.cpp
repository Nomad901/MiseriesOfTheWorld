#include "Shotgun.h"
#include "SpreadMode.h"

void Shotgun::initShotgun(SDL_Renderer* pRenderer, SDL_FRect pCharRect, const Config::ReloadConfig& pReloadConfig, 
						  const Config::BulletsConfig& pBulletsConfig, const Config::WeaponConfig& pWeaponConfig)
{
	Weapon::setAllParameteres(pWeaponConfig.mPosWeapon, pWeaponConfig.mW, pWeaponConfig.mH,
		pWeaponConfig.mRobustness, pWeaponConfig.mPower, pWeaponConfig.mWeight);
	Gun::initGun(pRenderer, pReloadConfig, pCharRect);

	mFireModeFactory.appendMode("SpreadMode", std::make_unique<SpreadMode>());
	auto tmpMode = mFireModeFactory.getExactMode<SpreadMode>().value();
	tmpMode.get().init(pRenderer, pBulletsConfig.mCapacityBullets, pBulletsConfig.mPathBullets, pBulletsConfig.mDelayBullets,
					   pBulletsConfig.mQuantityBullets, pBulletsConfig.mQuantitySetsBullets, pBulletsConfig.mPosBullets,
					   pBulletsConfig.mW, pBulletsConfig.mH, pBulletsConfig.mSpeedBullets);
}

void Shotgun::initShotgunAutomaticaly(SDL_Renderer* pRenderer, SDL_FRect pCharRect)
{
	initShotgun(pRenderer, pCharRect,
				Gun::Config::ReloadConfig{ false, 6, 25, {255,255,255,255} },
				Gun::Config::BulletsConfig{ 500, getCurrentPath() / "Assets" / "photos and ttf" / "bulletV2.png", 100,
											60,6,30,30,3,getWeaponStats().mPos },
				Gun::Config::WeaponConfig{ 100,20, std::make_pair(3, 5),100,100, getWeaponStats().mPos });
	Weapon::setPaths(getCurrentPath() / "Assets" / "photos and ttf" / "Shotgun.png",
					 getCurrentPath() / "Assets" / "photos and ttf" / "brokenShotgun.png");
	Weapon::setShootPath(getCurrentPath() / "Assets" / "photos and ttf" / "shootShotgun.png",
						{ {SideOfChar::RIGHT, {0,1,2}} }, 50, 100);
	Weapon::setReloadPath(getCurrentPath() / "Assets" / "photos and ttf" / "reloadShotgun.png",
						 { {SideOfChar::RIGHT, {0,1,2,3}} }, 10, 200);
	Gun::setReloadAnimationEndless(true);
	setAsASpecialWeapon();
}

void Shotgun::updateBullets(SDL_Renderer* pRenderer)
{
	mFireModeFactory.getMode().value().get().render();
	mFireModeFactory.getMode().value().get().update(pRenderer, Weapon::getWeaponStats().mPos);
	if (mFireModeFactory.getMode().value().get().shouldBreakWeapon())
	{
		Gun::manageRobustness();
		mFireModeFactory.getMode().value().get().setShouldBreakWeapon(false);
	}
}

std::pair<int32_t, bool> Shotgun::manageDamage(SDL_FRect pEnemyRect)
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

void Shotgun::shoot()
{
	auto tmpFireMode = mFireModeFactory.getMode().value();
	if (!Weapon::getWeaponStates().mRealodingState &&
		!Weapon::getWeaponStates().mIsBroken &&
		!Weapon::getWeaponStates().mIsFreezed)
	{
		tmpFireMode.get().shoot(mFactoryObjects.convertFRect(Weapon::getCharCollisions().mChar),
			mFactoryObjects.convertFRect(Weapon::getCharCollisions().mWeapon),
			mWasReload);
		Weapon::makeShoot(true);
		mWasReload = false;
	}
	if (tmpFireMode.get().getQuantityBullets() == 0)
	{
		reload();
	}
}

void Shotgun::reload()
{
	mWasReload = true;
	if (!Weapon::getWeaponStates().mRealodingState)
		Weapon::makeReload(true);
}

bool Shotgun::checkDamage(SDL_FRect pEnemyRect)
{
	auto result = mFireModeFactory.getExactMode<SpreadMode>();
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

void Shotgun::setAsASpecialWeapon()
{
	mFireModeFactory.getExactMode<SpreadMode>().value().get().setAsSpecial();
	Weapon::setAllParameteres(Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW,
							  Weapon::getWeaponStats().mH, 100, std::make_pair(3, 5), 20);
}

void Shotgun::accept(WeaponVisitor& pWeaponVisitor)
{
	pWeaponVisitor.visit(*this);
}

void Shotgun::setBulletsPerShot(int32_t pQuantity)
{
	mFireModeFactory.getExactMode<SpreadMode>()->get().setQuantityBulletsPerTime(pQuantity);
}

void Shotgun::setRangeOfBullets(int32_t pRange)
{
	mFireModeFactory.getExactMode<SpreadMode>()->get().setRangeOfSpread(pRange);
}
