#include "Knife.h"


void Knife::initKnife(SDL_Renderer* pRenderer, const Config::WeaponConfig& pConfig, SDL_FRect pCharRect)
{
	Melee::initMelee(pRenderer, pConfig, pCharRect);
	Weapon::setAllParameteres(pConfig.mPosWeapon, pConfig.mW, pConfig.mH, 
							  pConfig.mRobustness, pConfig.mPower, pConfig.mWeight);
}

void Knife::initKnifeAutomatically(SDL_Renderer* pRenderer, SDL_FRect pCharRect)
{
	initKnife(pRenderer, 
			  Config::WeaponConfig{ 10,200,100, pCharRect, std::make_pair(3,5),100,100, Weapon::getWeaponStats().mPos }, 
			  pCharRect);
	Weapon::setPaths(getCurrentPath()  / "Assets" / "photos and ttf" / "knife.png",
					 getCurrentPath()  / "Assets" / "photos and ttf" / "brokenKnife.png");
	Weapon::setShootPath(getCurrentPath() / "Assets" / "photos and ttf" / "knifeHitting.png",
						 { {SideOfChar::RIGHT,{0,1,2,3,4,5}} }, 10, 200);
	setAsASpecialWeapon();
}

std::pair<int32_t, bool> Knife::manageDamage(SDL_FRect pEnemyRect)
{
	if (mFactoryObjects.areColliding(mFactoryObjects.convertFRect(pEnemyRect),
									 mFactoryObjects.convertFRect(Melee::getHitBoxAttack())) &&
		!manageDelay() && 
		Weapon::getWeaponStates().mShootingState)
	{
		setCurrentRobustness(getCurrentRobustness() - Weapon::getNumberRobustness());
		Weapon::makeShoot(false);
		return std::make_pair(Knife::getPower(), true);
	}
	return std::make_pair(0, false);
}

void Knife::attack()
{
	if (!manageDelay())
	{
		if (!Weapon::getWeaponStates().mIsFreezed &&
			!Weapon::getWeaponStates().mIsBroken)
		{
			Weapon::makeShoot(true);
			mTimer.startTimer();
		}
	}
}

bool Knife::checkDamage(SDL_FRect pEnemyRect)
{
	return mFactoryObjects.areColliding(mFactoryObjects.convertFRect(pEnemyRect),
										mFactoryObjects.convertFRect(Melee::getHitBoxAttack()));
}

void Knife::setAsASpecialWeapon()
{
	Weapon::setAllParameteres(Weapon::getWeaponStats().mPos, 100, 100, 100, std::make_pair(3, 5), 10);
}

void Knife::accept(WeaponVisitor& pWeaponVisitor)
{
	pWeaponVisitor.visit(*this);
}

bool Knife::manageDelay()
{
	if (!mTimer.isRunning())
		return false;
	if(mTimer.getDeltaTime(false) >= mConfig.mWeaponConfig.mDelay)
		mTimer.stopTimer();
	return true;
}
