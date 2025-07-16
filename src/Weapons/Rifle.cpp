#include "Rifle.h"
#include "Irregular.h"
#include "SingleOrSequence.h"

void Rifle::initRifle(SDL_Renderer* pRenderer, RifleType pType, SDL_FRect pCharRect,
					  const Config::ReloadConfig& pReloadConfig, 
					  const Config::BulletsConfig& pBulletsConfig, 
					  const Config::WeaponConfig& pWeaponConfig)
{
	mRifleType = pType;
	Weapon::setAllParameteres(pWeaponConfig.mPosWeapon, pWeaponConfig.mW, pWeaponConfig.mH,
							  pWeaponConfig.mRobustness, pWeaponConfig.mPower, pWeaponConfig.mWeight);
	Gun::initGun(pRenderer, pReloadConfig, pCharRect);
	seterModes();
	auto tmpMode = mFireModeFactory.getMode().value();
	tmpMode.get().init(pRenderer, pBulletsConfig.mCapacityBullets, pBulletsConfig.mPathBullets, pBulletsConfig.mDelayBullets,
					   pBulletsConfig.mQuantityBullets, pBulletsConfig.mQuantitySetsBullets, pBulletsConfig.mPosBullets,
					   pBulletsConfig.mW, pBulletsConfig.mH, pBulletsConfig.mSpeedBullets);
}

void Rifle::initRifleAutomaticaly(SDL_Renderer* pRenderer, RifleType pType, SDL_FRect pCharRect)
{
	mRifleType = pType;
	if (pType == RifleType::SNIPER)
		makeSniperRifle(pRenderer, pCharRect);
	else
		makeUsualRifle(pRenderer, pCharRect);
	setAsASpecialWeapon();
}

void Rifle::updateBullets(SDL_Renderer* pRenderer)
{
	mFireModeFactory.getMode().value().get().render();
	mFireModeFactory.getMode().value().get().update(pRenderer, Weapon::getWeaponStats().mPos);
	if (mFireModeFactory.getMode().value().get().shouldBreakWeapon())
	{
		Gun::manageRobustness();
		mFireModeFactory.getMode().value().get().setShouldBreakWeapon(false);
	}
}

std::pair<int32_t, bool> Rifle::manageDamage(SDL_FRect pEnemyRect)
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

void Rifle::shoot()
{
	auto tmpFireMode = mFireModeFactory.getMode().value();
	if (!Weapon::getWeaponStates().mRealodingState &&
		!Weapon::getWeaponStates().mIsBroken &&
		!Weapon::getWeaponStates().mIsFreezed)
	{	
		Weapon::makeShoot(true);
		tmpFireMode.get().shoot(mFactoryObjects.convertFRect(Weapon::getCharCollisions().mChar),
								mFactoryObjects.convertFRect(Weapon::getCharCollisions().mWeapon),
								mWasReload);
		mWasReload = false;
	}
	if (tmpFireMode.get().getQuantityBullets() == 0)
		reload();
}

void Rifle::reload()
{
	mWasReload = true;
	if (!Weapon::getWeaponStates().mRealodingState)
		 Weapon::makeReload(true);
}

bool Rifle::checkDamage(SDL_FRect pEnemyRect)
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

void Rifle::setAsASpecialWeapon()
{
	if(mRifleType != RifleType::SNIPER)
		mFireModeFactory.getMode().value().get().setAsSpecial();
	if (mRifleType == RifleType::SNIPER)
		Weapon::setAllParameteres(Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW,
								  Weapon::getWeaponStats().mH, 100, std::make_pair(8, 12), 25);
	else
		Weapon::setAllParameteres(Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW,
								  Weapon::getWeaponStats().mH, 100, std::make_pair(4, 18), 15);
}

void Rifle::accept(WeaponVisitor& pWeaponVisitor)
{
	pWeaponVisitor.visit(*this);
}

void Rifle::seterModes()
{
	switch (mRifleType)
	{
	case RifleType::DEFAULT:
		mFireModeFactory.appendMode("SingleOrSequence", std::make_unique<SingleOrSequence>());
		mFireModeFactory.setMode("SingleOrSequence");
		mFireModeFactory.getExactMode<SingleOrSequence>().value().get().setMode(Mode::SEQUENCE);
		break;
	case RifleType::SNIPER:
		mFireModeFactory.appendMode("SingleOrSequence", std::make_unique<SingleOrSequence>());
		mFireModeFactory.setMode("SingleOrSequence");
		mFireModeFactory.getExactMode<SingleOrSequence>().value().get().setMode(Mode::SINGLE);
		break;
	case RifleType::IRREGULAR:
		mFireModeFactory.appendMode("Irregular", std::make_unique<Irregular>());
		mFireModeFactory.setMode("Irregular");
		break;
	}
}

void Rifle::makeUsualRifle(SDL_Renderer* pRenderer, SDL_FRect pCharRect)
{
	initRifle(pRenderer, mRifleType, pCharRect,
			  Gun::Config::ReloadConfig{ false, 4, 25, {255,255,255,255} },
			  Gun::Config::BulletsConfig{ 500, getCurrentPath() / "Assets" / "photos and ttf" / "bulletV2.png", 100,
			  							  45,4,30,30,3,getWeaponStats().mPos },
			  Gun::Config::WeaponConfig{ 100,15, std::make_pair(4, 8),100,100, getWeaponStats().mPos });
	Weapon::setPaths(getCurrentPath() / "Assets" / "photos and ttf" / "rifle.png",
					 getCurrentPath() / "Assets" / "photos and ttf" / "brokenRifle.png");
	Weapon::setShootPath(getCurrentPath() / "Assets" / "photos and ttf" / "shootRifle.png",
						{ {SideOfChar::RIGHT, {0,1,2}} }, 50, 300);
	Weapon::setReloadPath(getCurrentPath() / "Assets" / "photos and ttf" / "reloadRifleV2.png",
						  { {SideOfChar::RIGHT, {0,1,2,3,4,5}} }, 50, 700);
}

void Rifle::makeSniperRifle(SDL_Renderer* pRenderer, SDL_FRect pCharRect)
{
	initRifle(pRenderer, mRifleType, pCharRect,
			  Gun::Config::ReloadConfig{ false, 5, 25, {255,255,255,255} },
			  Gun::Config::BulletsConfig{ 500, getCurrentPath() / "Assets" / "photos and ttf" / "bulletV2.png", 1000,
			  							  10,3,30,30,2,getWeaponStats().mPos },
			  Gun::Config::WeaponConfig{ 100,25, std::make_pair(8, 12),100,100, getWeaponStats().mPos });
	Weapon::setPaths(getCurrentPath() / "Assets" / "photos and ttf" / "sniperRifle.png",
					 getCurrentPath() / "Assets" / "photos and ttf" / "brokenSniper.png");
	Weapon::setShootPath(getCurrentPath() / "Assets" / "photos and ttf" / "sniperShoot.png",
						 { {SideOfChar::RIGHT, {0,1,2}} }, 50, 300);
	Weapon::setReloadPath(getCurrentPath() / "Assets" / "photos and ttf" / "sniperReload.png",
						  { {SideOfChar::RIGHT, {0,1,2,3,4,5}} }, 50, 900);
}
