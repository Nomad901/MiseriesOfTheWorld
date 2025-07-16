#include "SpreadMode.h"

SpreadMode::SpreadMode()
{
	mTimer.setDimensionOfTime(Dimension::MILISECONDS);
}

void SpreadMode::shoot(SDL_Rect pCharRect, SDL_Rect pWeaponRect, bool pWasReload)
{
	static int32_t tmpBreakingWeapon = getFireStat().mOriginalQuantityBullets - 2;

	manageReload(tmpBreakingWeapon, pWasReload);

	if (getFireStat().mQuantitySets != 0 && getFireStat().mQuantityBullets != 0)
	{
		if (manageDelay())
			return;

		mRotateMachine.calculateRadians(FireMode::getPos(), InputManager::getInstance().getMousePos());
		int32_t tmpCounter = 0;
		for (size_t i = 0; i < mQuantityBulletsPerTime; ++i)
		{
			mBulletsPool->makeBulletActive();
			mBulletsPool->manageLastBulletInside()->setSpeed(getFireStat().mSpeed);
			Vector2f tmpPos = { static_cast<float>(pCharRect.x), static_cast<float>(pCharRect.y) };
			mBulletsPool->manageLastBulletInside()->shootBullet({ tmpPos.mX + getFireStat().mW - 40, tmpPos.mY + tmpCounter }, InputManager::getInstance().getMousePos(),
																  pWeaponRect, mRotateMachine.getAngle(), { static_cast<float>(pWeaponRect.w / 2 - 10),-5 },
																  pCharRect.w, pCharRect.h);
			tmpCounter += mMultiplier;
			mRotateMachine.calculateRadians(FireMode::getPos(), InputManager::getInstance().getMousePos());
		}
		if (getFireStat().mQuantityBullets <= tmpBreakingWeapon)
		{
			setShouldBreakWeapon(true);
			subtractionBullets(tmpBreakingWeapon);
		}
		setQuantityBullets(getFireStat().mQuantityBullets - 1);
		mTimer.startTimer();
	}
}

void SpreadMode::update(SDL_Renderer* pRenderer, const Vector2f& pPos)
{
	if (mBulletsPool->getSizeBullets() <= 0)
	{
		mBulletsPool->init(getFireStat().mCapacity, pRenderer, getFireStat().mPath, pPos,
						   getFireStat().mW, getFireStat().mH, getFireStat().mSpeed);
	}
	FireMode::getFireStat().mPos = pPos;
}

void SpreadMode::render()
{
	mBulletsPool->update();
}

void SpreadMode::setAsSpecial()
{
	setDelay(300);
	setQuantityBullets(8);
	setOrigQuantityBullets(8);
	setQuantitySets(8);
	mQuantityBulletsPerTime = 4;
}

void SpreadMode::setQuantityBulletsPerTime(int32_t pQuantityBullets)
{
	if (pQuantityBullets > 0 && pQuantityBullets < getFireStat().mQuantityBullets)
		mQuantityBulletsPerTime = pQuantityBullets;
}

void SpreadMode::setRangeOfSpread(int32_t pRange)
{
	mMultiplier = pRange;
}

bool SpreadMode::manageDelay()
{
	if (mTimer.isRunning()) {
		float delta = mTimer.getDeltaTime(false);

		if (delta < getFireStat().mDelay) {
			setDelayGoing(true);
			return true;
		}

		mTimer.stopTimer();
		setDelayGoing(false);
	}
	return false;
}

void SpreadMode::manageReload(int32_t& pTmpBreakingWeapon, bool pWasReload)
{
	if (pWasReload)
	{
		setQuantityBullets(getFireStat().mOriginalQuantityBullets);
		pTmpBreakingWeapon = getFireStat().mOriginalQuantityBullets;
		subtractionBullets(pTmpBreakingWeapon);
		setQuantitySets(getFireStat().mQuantitySets - 1);
	}
}

void SpreadMode::subtractionBullets(int32_t& pTmpBreakingWeapon)
{
	pTmpBreakingWeapon -= 2;
}
