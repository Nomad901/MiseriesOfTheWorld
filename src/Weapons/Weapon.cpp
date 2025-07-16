#include "Weapons/Weapon.h"
#include "Weapons/WeaponManager.h"
#include "Game/AnimatedTexture.h"

void Weapon::setAllParameteres(const Vector2f pPos, int32_t pW, int32_t pH,
							   float pRobustness, std::pair<int32_t, int32_t> pPower, 
							   int16_t pWeight)
{
	mWeaponStats.mPos = pPos;
	mWeaponStats.mW = pW;
	mWeaponStats.mH = pH;
	mWeaponStats.mScaleOfRobustness = pRobustness;
	mWeaponStats.mPower.setBeginAndEnd(pPower.first, pPower.second);
	mWeaponStats.mWeight = pWeight;

	mCharCollision.mWeapon = { pPos.mX,pPos.mY, static_cast<float>(pW), static_cast<float>(pH) };
}

void Weapon::initWeapon(SDL_Renderer* pRenderer)
{
	mRenderer = pRenderer;
	mAnimateStateMachine.init(pRenderer);
}

const Weapon::WeaponStates& Weapon::getWeaponStates() const noexcept
{
	return mWeaponStates;
}

const Weapon::WeaponStats& Weapon::getWeaponStats() const noexcept
{
	return mWeaponStats;
}

const Weapon::CharCollision& Weapon::getCharCollisions() const noexcept
{
	return mCharCollision;
}

const Weapon::Textures& Weapon::getTextures() const noexcept
{
	return mTextures;
}

AnimateStateMachine& Weapon::getAnimatedStateMachine()
{
	return mAnimateStateMachine;
}

float Weapon::getNumberRobustness()
{
	mWeaponStats.mRobustness.generateNew();
	return mWeaponStats.mRobustness.getNumber();
}

int32_t Weapon::getPower()
{
	mWeaponStats.mPower.generateNew();
	return mWeaponStats.mPower.getNumber();
}

PATH Weapon::getCurrentPath() const noexcept
{
	return PROJECT_ROOT;
}

void Weapon::setActive(bool pActive)
{
	mWeaponStates.mIsActive = pActive;
}

void Weapon::setAvailable(bool pAvailable)
{
	mWeaponStates.mIsAvailable = pAvailable;
}

void Weapon::setWasDamage(bool pWasDamage)
{
	mWeaponStates.mWasDamage = pWasDamage;
}

void Weapon::enableBroken(bool pBrokenEnabled)
{
	mWeaponStates.mBrokenEnabled = pBrokenEnabled;
}

void Weapon::makeFreezed(bool pFreezed)
{
	mWeaponStates.mIsFreezed = pFreezed;
}

void Weapon::makeBroken(bool pBroken)
{
	mWeaponStates.mIsBroken = pBroken;
}

void Weapon::makeShoot(bool pShoot)
{
	mWeaponStates.mShootingState = pShoot;
}

void Weapon::makeReload(bool pReload)
{
	mWeaponStates.mRealodingState = pReload;
}

void Weapon::setRobustness(float pRobustness)
{
	if (pRobustness > 0.0f)
		mWeaponStats.mScaleOfRobustness = pRobustness;
}

void Weapon::setWeight(int16_t pWeight)
{
	if (pWeight > 0 && pWeight < 100)
		mWeaponStats.mWeight = pWeight;
}

void Weapon::setSize(const int32_t pW, const int32_t pH)
{
	if (pW > 0 && pW < WIN_WIDTH / 2 &&
		pH > 0 && pH < WIN_HEIGHT / 2)
	{
		mWeaponStats.mW = pW;
		mWeaponStats.mH = pH;
		
		mCharCollision.mWeapon.w = static_cast<float>(pW);
		mCharCollision.mWeapon.h = static_cast<float>(pH);
	}
}

void Weapon::setPos(const Vector2f& pPos)
{
	if (pPos.mX > 0 && pPos.mX < WIN_WIDTH &&
		pPos.mY > 0 && pPos.mY < WIN_HEIGHT &&
		!mWeaponStates.mIsFreezed)
	{
		mWeaponStats.mPos = pPos;
		
		mCharCollision.mWeapon.x = pPos.mX;
		mCharCollision.mWeapon.y = pPos.mY;
	}
}

void Weapon::setPosChar(const Vector2f& pPosChar)
{
	if (pPosChar.mX > 0 && pPosChar.mX < WIN_WIDTH &&
		pPosChar.mY > 0 && pPosChar.mY < WIN_HEIGHT)
	{
		mCharCollision.mChar.x = pPosChar.mX;
		mCharCollision.mChar.y = pPosChar.mY;
	}
}

