#include "Tango.h"

void Tango::initTango(SDL_Renderer* pRenderer, SDL_FRect pCharRect, const Config::ReloadConfig& pReloadConfig, 
					  const Config::BulletsConfig& pBulletsConfig, const Config::WeaponConfig& pWeaponConfig)
{
	mTimer.setDimensionOfTime(Dimension::MILISECONDS);

	Weapon::setAllParameteres(pWeaponConfig.mPosWeapon, pWeaponConfig.mW, pWeaponConfig.mH,
							  pWeaponConfig.mRobustness, pWeaponConfig.mPower, pWeaponConfig.mWeight);
	Gun::initGun(pRenderer, pReloadConfig, pCharRect);

	mFireModeFactory.appendMode("SpreadMode", std::make_unique<BurstMode>(pRenderer));
	setAsASpecialWeapon();
	mFireModeFactory.getExactMode<BurstMode>()->get().manageAnim(pRenderer, Weapon::getWeaponStats().mPos);
	auto tmpMode = mFireModeFactory.getExactMode<BurstMode>().value();
	tmpMode.get().init(pRenderer, pBulletsConfig.mCapacityBullets, pBulletsConfig.mPathBullets, pBulletsConfig.mDelayBullets,
					   pBulletsConfig.mQuantityBullets, pBulletsConfig.mQuantitySetsBullets, pBulletsConfig.mPosBullets,
					   pBulletsConfig.mW, pBulletsConfig.mH, pBulletsConfig.mSpeedBullets);

	mAnimateStateMachine.init(pRenderer);
	mAnimateStateMachine.pushStateW("ChargingTango", TypeWait::GENERAL,
									getCurrentPath() / "Assets" / "photos and ttf" / "chargingTango.png",
									Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW, Weapon::getWeaponStats().mH,
									HorVer::HORIZONTAL, { {SideOfChar::RIGHT, {0,1,2,3,4,5,6,7,8,9}} }, 10, tmpMode.get().getItensisty());
	mAnimateStateMachine.pushStateW("ChargingTango", TypeWait::WAIT,
									getCurrentPath() / "Assets" / "photos and ttf" / "chargedTango.png",
									Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW, Weapon::getWeaponStats().mH,
									HorVer::HORIZONTAL, { {SideOfChar::RIGHT, {0,1,2,3}} }, 10, tmpMode.get().getItensisty());
	mAnimateStateMachine.getState("ChargingTango")->get().waitWithAnim(true);
	mAnimateStateMachine.getState("ChargingTango")->get().setCurrentSide(SideOfChar::RIGHT);
}

void Tango::initTangoAutomaticaly(SDL_Renderer* pRenderer, SDL_FRect pCharRect)
{

	initTango(pRenderer, pCharRect,
			  Gun::Config::ReloadConfig{ false, 6, 25, {255,255,255,255} },
			  Gun::Config::BulletsConfig{ 500, getCurrentPath() / "Assets" / "photos and ttf" / "tangoBullet.png", 100,
			  							  60,6,30,30,3,getWeaponStats().mPos },
			  Gun::Config::WeaponConfig{ 100,20, std::make_pair(3, 5),100,100, getWeaponStats().mPos });
	Weapon::setPaths(getCurrentPath() / "Assets" / "photos and ttf" / "tango.png",
					 getCurrentPath() / "Assets" / "photos and ttf" / "brokenTango.png");
	Weapon::setShootPath(getCurrentPath() / "Assets" / "photos and ttf" / "shootTango.png",
						{ {SideOfChar::RIGHT, {0,1,2}} }, 50, 100);
	Weapon::setReloadPath(getCurrentPath() / "Assets" / "photos and ttf" / "reloadTango.png",
						 { {SideOfChar::RIGHT, {0,1,2,3}} }, 10, 200);
	Gun::setReloadAnimationEndless(true);
}

void Tango::updateBullets(SDL_Renderer* pRenderer)
{
	mFireModeFactory.getExactMode<BurstMode>().value().get().render();
	mFireModeFactory.getExactMode<BurstMode>().value().get().update(pRenderer, Weapon::getWeaponStats().mPos);
	if (mFireModeFactory.getExactMode<BurstMode>().value().get().shouldBreakWeapon())
	{
		Gun::manageRobustness();
		mFireModeFactory.getExactMode<BurstMode>().value().get().setShouldBreakWeapon(false);
	}
}

