#include "Pistol.h"
#include "SingleOrSequence.h"

void Pistol::initPistol(SDL_Renderer* pRenderer, PistolType pType, SDL_FRect pCharRect, const Config::ReloadConfig& pReloadConfig,
					    const Config::BulletsConfig& pBulletsConfig, const Config::WeaponConfig& pWeaponConfig)
{
	mPistolType = pType;
	Weapon::setAllParameteres(pWeaponConfig.mPosWeapon, pWeaponConfig.mW, pWeaponConfig.mH, 
							  pWeaponConfig.mRobustness, pWeaponConfig.mPower, pWeaponConfig.mWeight);
	Gun::initGun(pRenderer, pReloadConfig, pCharRect);
	seterModes();
	auto tmpMode = mFireModeFactory.getMode().value();
	tmpMode.get().init(pRenderer, pBulletsConfig.mCapacityBullets, pBulletsConfig.mPathBullets, pBulletsConfig.mDelayBullets,
					   pBulletsConfig.mQuantityBullets, pBulletsConfig.mQuantitySetsBullets, pBulletsConfig.mPosBullets,
					   pBulletsConfig.mW, pBulletsConfig.mH, pBulletsConfig.mSpeedBullets);
}

void Pistol::initPistolAutomaticaly(SDL_Renderer* pRenderer, PistolType pType, SDL_FRect pCharRect)
{
	initPistol(pRenderer, pType, pCharRect,
			   Gun::Config::ReloadConfig{ false, 3, 25, {255,255,255,255} },
			   Gun::Config::BulletsConfig{ 500, getCurrentPath() / "Assets" / "photos and ttf" / "bulletV2.png", 100,
										   60,6,30,30,3,getWeaponStats().mPos },
			   Gun::Config::WeaponConfig{ 100,5, std::make_pair(0, 5),100,100, getWeaponStats().mPos });
	Weapon::setPaths(getCurrentPath() / "Assets" / "photos and ttf" / "Pistol.png", 
					 getCurrentPath() / "Assets" / "photos and ttf" / "brokenPistol.png");
	Weapon::setShootPath(getCurrentPath() / "Assets" / "photos and ttf" / "PistolSh.png",
				      { {SideOfChar::RIGHT, {0,1,2}} }, 50, 300);
	Weapon::setReloadPath(getCurrentPath() / "Assets" / "photos and ttf" / "PistolRel.png",
					   { {SideOfChar::RIGHT, {0,1,2,3,4,5,6,7,8,9,10,11,12}} }, 50, 220);
	setAsASpecialWeapon();
}

void Pistol::shoot()
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

void Pistol::reload()
{
	mWasReload = true;
	if (!Weapon::getWeaponStates().mRealodingState)
		Weapon::makeReload(true);
}

bool Pistol::checkDamage(SDL_FRect pEnemyRect)
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

void Pistol::setAsASpecialWeapon()
{
	mFireModeFactory.getMode().value().get().setAsSpecial();
	Weapon::setAllParameteres(Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW, 
							  Weapon::getWeaponStats().mH, 100, std::make_pair(2, 5), 5);
}

void Pistol::accept(WeaponVisitor& pWeaponVisitor)
{
	pWeaponVisitor.visit(*this);
}

void Pistol::seterModes()
{
	if (mPistolType == PistolType::DEFAULT)
	{
		mFireModeFactory.appendMode("SingleOrSequence", std::make_unique<SingleOrSequence>());
		mFireModeFactory.setMode("SingleOrSequence");
		mFireModeFactory.getExactMode<SingleOrSequence>().value().get().setMode(Mode::SINGLE);
	}
	else if (mPistolType == PistolType::FAST)
	{
		mFireModeFactory.appendMode("SingleOrSequence", std::make_unique<SingleOrSequence>());
		mFireModeFactory.setMode("SingleOrSequence");
		mFireModeFactory.getExactMode<SingleOrSequence>().value().get().setMode(Mode::SEQUENCE);
	}
}

void Pistol::updateBullets(SDL_Renderer* pRenderer)
{
	mFireModeFactory.getMode().value().get().render();
	mFireModeFactory.getMode().value().get().update(pRenderer, Weapon::getWeaponStats().mPos);
	if (mFireModeFactory.getMode().value().get().shouldBreakWeapon())
	{
		Gun::manageRobustness();
		mFireModeFactory.getMode().value().get().setShouldBreakWeapon(false);
	}
}

std::pair<int32_t, bool> Pistol::manageDamage(SDL_FRect pEnemyRect)
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