void Weapon::setPower(std::pair<int32_t, int32_t> pPowerRange)
{
	if (pPowerRange.first >= 0 && pPowerRange.second >= 0)
		mWeaponStats.mPower.setBeginAndEnd(pPowerRange.first, pPowerRange.second);
}

void Weapon::setCharCollision(SDL_FRect pCharCollision)
{
	mCharCollision.mChar = pCharCollision;
}

void Weapon::setWeaponCollision(SDL_FRect pWeaponCollision)
{
	mCharCollision.mWeapon = pWeaponCollision;
}

void Weapon::setPaths(const PATH& pStaticPath, const PATH& pBrokenPath)
{
	if (std::filesystem::exists(pStaticPath))
	{
		mTextures.mStaticPath = pStaticPath;
		TextureManager::getInstance().appendTextureWithoutBack(mRenderer, pStaticPath,
			{ static_cast<int32_t>(mCharCollision.mWeapon.x),
			  static_cast<int32_t>(mCharCollision.mWeapon.y),
			  static_cast<int32_t>(mCharCollision.mWeapon.w),
			  static_cast<int32_t>(mCharCollision.mWeapon.h) }, { 34,177,76 });
	}
	if (std::filesystem::exists(pBrokenPath) && mWeaponStates.mBrokenEnabled)
	{
		mTextures.mBrokenWeapon = pBrokenPath;
		TextureManager::getInstance().appendTexture(mRenderer, pBrokenPath,
			{ static_cast<int32_t>(mCharCollision.mWeapon.x),
			  static_cast<int32_t>(mCharCollision.mWeapon.y),
			  static_cast<int32_t>(mCharCollision.mWeapon.w),
			  static_cast<int32_t>(mCharCollision.mWeapon.h) });
	}
}

void Weapon::setShootPath(const PATH& pShootPath, 
						  const std::unordered_map<SideOfChar, std::vector<uint32_t>>& pNumbers, int32_t pDelay, float pIntensity)
{
	if (std::filesystem::exists(pShootPath))
	{
		mTextures.mShootingPath = pShootPath;
		mAnimateStateMachine.pushStateW("ShootAnimWeapon", TypeWait::GENERAL, pShootPath, Weapon::getWeaponStats().mPos,
										 mCharCollision.mWeapon.w, mCharCollision.mWeapon.h, HorVer::HORIZONTAL, pNumbers, pDelay, pIntensity);
	}
}

void Weapon::setReloadPath(const PATH& pReloadPath,
						   const std::unordered_map<SideOfChar, std::vector<uint32_t>>& pNumbers, int32_t pDelay, float pIntensity)
{
	if (std::filesystem::exists(pReloadPath))
	{
		mTextures.mReloadingPath = pReloadPath;
		mAnimateStateMachine.pushStateW("ReloadAnimWeapon", TypeWait::GENERAL, pReloadPath, Weapon::getWeaponStats().mPos,
										 mCharCollision.mWeapon.w, mCharCollision.mWeapon.h, HorVer::HORIZONTAL, pNumbers, pDelay, pIntensity);
	}
}

int32_t Weapon::wasDamage()
{
	if (mWeaponStates.mWasDamage)
	{
		mWeaponStats.mPower.generateNew();
		return mWeaponStats.mPower.getNumber();
	}
	return 0;
}

int32_t Weapon::getSpeedOfChar(int32_t pHisSpeed)
{
	return pHisSpeed * (1 - (mWeaponStats.mWeight / 100));
}

void Weapon::updatePositions(const Vector2f& pPosChar, const Vector2f& pPosWeapon)
{
	if (pPosChar.mX > 0 && pPosChar.mX < WIN_WIDTH &&
		pPosChar.mY > 0 && pPosChar.mY < WIN_HEIGHT)
	{
		mWeaponStats.mPos = pPosWeapon;

		mCharCollision.mChar.x   = static_cast<int32_t>(pPosChar.mX);
		mCharCollision.mChar.y   = static_cast<int32_t>(pPosChar.mY);

		mCharCollision.mWeapon.x = static_cast<int32_t>(pPosWeapon.mX);
		mCharCollision.mWeapon.y = static_cast<int32_t>(pPosWeapon.mY);
	}
}

//void Weapon::defaultParametersWeapons()
//{
//	mWeaponStats.mScaleOfRobustness = 100;
//	mWeaponStats.mWeight = 20;
//	mWeaponStats.mPower.setBeginAndEnd(0, 5);
//	
//	//mBulletsManager.mDelay = 100;
//	//mBulletsManager.mQuantityBullets = 60;
//	//mBulletsManager.mOriginalQuantityBullets = 60;
//	//mBulletsManager.mQuantityOfSets = 3;
//}