std::pair<int32_t, bool> Tango::manageDamage(SDL_FRect pEnemyRect)
{
	checkDamage(pEnemyRect);
	auto tmpFireMode = mFireModeFactory.getExactMode<BurstMode>();
	if (!tmpFireMode)
	{
		LOG("Unproper value!");
		return std::make_pair(0, false);
	}
	if (tmpFireMode.value().get().wasDamage())
		return std::make_pair(Weapon::getPower(), true);
	return std::make_pair(0, false);
}

BurstMode& Tango::getFireMode() 
{
	return mFireModeFactory.getExactMode<BurstMode>().value().get();
}

void Tango::shoot()
{
	auto tmpFireMode = mFireModeFactory.getExactMode<BurstMode>().value();
	if (tmpFireMode.get().manageDelay())
	{
		setTangoIsWaiting(true);
		return;
	}
	if (!Weapon::getWeaponStates().mRealodingState &&
		!Weapon::getWeaponStates().mIsBroken &&
		!Weapon::getWeaponStates().mIsFreezed)
	{
		setTangoIsWaiting(false);
		tmpFireMode.get().shootChargedBullets(mFactoryObjects.convertFRect(Weapon::getCharCollisions().mChar),
											  mFactoryObjects.convertFRect(Weapon::getCharCollisions().mWeapon));
		Weapon::makeShoot(true);
		mWasReload = false;
	}
	if (tmpFireMode.get().getQuantityBullets() == 0)
	{
		reload();
	}
}

void Tango::reload()
{
	mWasReload = true;
	if (!Weapon::getWeaponStates().mRealodingState)
		Weapon::makeReload(true);
}

bool Tango::checkDamage(SDL_FRect pEnemyRect)
{
	auto result = mFireModeFactory.getExactMode<BurstMode>();
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

void Tango::setAsASpecialWeapon()
{
	mFireModeFactory.getExactMode<BurstMode>().value().get().setAsSpecial();
	Weapon::setAllParameteres(Weapon::getWeaponStats().mPos, Weapon::getWeaponStats().mW,
							  Weapon::getWeaponStats().mH, 100, std::make_pair(7, 10), 40);
}

void Tango::accept(WeaponVisitor& pWeaponVisitor)
{
	pWeaponVisitor.visit(*this);
}

void Tango::setBulletsPerShot(int32_t pQuantity)
{
	mFireModeFactory.getExactMode<BurstMode>()->get().setQuantityBulletsPerTime(pQuantity);
}

void Tango::setRangeOfBullets(int32_t pRange)
{
	mFireModeFactory.getExactMode<BurstMode>()->get().setRangeOfSpread(pRange);
}

void Tango::setDelayOfStoraging(int32_t pDelay)
{
	mFireModeFactory.getExactMode<BurstMode>()->get().setDelayOfStoraging(pDelay);
}

void Tango::renderCharge(SDL_Renderer* pRenderer)
{
	mFireModeFactory.getExactMode<BurstMode>()->get().render();
	mFireModeFactory.getExactMode<BurstMode>()->get().update(pRenderer,
															 Weapon::getWeaponStats().mPos);

	mAnimateStateMachine.getState("ChargingTango")->get().setPosition(Weapon::getWeaponStats().mPos);
	if (mFireModeFactory.getExactMode<BurstMode>()->get().isSparing())
	{
		setTangoIsCharging(true);
		if (mFireModeFactory.getExactMode<BurstMode>()->get().manageDelay())
			return;

		mAnimateStateMachine.setCurrentState("ChargingTango");
		auto& tmpAnimState = mAnimateStateMachine.getState().value().get();
		if (mFireModeFactory.getExactMode<BurstMode>()->get().isSparing() &&
			mFireModeFactory.getExactMode<BurstMode>()->get().getQuantityBullets() > 0)
		{
			tmpAnimState.setActive(true);
			if (tmpAnimState.isEnded())
			{
				tmpAnimState.setActive(false);
				tmpAnimState.enableWaitAnim(true);
				tmpAnimState.runAnimation();
			}
			if (!tmpAnimState.isAnimating())
			{
				tmpAnimState.enableWaitAnim(false);
				tmpAnimState.runAnimationOnlyOnce();
			}
			mAnimateStateMachine.render("ChargingTango", true, getAngleOfWeapon() * 180 / M_PI);
		}
		else
		{
			tmpAnimState.nullTicks();
			tmpAnimState.setActive(false);
			tmpAnimState.enableWaitAnim(false);
		}
	}
	else
		setTangoIsCharging(false);
}

